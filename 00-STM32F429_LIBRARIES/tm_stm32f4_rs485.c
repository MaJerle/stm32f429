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

/* Private functions */
void TM_RS485_INT_InitPins(void);

void TM_RS485_Init(TM_RS485_t* RS485, uint8_t id, uint32_t baudrate) {
	/* Set ID for itself */
	RS485->MyID = id;
	
	/* Initialize direction pin */
	TM_RS485_INT_InitPins();
	
	/* Initialize USART */
	TM_USART_Init(RS485_USART, RS485_USART_PINSPACK, baudrate);
	
	/* Clear USART buffer */
	RS485_CLEAR_BUFFER;
}

uint8_t TM_RS485_Send(TM_RS485_t* RS485, uint8_t receiver, uint8_t* data, uint8_t count) {
	//start + sender + receiver + count + allbytes + crcMSB + crcLSB + end
	uint8_t y, tmp[RS485_BUFFER_LENGTH], i = 0;
	uint16_t calculatedCRC;
	volatile uint32_t delay;
	
	/* Check size */
	if (count > (RS485_BUFFER_LENGTH - 7)) {
		count = RS485_BUFFER_LENGTH - 7;
	}
	
	/* Send start byte  */
	tmp[i++] = RS485_START_BYTE;
	/* Send sender ID = from */
	tmp[i++] = RS485->MyID;
	/* Send receiver ID = to */
	tmp[i++] = receiver;
	/* Send number of data */
	tmp[i++] = count;
	
	/* Send data */
	for (y = 0; y < count; y++) {
		tmp[i++] = *(data + y);
	}
	
	/* Calculate CRC */
	calculatedCRC = TM_SOFTCRC_Generate(tmp, count + 4);
	
	/* Send CRC MSB */
	tmp[i++] = (calculatedCRC >> 8) & 0xFF;
	/* Send CRC LSB */
	tmp[i++] = calculatedCRC & 0xFF;
	/* Send stop byte */
	tmp[i++] = RS485_STOP_BYTE;
	
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
		
	/* Return number of data bytes sent */
	return count;
}

uint8_t TM_RS485_Receive(TM_RS485_t* RS485, uint8_t* data) {
	uint8_t c, toID, bytesCount, i, crcMSB, crcLSB, crcData[RS485_BUFFER_LENGTH];
	uint16_t calculatedCRC, receivedCRC;
	uint32_t timeout;

	/* Buffer is empty */
	if (TM_USART_BufferEmpty(RS485_USART)) {
		return 0;
	}
	
	/* Check if START and STOP bytes are available in buffer */
	if (
		!TM_USART_FindCharacter(RS485_USART, RS485_START_BYTE) ||
		!TM_USART_FindCharacter(RS485_USART, RS485_STOP_BYTE)
	) {
		return 0;
	}
	
	/* Read till we get START address */
	do {
		/* Get new character from USART */
		c = TM_USART_Getc(RS485_USART);
	} while (c != RS485_START_BYTE);

	/* Check again for start byte */
	if (c != RS485_START_BYTE) {
		return 0;
	}
	
	/* Add start byte for CRC array */
	crcData[0] = c;
	
	/* Wait for another byte */
	timeout = RS485_TIMEOUT;
	while (TM_USART_BufferEmpty(RS485_USART)) {
		if (timeout-- == 0x00) {
			/* Timeout, probably too-slow or failed communication */
			RS485_CLEAR_BUFFER;
			return 0;
		}
	}
	/* Get ID from where this message arrived */
	RS485->ReceivedID = TM_USART_Getc(RS485_USART);
	/* Add for CRC check */
	crcData[1] = RS485->ReceivedID;
	
	/* Wait another one */
	timeout = RS485_TIMEOUT;
	while (TM_USART_BufferEmpty(RS485_USART)) {
		if (timeout-- == 0x00) {
			/* Timeout, probably too-slow or failed communication */
			RS485_CLEAR_BUFFER;
			return 0;
		}
	}
	/* Receiver address (me) */
	toID = TM_USART_Getc(RS485_USART); 
	/* Add for CRC check */
	crcData[2] = toID;
	
	/* If message is for us */
	if (toID == RS485->MyID) {
		/* Read next */
		timeout = RS485_TIMEOUT;
		while (TM_USART_BufferEmpty(RS485_USART)) {
			if (timeout-- == 0x00) {
				/* Timeout, probably too-slow or failed communication */
				RS485_CLEAR_BUFFER;
				return 0;
			}
		}
		/* Get number of bytes we want for actual data */
		bytesCount = TM_USART_Getc(RS485_USART);
		/* Store to received count number */
		RS485->ReceivedCount = bytesCount;
		/* Add for CRC check */
		crcData[3] = bytesCount;
		
		/* Now read all bytes according to count */
		for (i = 0; i < bytesCount; i++) {
			/* Wait for byte */
			timeout = RS485_TIMEOUT;
			while (TM_USART_BufferEmpty(RS485_USART)) {
				if (timeout-- == 0x00) {
					/* Timeout, probably too-slow or failed communication */
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
		
		/* Calculate CRC data */
		receivedCRC = (crcMSB << 8) | crcLSB;
		calculatedCRC = TM_SOFTCRC_Generate(crcData, bytesCount + 4);
		
		/* Check if CRC is OK, if not, data are not OK */
		if (receivedCRC != calculatedCRC) {
			return 0;
		}
		
		/* Wait for STOP byte */
		timeout = RS485_TIMEOUT;
		while (TM_USART_BufferEmpty(RS485_USART)) {
			if (timeout-- == 0x00) {
				RS485_CLEAR_BUFFER;
				return 0;
			}
		}
		/* Read stop byte */
		c = TM_USART_Getc(RS485_USART);
		if (c != RS485_STOP_BYTE) {
			/* Clear buffer */
			RS485_CLEAR_BUFFER;
			return 0;
		}
		
		/* Data valid */
		return 1;
	}

	/* Return 0, data not valid */
	return 0;
}

/* Private functions */
void TM_RS485_INT_InitPins(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Enable RX/TX pin clock */
	RCC_AHB1PeriphClockCmd(RS485_DIR_RCC, ENABLE);
	
	/* Set pin settings */
	GPIO_InitStruct.GPIO_Pin = RS485_DIR_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	/* Initialize GPIO */
	GPIO_Init(RS485_DIR_PORT, &GPIO_InitStruct);
	
	/* Go to RX mode */
	RS485_ENABLE_RX;
}

