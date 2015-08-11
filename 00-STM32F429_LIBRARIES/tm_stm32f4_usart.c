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
#include "tm_stm32f4_usart.h"

/**
 * @brief Internal USART struct
 */
typedef struct {
	uint8_t *Buffer;
	uint16_t Size;
	uint16_t Num;
	uint16_t In;
	uint16_t Out;
	uint8_t Initialized;
	uint8_t StringDelimiter;
} TM_USART_t;

/* Set variables for buffers */
#ifdef USE_USART1
uint8_t TM_USART1_Buffer[TM_USART1_BUFFER_SIZE];
#endif
#ifdef USE_USART2
uint8_t TM_USART2_Buffer[TM_USART2_BUFFER_SIZE];
#endif
#ifdef USE_USART3
uint8_t TM_USART3_Buffer[TM_USART3_BUFFER_SIZE];
#endif
#ifdef USE_UART4
uint8_t TM_UART4_Buffer[TM_UART4_BUFFER_SIZE];
#endif
#ifdef USE_UART5
uint8_t TM_UART5_Buffer[TM_UART5_BUFFER_SIZE];
#endif
#ifdef USE_USART6
uint8_t TM_USART6_Buffer[TM_USART6_BUFFER_SIZE];
#endif
#ifdef USE_UART7
uint8_t TM_UART7_Buffer[TM_UART7_BUFFER_SIZE];
#endif
#ifdef USE_UART8
uint8_t TM_UART8_Buffer[TM_UART8_BUFFER_SIZE];
#endif

#ifdef USE_USART1
TM_USART_t TM_USART1 = {TM_USART1_Buffer, TM_USART1_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif
#ifdef USE_USART2
TM_USART_t TM_USART2 = {TM_USART2_Buffer, TM_USART2_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif
#ifdef USE_USART3
TM_USART_t TM_USART3 = {TM_USART3_Buffer, TM_USART3_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif
#ifdef USE_UART4
TM_USART_t TM_UART4 = {TM_UART4_Buffer, TM_UART4_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif
#ifdef USE_UART5
TM_USART_t TM_UART5 = {TM_UART5_Buffer, TM_UART5_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif
#ifdef USE_USART6
TM_USART_t TM_USART6 = {TM_USART6_Buffer, TM_USART6_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif
#ifdef USE_UART7
TM_USART_t TM_UART7 = {TM_UART7_Buffer, TM_UART7_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif
#ifdef USE_UART8
TM_USART_t TM_UART8 = {TM_UART8_Buffer, TM_UART8_BUFFER_SIZE, 0, 0, 0, 0, USART_STRING_DELIMITER};
#endif

/* Private functions */
void TM_USART1_InitPins(TM_USART_PinsPack_t pinspack);
void TM_USART2_InitPins(TM_USART_PinsPack_t pinspack);
void TM_USART3_InitPins(TM_USART_PinsPack_t pinspack);
void TM_UART4_InitPins(TM_USART_PinsPack_t pinspack);
void TM_UART5_InitPins(TM_USART_PinsPack_t pinspack);
void TM_USART6_InitPins(TM_USART_PinsPack_t pinspack);
void TM_UART7_InitPins(TM_USART_PinsPack_t pinspack);
void TM_UART8_InitPins(TM_USART_PinsPack_t pinspack);
void TM_USART_INT_InsertToBuffer(TM_USART_t* u, uint8_t c);
TM_USART_t* TM_USART_INT_GetUsart(USART_TypeDef* USARTx);
uint8_t TM_USART_INT_GetSubPriority(USART_TypeDef* USARTx);
uint8_t TM_USART_BufferFull(USART_TypeDef* USARTx);

/* Private initializator function */
static void TM_USART_INT_Init(
	USART_TypeDef* USARTx,
	TM_USART_PinsPack_t pinspack,
	uint32_t baudrate,
	TM_USART_HardwareFlowControl_t FlowControl,
	uint32_t Mode,
	uint32_t Parity,
	uint32_t StopBits,
	uint32_t WordLength
);

void TM_USART_Init(USART_TypeDef* USARTx, TM_USART_PinsPack_t pinspack, uint32_t baudrate) {
#ifdef USE_USART1
	if (USARTx == USART1) {
		TM_USART_INT_Init(USART1, pinspack, baudrate, TM_USART1_HARDWARE_FLOW_CONTROL, TM_USART1_MODE, TM_USART1_PARITY, TM_USART1_STOP_BITS, TM_USART1_WORD_LENGTH);
	}
#endif
#ifdef USE_USART2
	if (USARTx == USART2) {
		TM_USART_INT_Init(USART2, pinspack, baudrate, TM_USART2_HARDWARE_FLOW_CONTROL, TM_USART2_MODE, TM_USART2_PARITY, TM_USART2_STOP_BITS, TM_USART2_WORD_LENGTH);
	}
#endif
#ifdef USE_USART3
	if (USARTx == USART3) {
		TM_USART_INT_Init(USART3, pinspack, baudrate, TM_USART3_HARDWARE_FLOW_CONTROL, TM_USART3_MODE, TM_USART3_PARITY, TM_USART3_STOP_BITS, TM_USART3_WORD_LENGTH);
	}
#endif
#ifdef USE_UART4
	if (USARTx == UART4) {
		TM_USART_INT_Init(UART4, pinspack, baudrate, TM_UART4_HARDWARE_FLOW_CONTROL, TM_UART4_MODE, TM_UART4_PARITY, TM_UART4_STOP_BITS, TM_UART4_WORD_LENGTH);
	}
#endif
#ifdef USE_UART5
	if (USARTx == UART5) {
		TM_USART_INT_Init(UART5, pinspack, baudrate, TM_UART5_HARDWARE_FLOW_CONTROL, TM_UART5_MODE, TM_UART5_PARITY, TM_UART5_STOP_BITS, TM_UART5_WORD_LENGTH);
	}
#endif
#ifdef USE_USART6
	if (USARTx == USART6) {
		TM_USART_INT_Init(USART6, pinspack, baudrate, TM_USART6_HARDWARE_FLOW_CONTROL, TM_USART6_MODE, TM_USART6_PARITY, TM_USART6_STOP_BITS, TM_USART6_WORD_LENGTH);
	}
#endif
#ifdef USE_UART7
	if (USARTx == UART7) {
		TM_USART_INT_Init(UART7, pinspack, baudrate, TM_UART7_HARDWARE_FLOW_CONTROL, TM_UART7_MODE, TM_UART7_PARITY, TM_UART7_STOP_BITS, TM_UART7_WORD_LENGTH);
	}
#endif
#ifdef USE_UART8
	if (USARTx == UART8) {
		TM_USART_INT_Init(UART8, pinspack, baudrate, TM_UART8_HARDWARE_FLOW_CONTROL, TM_UART8_MODE, TM_UART8_PARITY, TM_UART8_STOP_BITS, TM_UART8_WORD_LENGTH);
	}
#endif
}

void TM_USART_InitWithFlowControl(USART_TypeDef* USARTx, TM_USART_PinsPack_t pinspack, uint32_t baudrate, TM_USART_HardwareFlowControl_t FlowControl) {
#ifdef USE_USART1
	if (USARTx == USART1) {
		TM_USART_INT_Init(USART1, pinspack, baudrate, FlowControl, TM_USART1_MODE, TM_USART1_PARITY, TM_USART1_STOP_BITS, TM_USART1_WORD_LENGTH);
	}
#endif
#ifdef USE_USART2
	if (USARTx == USART2) {
		TM_USART_INT_Init(USART2, pinspack, baudrate, FlowControl, TM_USART2_MODE, TM_USART2_PARITY, TM_USART2_STOP_BITS, TM_USART2_WORD_LENGTH);
	}
#endif
#ifdef USE_USART3
	if (USARTx == USART3) {
		TM_USART_INT_Init(USART3, pinspack, baudrate, FlowControl, TM_USART3_MODE, TM_USART3_PARITY, TM_USART3_STOP_BITS, TM_USART3_WORD_LENGTH);
	}
#endif
#ifdef USE_UART4
	if (USARTx == UART4) {
		TM_USART_INT_Init(UART4, pinspack, baudrate, FlowControl, TM_UART4_MODE, TM_UART4_PARITY, TM_UART4_STOP_BITS, TM_UART4_WORD_LENGTH);
	}
#endif
#ifdef USE_UART5
	if (USARTx == UART5) {
		TM_USART_INT_Init(UART5, pinspack, baudrate, FlowControl, TM_UART5_MODE, TM_UART5_PARITY, TM_UART5_STOP_BITS, TM_UART5_WORD_LENGTH);
	}
#endif
#ifdef USE_USART6
	if (USARTx == USART6) {
		TM_USART_INT_Init(USART6, pinspack, baudrate, FlowControl, TM_USART6_MODE, TM_USART6_PARITY, TM_USART6_STOP_BITS, TM_USART6_WORD_LENGTH);
	}
#endif
#ifdef USE_UART7
	if (USARTx == UART7) {
		TM_USART_INT_Init(UART7, pinspack, baudrate, FlowControl, TM_UART7_MODE, TM_UART7_PARITY, TM_UART7_STOP_BITS, TM_UART7_WORD_LENGTH);
	}
#endif
#ifdef USE_UART8
	if (USARTx == UART8) {
		TM_USART_INT_Init(UART8, pinspack, baudrate, FlowControl, TM_UART8_MODE, TM_UART8_PARITY, TM_UART8_STOP_BITS, TM_UART8_WORD_LENGTH);
	}
#endif
}

uint8_t TM_USART_Getc(USART_TypeDef* USARTx) {
	int8_t c = 0;
	TM_USART_t* u = TM_USART_INT_GetUsart(USARTx);
	
	/* Check if we have any data in buffer */
	if (u->Num > 0 || u->In != u->Out) {
		/* Check overflow */
		if (u->Out == u->Size) {
			u->Out = 0;
		}
		
		/* Read character */
		c = u->Buffer[u->Out];
		
		/* Increase output pointer */
		u->Out++;
		
		/* Decrease number of elements */
		if (u->Num) {
			u->Num--;
		}
	}
	
	/* Return character */
	return c;
}

uint16_t TM_USART_Gets(USART_TypeDef* USARTx, char* buffer, uint16_t bufsize) {
	uint16_t i = 0;
	
	/* Get USART structure */
	TM_USART_t* u = TM_USART_INT_GetUsart(USARTx);
	
	/* Check for any data on USART */
	if (
		u->Num == 0 ||                                             /*!< Buffer empty */
		(
			!TM_USART_FindCharacter(USARTx, u->StringDelimiter) && /*!< String delimiter not in buffer */
			u->Num != u->Size                                      /*!< Buffer is not full */
		)
	) {
		/* Return 0 */
		return 0;
	}
	
	/* If available buffer size is more than 0 characters */
	while (i < (bufsize - 1)) {
		/* We have available data */
		buffer[i] = (char) TM_USART_Getc(USARTx);
		
		/* Check for end of string */
		if ((uint8_t) buffer[i] == (uint8_t) u->StringDelimiter) {
			/* Done */
			break;
		}
		
		/* Increase */
		i++;
	}
	
	/* Add zero to the end of string */
	buffer[++i] = 0;               

	/* Return number of characters in buffer */
	return i;
}

uint8_t TM_USART_BufferEmpty(USART_TypeDef* USARTx) {
	TM_USART_t* u = TM_USART_INT_GetUsart(USARTx);
	
	/* Check if number of characters is zero in buffer */
	return (u->Num == 0 && u->In == u->Out);
}

uint8_t TM_USART_BufferFull(USART_TypeDef* USARTx) {
	TM_USART_t* u = TM_USART_INT_GetUsart(USARTx);
	
	/* Check if number of characters is the same as buffer size */
	return (u->Num == u->Size);
}

void TM_USART_ClearBuffer(USART_TypeDef* USARTx) {
	TM_USART_t* u = TM_USART_INT_GetUsart(USARTx);
	
	/* Reset variables */
	u->Num = 0;
	u->In = 0;
	u->Out = 0;
}

void TM_USART_SetCustomStringEndCharacter(USART_TypeDef* USARTx, uint8_t Character) {
	/* Get USART structure */
	TM_USART_t* u = TM_USART_INT_GetUsart(USARTx);
	
	/* Set delimiter */
	u->StringDelimiter = Character;
}

uint8_t TM_USART_FindCharacter(USART_TypeDef* USARTx, uint8_t c) {
	uint16_t num, out;
	TM_USART_t* u = TM_USART_INT_GetUsart(USARTx);
	
	/* Temp variables */
	num = u->Num;
	out = u->Out;
	
	while (num > 0) {
		/* Check overflow */
		if (out == u->Size) {
			out = 0;
		}
		
		/* Check if characters matches */
		if ((uint8_t) u->Buffer[out] == (uint8_t) c) {
			/* Character found */
			return 1;
		}
		
		/* Set new variables */
		out++;
		num--;
	}
	
	/* Character is not in buffer */
	return 0;
}

void TM_USART_Puts(USART_TypeDef* USARTx, char* str) {
	TM_USART_t* u = TM_USART_INT_GetUsart(USARTx);
	/* If we are not initialized */
	if (u->Initialized == 0) {
		return;
	}
	
	/* Go through entire string */
	while (*str) {
		/* Wait to be ready, buffer empty */
		USART_WAIT(USARTx);
		/* Send data */
		USARTx->DR = (uint16_t)(*str++ & 0x01FF);
		/* Wait to be ready, buffer empty */
		USART_WAIT(USARTx);
	}
}

void TM_USART_Send(USART_TypeDef* USARTx, uint8_t* DataArray, uint16_t count) {
	uint16_t i;
	TM_USART_t* u = TM_USART_INT_GetUsart(USARTx);
	/* If we are not initialized */
	if (u->Initialized == 0) {
		return;
	}
	
	/* Go through entire data array */
	for (i = 0; i < count; i++) {
		/* Wait to be ready, buffer empty */
		USART_WAIT(USARTx);
		/* Send data */
		USARTx->DR = (uint16_t)(DataArray[i]);
		/* Wait to be ready, buffer empty */
		USART_WAIT(USARTx);
	}
}

/* Private functions */
void TM_USART_INT_InsertToBuffer(TM_USART_t* u, uint8_t c) {
	/* Still available space in buffer */
	if (u->Num < u->Size) {
		/* Check overflow */
		if (u->In == u->Size) {
			u->In = 0;
		}
		
		/* Add to buffer */
		u->Buffer[u->In] = c;
		u->In++;
		u->Num++;
	}
}

__weak void TM_USART_InitCustomPinsCallback(USART_TypeDef* USARTx, uint16_t AlternateFunction) {
	/* Custom user function. */
	/* In case user needs functionality for custom pins, this function should be declared outside this library */
}

TM_USART_t* TM_USART_INT_GetUsart(USART_TypeDef* USARTx) {
	TM_USART_t* u;
	
#ifdef USE_USART1
	if (USARTx == USART1) {
		u = &TM_USART1;
	}
#endif
#ifdef USE_USART2
	if (USARTx == USART2) {
		u = &TM_USART2;
	}
#endif
#ifdef USE_USART3
	if (USARTx == USART3) {
		u = &TM_USART3;
	}
#endif
#ifdef USE_UART4
	if (USARTx == UART4) {
		u = &TM_UART4;
	}
#endif
#ifdef USE_UART5
	if (USARTx == UART5) {
		u = &TM_UART5;
	}
#endif
#ifdef USE_USART6
	if (USARTx == USART6) {
		u = &TM_USART6;
	}
#endif
#ifdef USE_UART7
	if (USARTx == UART7) {
		u = &TM_UART7;
	}
#endif
#ifdef USE_UART8
	if (USARTx == UART8) {
		u = &TM_UART8;
	}
#endif

	return u;
}

uint8_t TM_USART_INT_GetSubPriority(USART_TypeDef* USARTx) {
	uint8_t u;
	
#ifdef USE_USART1
	if (USARTx == USART1) {
		u = 0;
	}
#endif
#ifdef USE_USART2
	if (USARTx == USART2) {
		u = 1;
	}
#endif
#ifdef USE_USART3
	if (USARTx == USART3) {
		u = 2;
	}
#endif
#ifdef USE_UART4
	if (USARTx == UART4) {
		u = 4;
	}
#endif
#ifdef USE_UART5
	if (USARTx == UART5) {
		u = 5;
	}
#endif
#ifdef USE_USART6
	if (USARTx == USART6) {
		u = 6;
	}
#endif
#ifdef USE_UART7
	if (USARTx == UART7) {
		u = 7;
	}
#endif
#ifdef USE_UART8
	if (USARTx == UART8) {
		u = 8;
	}
#endif
	
	return u;
}

#ifdef USE_USART1
void TM_USART1_InitPins(TM_USART_PinsPack_t pinspack) {	
	/* Init pins */
#if defined(GPIOA)
	if (pinspack == TM_USART_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOA, GPIO_Pin_9 | GPIO_Pin_10, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_USART1);
	}
#endif
#if defined(GPIOB)
	if (pinspack == TM_USART_PinsPack_2) {
		TM_GPIO_InitAlternate(GPIOB, GPIO_Pin_6 | GPIO_Pin_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_USART1);
	}
#endif
	if (pinspack == TM_USART_PinsPack_Custom) {
		/* Init custom pins, callback used */
		TM_USART_InitCustomPinsCallback(USART1, GPIO_AF_USART1);
	}
}
#endif

#ifdef USE_USART2
void TM_USART2_InitPins(TM_USART_PinsPack_t pinspack) {
	/* Init pins */
#if defined(GPIOA)
	if (pinspack == TM_USART_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOA, GPIO_Pin_2 | GPIO_Pin_3, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_USART2);
	}
#endif
#if defined(GPIOD)
	if (pinspack == TM_USART_PinsPack_2) {
		TM_GPIO_InitAlternate(GPIOD, GPIO_Pin_5 | GPIO_Pin_6, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_USART2);
	}
#endif
	if (pinspack == TM_USART_PinsPack_Custom) {
		/* Init custom pins, callback used */
		TM_USART_InitCustomPinsCallback(USART2, GPIO_AF_USART2);
	}
}
#endif

#ifdef USE_USART3
void TM_USART3_InitPins(TM_USART_PinsPack_t pinspack) {
	/* Init pins */
#if defined(GPIOB)
	if (pinspack == TM_USART_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOB, GPIO_Pin_10 | GPIO_Pin_11, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_USART3);
	}
#endif
#if defined(GPIOC)
	if (pinspack == TM_USART_PinsPack_2) {
		TM_GPIO_InitAlternate(GPIOC, GPIO_Pin_10 | GPIO_Pin_11, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_USART3);
	}
#endif
#if defined(GPIOD)
	if (pinspack == TM_USART_PinsPack_3) {
		TM_GPIO_InitAlternate(GPIOD, GPIO_Pin_8 | GPIO_Pin_9, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_USART3);
	}
#endif
	if (pinspack == TM_USART_PinsPack_Custom) {
		/* Init custom pins, callback used */
		TM_USART_InitCustomPinsCallback(USART3, GPIO_AF_USART3);
	}
}
#endif

#ifdef USE_UART4
void TM_UART4_InitPins(TM_USART_PinsPack_t pinspack) {
	/* Init pins */
#if defined(GPIOA)
	if (pinspack == TM_USART_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOA, GPIO_Pin_0 | GPIO_Pin_1, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_UART4);
	}
#endif
#if defined(GPIOC)
	if (pinspack == TM_USART_PinsPack_2) {
		TM_GPIO_InitAlternate(GPIOC, GPIO_Pin_10 | GPIO_Pin_11, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_UART4);
	}
#endif
	if (pinspack == TM_USART_PinsPack_Custom) {
		/* Init custom pins, callback used */
		TM_USART_InitCustomPinsCallback(UART4, GPIO_AF_UART4);
	}
}
#endif

#ifdef USE_UART5
void TM_UART5_InitPins(TM_USART_PinsPack_t pinspack) {
	/* Init pins */
#if defined(GPIOC) && defined(GPIOD)
	if (pinspack == TM_USART_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOC, GPIO_Pin_12, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_UART5);
		TM_GPIO_InitAlternate(GPIOD, GPIO_Pin_2, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_UART5);
	}
#endif
	if (pinspack == TM_USART_PinsPack_Custom) {
		/* Init custom pins, callback used */
		TM_USART_InitCustomPinsCallback(UART5, GPIO_AF_UART5);
	}
}
#endif

#ifdef USE_USART6
void TM_USART6_InitPins(TM_USART_PinsPack_t pinspack) {
	/* Init pins */
#if defined(GPIOC)
	if (pinspack == TM_USART_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOC, GPIO_Pin_6 | GPIO_Pin_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_USART6);
	}
#endif
#if defined(GPIOG)
	if (pinspack == TM_USART_PinsPack_2) {
		TM_GPIO_InitAlternate(GPIOG, GPIO_Pin_14 | GPIO_Pin_9, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_USART6);
	}
#endif
	if (pinspack == TM_USART_PinsPack_Custom) {
		/* Init custom pins, callback used */
		TM_USART_InitCustomPinsCallback(USART6, GPIO_AF_USART6);
	}
}
#endif

#ifdef USE_UART7
void TM_UART7_InitPins(TM_USART_PinsPack_t pinspack) {
	/* Init pins */
#if defined(GPIOE)
	if (pinspack == TM_USART_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOE, GPIO_Pin_8 | GPIO_Pin_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_UART7);
	}
#endif
#if defined(GPIOF)
	if (pinspack == TM_USART_PinsPack_2) {
		TM_GPIO_InitAlternate(GPIOF, GPIO_Pin_7 | GPIO_Pin_6, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_UART7);
	}
#endif
	if (pinspack == TM_USART_PinsPack_Custom) {
		/* Init custom pins, callback used */
		TM_USART_InitCustomPinsCallback(UART7, GPIO_AF_UART7);
	}
}
#endif

#ifdef USE_UART8
void TM_UART8_InitPins(TM_USART_PinsPack_t pinspack) {
	/* Init pins */
#if defined(GPIOE)
	if (pinspack == TM_USART_PinsPack_1) {
		TM_GPIO_InitAlternate(GPIOE, GPIO_Pin_1 | GPIO_Pin_0, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High, GPIO_AF_UART8);
	}
#endif
	if (pinspack == TM_USART_PinsPack_Custom) {
		/* Init custom pins, callback used */
		TM_USART_InitCustomPinsCallback(UART8, GPIO_AF_UART8);
	}
}
#endif

#ifdef USE_USART1
void USART1_IRQHandler(void) {
	/* Check if interrupt was because data is received */
	if (USART1->SR & USART_SR_RXNE) {
		#ifdef TM_USART1_USE_CUSTOM_IRQ
			/* Call user function */
			TM_USART1_ReceiveHandler(USART1->DR);
		#else
			/* Put received data into internal buffer */
			TM_USART_INT_InsertToBuffer(&TM_USART1, USART1->DR);
		#endif
	}
}
#endif

#ifdef USE_USART2
void USART2_IRQHandler(void) {
	/* Check if interrupt was because data is received */
	if (USART2->SR & USART_SR_RXNE) {
		#ifdef TM_USART2_USE_CUSTOM_IRQ
			/* Call user function */
			TM_USART2_ReceiveHandler(USART2->DR);
		#else 
			/* Put received data into internal buffer */
			TM_USART_INT_InsertToBuffer(&TM_USART2, USART2->DR);
		#endif
	}
}
#endif

#ifdef USE_USART3
void USART3_IRQHandler(void) {
	/* Check if interrupt was because data is received */
	if (USART3->SR & USART_SR_RXNE) {
		#ifdef TM_USART3_USE_CUSTOM_IRQ
			/* Call user function */
			TM_USART3_ReceiveHandler(USART3->DR);
		#else
			/* Put received data into internal buffer */
			TM_USART_INT_InsertToBuffer(&TM_USART3, USART3->DR);
		#endif
	}
}
#endif

#ifdef USE_UART4
void UART4_IRQHandler(void) {
	/* Check if interrupt was because data is received */
	if (UART4->SR & USART_SR_RXNE) {
		#ifdef TM_UART4_USE_CUSTOM_IRQ
			/* Call user function */
			TM_UART4_ReceiveHandler(UART4->DR);
		#else
			/* Put received data into internal buffer */
			TM_USART_INT_InsertToBuffer(&TM_UART4, UART4->DR);
		#endif
	}
}
#endif

#ifdef USE_UART5
void UART5_IRQHandler(void) {
	/* Check if interrupt was because data is received */
	if (UART5->SR & USART_SR_RXNE) {
		#ifdef TM_UART5_USE_CUSTOM_IRQ
			/* Call user function */
			TM_UART5_ReceiveHandler(UART5->DR);
		#else
			/* Put received data into internal buffer */
			TM_USART_INT_InsertToBuffer(&TM_UART5, UART5->DR);
		#endif
	}
}
#endif

#ifdef USE_USART6
void USART6_IRQHandler(void) {
	/* Check if interrupt was because data is received */
	if (USART6->SR & USART_SR_RXNE) {
		#ifdef TM_USART6_USE_CUSTOM_IRQ
			/* Call user function */
			TM_USART6_ReceiveHandler(USART6->DR);
		#else
			/* Put received data into internal buffer */
			TM_USART_INT_InsertToBuffer(&TM_USART6, USART6->DR);
		#endif
	}
}
#endif

#ifdef USE_UART7
void UART7_IRQHandler(void) {
	/* Check if interrupt was because data is received */
	if (UART7->SR & USART_SR_RXNE) {
		#ifdef TM_UART7_USE_CUSTOM_IRQ
			/* Call user function */
			TM_UART7_ReceiveHandler(UART7->DR);
		#else
			/* Put received data into internal buffer */
			TM_USART_INT_InsertToBuffer(&TM_UART7, UART7->DR);
		#endif
	}
}
#endif

#ifdef USE_UART8
void UART8_IRQHandler(void) {
	/* Check if interrupt was because data is received */
	if (UART8->SR & USART_SR_RXNE) {
		#ifdef TM_UART8_USE_CUSTOM_IRQ
			/* Call user function */
			TM_UART8_ReceiveHandler(UART8->DR);
		#else
			/* Put received data into internal buffer */
			TM_USART_INT_InsertToBuffer(&TM_UART8, UART8->DR);
		#endif
	}
}
#endif

static void TM_USART_INT_Init(
	USART_TypeDef* USARTx,
	TM_USART_PinsPack_t pinspack,
	uint32_t baudrate,
	TM_USART_HardwareFlowControl_t FlowControl,
	uint32_t Mode,
	uint32_t Parity,
	uint32_t StopBits,
	uint32_t WordLength
) {
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	TM_USART_t* u = TM_USART_INT_GetUsart(USARTx);

	/* Set USART baudrate */
	USART_InitStruct.USART_BaudRate = baudrate;
	
	/*
	 * Initialize USARTx pins
	 * Set channel for USARTx NVIC
	 */
#ifdef USE_USART1
	if (USARTx == USART1) {
		/* Enable USART clock */
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
		
		/* Init pins */
		TM_USART1_InitPins(pinspack);
		
		/* Set IRQ channel */
		NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	}
#endif
#ifdef USE_USART2
	if (USARTx == USART2) {
		/* Enable USART clock */
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
		
		/* Init pins */
		TM_USART2_InitPins(pinspack);
		
		/* Set IRQ channel */
		NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	}
#endif
#ifdef USE_USART3
	if (USARTx == USART3) {
		/* Enable USART clock */
		RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
		
		/* Init pins */
		TM_USART3_InitPins(pinspack);
		
		/* Set IRQ channel */
		NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	}
#endif
#ifdef USE_UART4
	if (USARTx == UART4) {
		/* Enable UART clock */
		RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
		
		/* Init pins */
		TM_UART4_InitPins(pinspack);
		
		/* Set IRQ channel */
		NVIC_InitStruct.NVIC_IRQChannel = UART4_IRQn;
	}
#endif
#ifdef USE_UART5
	if (USARTx == UART5) {
		/* Enable UART clock */
		RCC->APB1ENR |= RCC_APB1ENR_UART5EN;

		/* Init pins */
		TM_UART5_InitPins(pinspack);
		
		/* Set IRQ channel */
		NVIC_InitStruct.NVIC_IRQChannel = UART5_IRQn;
	}
#endif
#ifdef USE_USART6
	if (USARTx == USART6) {
		/* Enable UART clock */
		RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
		
		/* Init pins */
		TM_USART6_InitPins(pinspack);
		
		/* Set IRQ channel */
		NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;
	}
#endif
#ifdef USE_UART7
	if (USARTx == UART7) {
		/* Enable UART clock */
		RCC->APB1ENR |= RCC_APB1ENR_UART7EN;
		
		/* Init pins */
		TM_UART7_InitPins(pinspack);
		
		/* Set IRQ channel */
		NVIC_InitStruct.NVIC_IRQChannel = UART7_IRQn;
	}
#endif
#ifdef USE_UART8
	if (USARTx == UART8) {
		/* Enable UART clock */
		RCC->APB1ENR |= RCC_APB1ENR_UART8EN;

		/* Init pins */
		TM_UART8_InitPins(pinspack);
		
		/* Set IRQ channel */
		NVIC_InitStruct.NVIC_IRQChannel = UART8_IRQn;
	}
#endif
	
	/* Deinit USART, force reset */
	USART_DeInit(USARTx);
	
	/* Fill NVIC settings */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = USART_NVIC_PRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = TM_USART_INT_GetSubPriority(USARTx);
	NVIC_Init(&NVIC_InitStruct);
	
	/* Fill default settings */
	USART_InitStruct.USART_HardwareFlowControl = FlowControl;
	USART_InitStruct.USART_Mode = Mode;
	USART_InitStruct.USART_Parity = Parity;
	USART_InitStruct.USART_StopBits = StopBits;
	USART_InitStruct.USART_WordLength = WordLength;
	
	/* We are not initialized */
	u->Initialized = 0;
	
	do {
		volatile uint32_t x = 0xFFF;
		while (x--);
	} while (0);
	
	/* Init */
	USART_Init(USARTx, &USART_InitStruct);
	
	/* Enable RX interrupt */
	USARTx->CR1 |= USART_CR1_RXNEIE;
	
	/* We are initialized now */
	u->Initialized = 1;
	
	/* Enable USART peripheral */
	USARTx->CR1 |= USART_CR1_UE;
}

