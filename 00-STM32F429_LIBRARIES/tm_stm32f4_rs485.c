/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "tm_stm32f4_rs485.h"

TM_RS485_t RS485_Settings;
void TM_RS485_InitPins(void);

void TM_RS485_Init(uint8_t id, uint32_t baudrate) {
	/* Set ID for itself */
	RS485_Settings.id = id;
	
	/* Initialize RX/TX pin */
	TM_RS485_InitPins();
	/* Initialize USART */
	TM_USART_Init(RS485_USART, RS485_USART_PINSPACK, baudrate);
}

void TM_RS485_InitPins(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Enable RX/TX pin clock */
	RCC_AHB1PeriphClockCmd(RS485_RXTX_ENABLE_RCC, ENABLE);
	
	/* Set pin settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = RS485_RXTX_ENABLE_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	/* Initialize GPIO */
	GPIO_Init(RS485_RXTX_ENABLE_PORT, &GPIO_InitStruct);
	
	/* Go to RX mode */
	RS485_ENABLE_RX;
}

uint8_t TM_RS485_Send(uint8_t receiver, uint8_t* data, uint8_t count) {
	//start + sender + receiver + count + allbytes + crcMSB + crcLSB + end
	uint8_t y, tmp[RS485_BUFFER_LENGTH], i = 0;
	uint16_t calculatedCRC;
	volatile uint32_t delay;
	
	/* Check size */
	if (count > (RS485_BUFFER_LENGTH - 7)) {
		count = RS485_BUFFER_LENGTH - 7;
	}
	
	tmp[i++] = RS485_START_BYTE;					/* Send start byte */
	tmp[i++] = RS485_Settings.id;					/* Send sender ID */
	tmp[i++] = receiver;							/* Send receiver ID */
	tmp[i++] = count;								/* Send number of data */
	
	for (y = 0; y < count; y++) {					/* Send data */
		tmp[i++] = *(data + y);
	}
	
	/* Calculate CRC */
	calculatedCRC = TM_SOFTCRC_Generate(tmp, count + 4);
	
	tmp[i++] = (calculatedCRC >> 8) & 0xFF;			/* Send CRC MSB */
	tmp[i++] = calculatedCRC & 0xFF;				/* Send CRC LSB */
	tmp[i++] = RS485_STOP_BYTE;						/* Send stop byte */
	
	/* Enable transmitter */
	RS485_ENABLE_TX;
	
	/* Little delay */
	for (delay = 0; delay < RS485_DELAY; delay++);
	
	/* Send byte by byte */
	for (y = 0; y < i; y++) {
		TM_USART_Putc(RS485_USART, tmp[y]);
	}
	
	/* Little delay */
	for (delay = 0; delay < RS485_DELAY; delay++);
	
	/* Go back to RX mode */
	RS485_ENABLE_RX;
		
	/* Data sent OK */
	return 1;
}

uint8_t TM_RS485_Receive(uint8_t* from, uint8_t* data, uint8_t* count) {
	uint8_t c, toID, bytesCount, i, crcMSB, crcLSB, crcData[RS485_BUFFER_LENGTH];
	uint16_t calculatedCRC, receivedCRC;
	uint32_t timeout;
	
	/* If any data received */
	if (!TM_USART_BufferEmpty(RS485_USART)) {
		/* Wait till byte is start byte */
		while (!TM_USART_BufferEmpty(RS485_USART)) {
			c = TM_USART_Getc(RS485_USART);
			if (c == RS485_START_BYTE) {
				/* Start byte received */
				break;
			}
		}
		/* If data is start byte */
		if (c == RS485_START_BYTE) {
			crcData[0] = c;
			/* Wait for another byte */
			timeout = RS485_TIMEOUT;
			while (TM_USART_BufferEmpty(RS485_USART)) {
				if (timeout-- == 0x00) {
					RS485_CLEAR_BUFFER;
					return 0;
				}
			}
			*from = TM_USART_Getc(RS485_USART); //from address
			crcData[1] = *from;
			
			/* Check sender */
			if (*from != 0x00) {
				RS485_CLEAR_BUFFER;
				return 0;
			}
			
			/* Wait another one */
			timeout = RS485_TIMEOUT;
			while (TM_USART_BufferEmpty(RS485_USART)) {
				if (timeout-- == 0x00) {
					RS485_CLEAR_BUFFER;
					return 0;
				}
			}
			/* Receiver address (me) */
			toID = TM_USART_Getc(RS485_USART); 
			crcData[2] = toID;
			
			/* If message is for us */
			if (toID == RS485_Settings.id) {
				/* Read next */
				timeout = RS485_TIMEOUT;
				while (TM_USART_BufferEmpty(RS485_USART)) {
					if (timeout-- == 0x00) {
						RS485_CLEAR_BUFFER;
						return 0;
					}
				}
				bytesCount = TM_USART_Getc(RS485_USART);
				*count = bytesCount;
				crcData[3] = bytesCount;
				
				/* Now read all bytes from count */
				for (i = 0; i < bytesCount; i++) {
					/* Wait for byte */
					timeout = RS485_TIMEOUT;
					while (TM_USART_BufferEmpty(RS485_USART)) {
						if (timeout-- == 0x00) {
							RS485_CLEAR_BUFFER;
							return 0;
						}
					}
					*(data + i) = TM_USART_Getc(RS485_USART);
					crcData[4 + i] = *(data + i);
				}
				/* Read CRC MSB */
				timeout = RS485_TIMEOUT;
				while (TM_USART_BufferEmpty(RS485_USART)) {
					if (timeout-- == 0x00) {
						RS485_CLEAR_BUFFER;
						return 0;
					}
				}
				crcMSB = TM_USART_Getc(RS485_USART);
				
				/* Read CRC LSB */
				timeout = RS485_TIMEOUT;
				while (TM_USART_BufferEmpty(RS485_USART)) {
					if (timeout-- == 0x00) {
						RS485_CLEAR_BUFFER;
						return 0;
					}
				}
				crcLSB = TM_USART_Getc(RS485_USART);
				
				receivedCRC = (crcMSB << 8) | crcLSB;
				calculatedCRC = TM_SOFTCRC_Generate(crcData, bytesCount + 4); //First 4 control bytes
				if (receivedCRC != calculatedCRC) {
					/* Clear buffer */
					RS485_CLEAR_BUFFER;
					return 0;
				}
				
				timeout = RS485_TIMEOUT;
				while (TM_USART_BufferEmpty(RS485_USART)) {
					if (timeout-- == 0x00) {
						RS485_CLEAR_BUFFER;
						return 0;
					}
				}
				c = TM_USART_Getc(RS485_USART);
				if (c != RS485_STOP_BYTE) {
					/* Clear buffer */
					RS485_CLEAR_BUFFER;
					return 0;
				}
				
				/* Data valid */
				return 1;
			} else {
				/* Clear buffer */
				RS485_CLEAR_BUFFER;
				return 0;
			}
		} else {
			/* Clear buffer */
			RS485_CLEAR_BUFFER;
			return 0;
		}
	}
	
	/* Return 0, data not valid */
	return 0;
}

