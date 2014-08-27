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

void TM_RS485_Init(uint8_t id, uint32_t baudrate) {
	RS485_Settings.id = id;
	
	TM_RS485_InitPins();
	TM_USART_Init(RS485_USART, RS485_USART_PINSPACK, baudrate);
}

void TM_RS485_InitPins(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RS485_RXTX_ENABLE_RCC, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = RS485_RXTX_ENABLE_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	GPIO_Init(RS485_RXTX_ENABLE_PORT, &GPIO_InitStruct);
	
	RS485_ENABLE_RX;
}

uint8_t TM_RS485_Send(uint8_t receiver, uint8_t* data, uint8_t count) {
	//start + sender + receiver + count + allbytes + crcMSB + crcLSB + end
	uint8_t y, tmp[RS485_BUFFER_LENGTH], i = 0;
	uint16_t calculatedCRC;
	volatile uint32_t delay;
	if (count > (RS485_BUFFER_LENGTH - 7)) {
		count = RS485_BUFFER_LENGTH - 7;
	}

	//Enable transmitter
	RS485_ENABLE_TX;
	
	for (delay = 0; delay < RS485_DELAY; delay++) {}
	
	tmp[i++] = RS485_START_BYTE;					//Send start byte
	tmp[i++] = RS485_Settings.id;					//Send sender ID
	tmp[i++] = receiver;							//Send receiver id
	tmp[i++] = count;								//Send number of data
	for (y = 0; y < count; y++) {					//Send data
		tmp[i++] = *(data+y);
	}
	
	calculatedCRC = TM_SOFTCRC_Generate(tmp, count + 4);
	
	tmp[i++] = (calculatedCRC >> 8) & 0xFF;			//Send CRC MSB
	tmp[i++] = calculatedCRC & 0xFF;				//Send CRC LSB
	tmp[i++] = RS485_STOP_BYTE;						//Send stop byte
	
	for (y = 0; y < i; y++) {
		TM_USART_Putc(RS485_USART, tmp[y]);
	}
	
	//Wait until transmitter is empty
	while (USART_GetFlagStatus(RS485_USART, USART_FLAG_TXE) == RESET);
	for (delay = 0; delay < RS485_DELAY; delay++) {}
		
	RS485_ENABLE_RX;
		
	//Data sent successfully
	return 1;
}

uint8_t TM_RS485_Receive(uint8_t* from, uint8_t* data, uint8_t* count) {
	uint8_t c, toID, bytesCount, i, crcMSB, crcLSB, crcData[RS485_BUFFER_LENGTH];
	uint16_t calculatedCRC, receivedCRC;
	
	RS485_ENABLE_RX;
	
	//If any data received
	if (!TM_USART_BufferEmpty(RS485_USART)) {
		//Wait till byte is start byte
		while (!TM_USART_BufferEmpty(RS485_USART)) {
			c = TM_USART_Getc(RS485_USART);
			if (c == RS485_START_BYTE) {
				break;
			}
		}
		//If data is start byte
		//c = TM_USART_Getc(RS485_USART);
		if (c == RS485_START_BYTE) {
			crcData[0] = c;
			//Wait for another byte
			while (TM_USART_BufferEmpty(RS485_USART));
			*from = TM_USART_Getc(RS485_USART); //from address
			crcData[1] = *from;
			
			//Wait another one
			while (TM_USART_BufferEmpty(RS485_USART));
			toID = TM_USART_Getc(RS485_USART); //receiver address (me)
			crcData[2] = toID;
			
			//If message is for us
			if (toID == RS485_Settings.id) {
				//Read next
				while (TM_USART_BufferEmpty(RS485_USART));
				bytesCount = TM_USART_Getc(RS485_USART);
				*count = bytesCount;
				crcData[3] = bytesCount;
				
				//Now read all bytes from count
				for (i = 0; i < bytesCount; i++) {
					//Wait for byte
					while (TM_USART_BufferEmpty(RS485_USART));
					*(data + i) = TM_USART_Getc(RS485_USART);
					crcData[4 + i] = *(data + i);
				}
				//Read CRC MSB
				while (TM_USART_BufferEmpty(RS485_USART));
				crcMSB = TM_USART_Getc(RS485_USART);
				//Read CRC LSB
				while (TM_USART_BufferEmpty(RS485_USART));
				crcLSB = TM_USART_Getc(RS485_USART);
				
				receivedCRC = (crcMSB << 8) | crcLSB;
				calculatedCRC = TM_SOFTCRC_Generate(crcData, bytesCount + 4); //First 4 control bytes
				if (receivedCRC != calculatedCRC) {
					//Clear buffer
					RS485_CLEAR_BUFFER;
					return 0;
				}
				
				while (TM_USART_BufferEmpty(RS485_USART));
				c = TM_USART_Getc(RS485_USART);
				if (c != RS485_STOP_BYTE) {
					//Clear buffer
					RS485_CLEAR_BUFFER;
					return 0;
				}
				
				//Clear buffer
				RS485_CLEAR_BUFFER;
				return 1;
			} else {
				//Clear buffer
				RS485_CLEAR_BUFFER;
				return 0;
			}
		} else {
			//Clear buffer
			RS485_CLEAR_BUFFER;
			return 0;
		}
	}
	
	//Return true if data is valid or false if not
	return 0;
}
/*
#ifdef RS485_USART_IRQ_USART1
void TM_USART1_ReceiveHandler(uint8_t c)
#elif defined RS485_USART_IRQ_USART2
void TM_USART2_ReceiveHandler(uint8_t c)
#elif defined RS485_USART_IRQ_USART3
void TM_USART3_ReceiveHandler(uint8_t c)
#elif defined RS485_USART_IRQ_UART4
void TM_UART4_ReceiveHandler(uint8_t c)
#elif defined RS485_USART_IRQ_UART5
void TM_UART5_ReceiveHandler(uint8_t c)
#elif defined RS485_USART_IRQ_USART6
void TM_USART6_ReceiveHandler(uint8_t c)
#elif defined RS485_USART_IRQ_UART7
void TM_UART7_ReceiveHandler(uint8_t c)
#elif defined RS485_USART_IRQ_UART8
void TM_UART8_ReceiveHandler(uint8_t c)
#else
	#error "Please define RS485_USART_IRQ_USARTx for RS485 communication"
#endif
{
	
}
*/

