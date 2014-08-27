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
 * Variables
 *
 */
int8_t TM_USART_Buffer[8][TM_USART_BUFFER_SIZE];
uint16_t tm_usart_buf_in[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint16_t tm_usart_buf_out[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint16_t tm_usart_buf_num[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void TM_USART_Init(USART_TypeDef* USARTx, TM_USART_PinsPack_t pinspack, uint32_t baudrate) {
	/**
	 * Initialization structures declared
	 *
	 * USART: Universal Synchronous/Asynchronous Receiver/Transmitter
	 * NVIC: Nested Vector Interrupt Controller
	 */
	USART_InitTypeDef 	USART_InitStruct;
	NVIC_InitTypeDef	NVIC_InitStruct;

	/**
	 * Default settings:
	 * 
	 * Set Baudrate to value you pass to function
	 * Disable Hardware Flow control
	 * Set Mode To TX and RX, so USART will work in full-duplex mode
	 * Disable parity bit
	 * Set 1 stop bit
	 * Set Data bits to 8
	 */
	USART_InitStruct.USART_BaudRate = baudrate;
	
	/*
	 * Initialize USARTx pins
	 * Set channel for USARTx NVIC
	 */
#ifdef TM_USE_USART1
	if (USARTx == USART1) {
		TM_USART1_InitPins(pinspack);
		NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
		
		USART_InitStruct.USART_HardwareFlowControl = TM_USART1_HARDWARE_FLOW_CONTROL;
		USART_InitStruct.USART_Mode = TM_USART1_MODE;
		USART_InitStruct.USART_Parity = TM_USART1_PARITY;
		USART_InitStruct.USART_StopBits = TM_USART1_STOP_BITS;
		USART_InitStruct.USART_WordLength = TM_USART1_WORD_LENGTH;
	}
#endif
#ifdef TM_USE_USART2
	if (USARTx == USART2) {
		TM_USART2_InitPins(pinspack);
		NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
		
		USART_InitStruct.USART_HardwareFlowControl = TM_USART2_HARDWARE_FLOW_CONTROL;
		USART_InitStruct.USART_Mode = TM_USART2_MODE;
		USART_InitStruct.USART_Parity = TM_USART2_PARITY;
		USART_InitStruct.USART_StopBits = TM_USART2_STOP_BITS;
		USART_InitStruct.USART_WordLength = TM_USART2_WORD_LENGTH;
	}
#endif
#ifdef TM_USE_USART3
	if (USARTx == USART3) {
		TM_USART3_InitPins(pinspack);
		NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
		
		USART_InitStruct.USART_HardwareFlowControl = TM_USART3_HARDWARE_FLOW_CONTROL;
		USART_InitStruct.USART_Mode = TM_USART3_MODE;
		USART_InitStruct.USART_Parity = TM_USART3_PARITY;
		USART_InitStruct.USART_StopBits = TM_USART3_STOP_BITS;
		USART_InitStruct.USART_WordLength = TM_USART3_WORD_LENGTH;
	}
#endif
#ifdef TM_USE_UART4
	if (USARTx == UART4) {
		TM_UART4_InitPins(pinspack);
		NVIC_InitStruct.NVIC_IRQChannel = UART4_IRQn;
		
		USART_InitStruct.USART_HardwareFlowControl = TM_UART4_HARDWARE_FLOW_CONTROL;
		USART_InitStruct.USART_Mode = TM_UART4_MODE;
		USART_InitStruct.USART_Parity = TM_UART4_PARITY;
		USART_InitStruct.USART_StopBits = TM_UART4_STOP_BITS;
		USART_InitStruct.USART_WordLength = TM_UART4_WORD_LENGTH;
	}
#endif
#ifdef TM_USE_UART5
	if (USARTx == UART5) {
		TM_UART5_InitPins(pinspack);
		NVIC_InitStruct.NVIC_IRQChannel = UART5_IRQn;
		
		USART_InitStruct.USART_HardwareFlowControl = TM_UART5_HARDWARE_FLOW_CONTROL;
		USART_InitStruct.USART_Mode = TM_UART5_MODE;
		USART_InitStruct.USART_Parity = TM_UART5_PARITY;
		USART_InitStruct.USART_StopBits = TM_UART5_STOP_BITS;
		USART_InitStruct.USART_WordLength = TM_UART5_WORD_LENGTH;
	}
#endif
#ifdef TM_USE_USART6
	if (USARTx == USART6) {
		TM_USART6_InitPins(pinspack);
		NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;
		
		USART_InitStruct.USART_HardwareFlowControl = TM_USART6_HARDWARE_FLOW_CONTROL;
		USART_InitStruct.USART_Mode = TM_USART6_MODE;
		USART_InitStruct.USART_Parity = TM_USART6_PARITY;
		USART_InitStruct.USART_StopBits = TM_USART6_STOP_BITS;
		USART_InitStruct.USART_WordLength = TM_USART6_WORD_LENGTH;
	}
#endif
#ifdef TM_USE_UART7
	if (USARTx == UART7) {
		TM_UART7_InitPins(pinspack);
		NVIC_InitStruct.NVIC_IRQChannel = UART7_IRQn;
		
		USART_InitStruct.USART_HardwareFlowControl = TM_UART7_HARDWARE_FLOW_CONTROL;
		USART_InitStruct.USART_Mode = TM_UART7_MODE;
		USART_InitStruct.USART_Parity = TM_UART7_PARITY;
		USART_InitStruct.USART_StopBits = TM_UART7_STOP_BITS;
		USART_InitStruct.USART_WordLength = TM_UART7_WORD_LENGTH;
	}
#endif
#ifdef TM_USE_UART8
	if (USARTx == UART8) {
		TM_UART8_InitPins(pinspack);
		NVIC_InitStruct.NVIC_IRQChannel = UART8_IRQn;
		
		USART_InitStruct.USART_HardwareFlowControl = TM_UART8_HARDWARE_FLOW_CONTROL;
		USART_InitStruct.USART_Mode = TM_UART8_MODE;
		USART_InitStruct.USART_Parity = TM_UART8_PARITY;
		USART_InitStruct.USART_StopBits = TM_UART8_STOP_BITS;
		USART_InitStruct.USART_WordLength = TM_UART8_WORD_LENGTH;
	}
#endif

	USART_Init(USARTx, &USART_InitStruct);
	USART_Cmd(USARTx, ENABLE);
	
	/**
	 * Enable RX interrupt
	 */
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

	/**
	 * Set Channel Cmd to enable. That will enable USARTx channel in NVIC
	 * Set NVIC_IRQChannelPreemptionPriority. This means high priority for USARTs
	 * USARTx with lower x has high subpriority
	 *
	 * Initialize NVIC
	 */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = TM_USART_NVIC_PRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = TM_USART_GetUsartNumber(USARTx);
	NVIC_Init(&NVIC_InitStruct);
}

#ifdef TM_USE_USART1
void TM_USART1_InitPins(TM_USART_PinsPack_t pinspack) {
	//GPIO Structure
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//Enable clock for USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	if (pinspack == TM_USART_PinsPack_1) {
		// Enable clock for GPIOA
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
		//                         TX           RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;

		//Alternating function
		GPIO_Init(GPIOA, &GPIO_InitStruct);
	} else if (pinspack == TM_USART_PinsPack_2) {
		// Enable clock for GPIOB
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
		//                         TX           RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;

		//Alternating function
		GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}
#endif

#ifdef TM_USE_USART2
void TM_USART2_InitPins(TM_USART_PinsPack_t pinspack) {
	//GPIO Structure
	GPIO_InitTypeDef GPIO_InitStruct;
	//Enable clock for USART2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	if (pinspack == TM_USART_PinsPack_1) {
		// Enable clock for GPIOA
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
		//                         TX           RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;

		//Alternating function
		GPIO_Init(GPIOA, &GPIO_InitStruct);
	} else if (pinspack == TM_USART_PinsPack_2) {
		// Enable clock for GPIOD
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
		//                         TX           RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;

		//Alternating function
		GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
}
#endif

#ifdef TM_USE_USART3
void TM_USART3_InitPins(TM_USART_PinsPack_t pinspack) {
	//GPIO Structure
	GPIO_InitTypeDef GPIO_InitStruct;
	//Enable clock for USART3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	if (pinspack == TM_USART_PinsPack_1) {
		// Enable clock for GPIOB
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
		//                         TX           RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;

		//Alternating function
		GPIO_Init(GPIOB, &GPIO_InitStruct);
	} else if (pinspack == TM_USART_PinsPack_2) {
		// Enable clock for GPIOC
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

		GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
		//                         TX            RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;

		//Alternating function
		GPIO_Init(GPIOC, &GPIO_InitStruct);
	} else if (pinspack == TM_USART_PinsPack_3) {
		// Enable clock for GPIOD
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
		//                         TX           RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;

		//Alternating function
		GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
}
#endif

#ifdef TM_USE_UART4
void TM_UART4_InitPins(TM_USART_PinsPack_t pinspack) {
	//GPIO Structure
	GPIO_InitTypeDef GPIO_InitStruct;
	//Enable clock for UART4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	if (pinspack == TM_USART_PinsPack_1) {
		// Enable clock for GPIOA
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);
		//                         TX           RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;

		//Alternating function
		GPIO_Init(GPIOA, &GPIO_InitStruct);
	} else if (pinspack == TM_USART_PinsPack_2) {
		// Enable clock for GPIOC
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

		GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
		//                         TX            RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;

		//Alternating function
		GPIO_Init(GPIOC, &GPIO_InitStruct);
	}
}
#endif

#ifdef TM_USE_UART5
void TM_UART5_InitPins(TM_USART_PinsPack_t pinspack) {
	//GPIO Structure
	GPIO_InitTypeDef GPIO_InitStruct;
	//Enable clock for UART5
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	if (pinspack == TM_USART_PinsPack_1) {
		// Enable clock for GPIOC and GPIOD
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		//Alternating function
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
		//                         TX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
		GPIO_Init(GPIOC, &GPIO_InitStruct);
		//                         RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
		GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
}
#endif

#ifdef TM_USE_USART6
void TM_USART6_InitPins(TM_USART_PinsPack_t pinspack) {
	//GPIO Structure
	GPIO_InitTypeDef GPIO_InitStruct;
	//Enable clock for USART6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	if (pinspack == TM_USART_PinsPack_1) {
		// Enable clock for GPIOC
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

		GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
		//                         TX           RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;

		//Alternating function
		GPIO_Init(GPIOC, &GPIO_InitStruct);
	} else if (pinspack == TM_USART_PinsPack_2) {
		// Enable clock for GPIOG
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

		GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6);
		GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);
		//                         TX            RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_9;

		//Alternating function
		GPIO_Init(GPIOG, &GPIO_InitStruct);
	}
}
#endif

#ifdef TM_USE_UART7
void TM_UART7_InitPins(TM_USART_PinsPack_t pinspack) {
	//GPIO Structure
	GPIO_InitTypeDef GPIO_InitStruct;
	//Enable clock for UART7
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	if (pinspack == TM_USART_PinsPack_1) {
		// Enable clock for GPIOE
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

		GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_UART7);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_UART7);
		//                         TX           RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_7;

		//Alternating function
		GPIO_Init(GPIOE, &GPIO_InitStruct);
	} else if (pinspack == TM_USART_PinsPack_2) {
		// Enable clock for GPIOF
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

		GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_UART7);
		GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_UART7);
		//                         TX           RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;

		//Alternating function
		GPIO_Init(GPIOF, &GPIO_InitStruct);
	}
}
#endif

#ifdef TM_USE_UART8
void TM_UART8_InitPins(TM_USART_PinsPack_t pinspack) {
	//GPIO Structure
	GPIO_InitTypeDef GPIO_InitStruct;
	//Enable clock for UART8
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	if (pinspack == TM_USART_PinsPack_1) {
		// Enable clock for GPIOE
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

		GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_UART8);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_UART8);
		//                         TX           RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;

		//Alternating function
		GPIO_Init(GPIOE, &GPIO_InitStruct);
	}
}
#endif

void TM_USART_Putc(USART_TypeDef* USARTx, volatile char c) {
	//Wait until transmitter is empty
	while (!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));
	//Send data
	USART_SendData(USARTx, c);
}

void TM_USART_Puts(USART_TypeDef* USARTx, char* str) {
	while (*str) {
		TM_USART_Putc(USARTx, *str++);
	}
}

#ifdef USART1
void USART1_IRQHandler(void) {
	//Check if interrupt was because data is received
	if (USART_GetITStatus(USART1, USART_IT_RXNE)) {
		#ifdef TM_USART1_USE_CUSTOM_IRQ
			//Call user function
			TM_USART1_ReceiveHandler(USART1->DR);
		#else
			//Put received data into internal buffer
			TM_USART_InsertToBuffer(0, USART1->DR);
		#endif
	}
}
#endif

#ifdef USART2
void USART2_IRQHandler(void) {
	//Check if interrupt was because data is received
	if (USART_GetITStatus(USART2, USART_IT_RXNE)) {
		#ifdef TM_USART2_USE_CUSTOM_IRQ
			//Call user function
			TM_USART2_ReceiveHandler(USART2->DR);
		#else 
			//Put received data into internal buffer
			TM_USART_InsertToBuffer(1, USART2->DR);
		#endif
	}
}
#endif

#ifdef USART3
void USART3_IRQHandler(void) {
	//Check if interrupt was because data is received
	if (USART_GetITStatus(USART3, USART_IT_RXNE)) {
		#ifdef TM_USART3_USE_CUSTOM_IRQ
			//Call user function
			TM_USART3_ReceiveHandler(USART3->DR);
		#else
			//Put received data into internal buffer
			TM_USART_InsertToBuffer(2, USART3->DR);
		#endif
	}
}
#endif

#ifdef UART4
void UART4_IRQHandler(void) {
	//Check if interrupt was because data is received
	if (USART_GetITStatus(UART4, USART_IT_RXNE)) {
		#ifdef TM_UART4_USE_CUSTOM_IRQ
			//Call user function
			TM_UART4_ReceiveHandler(UART4->DR);
		#else
			//Put received data into internal buffer
			TM_USART_InsertToBuffer(3, UART4->DR);
		#endif
	}
}
#endif

#ifdef UART5
void UART5_IRQHandler(void) {
	//Check if interrupt was because data is received
	if (USART_GetITStatus(UART5, USART_IT_RXNE)) {
		#ifdef TM_UART5_USE_CUSTOM_IRQ
			//Call user function
			TM_UART5_ReceiveHandler(UART5->DR);
		#else
			//Put received data into internal buffer
			TM_USART_InsertToBuffer(4, UART5->DR);
		#endif
	}
}
#endif

#ifdef USART6
void USART6_IRQHandler(void) {
	//Check if interrupt was because data is received
	if (USART_GetITStatus(USART6, USART_IT_RXNE)) {
		#ifdef TM_USART6_USE_CUSTOM_IRQ
			//Call user function
			TM_USART6_ReceiveHandler(USART6->DR);
		#else
			//Put received data into internal buffer
			TM_USART_InsertToBuffer(5, USART6->DR);
		#endif
	}
}
#endif

#ifdef UART7
void UART7_IRQHandler(void) {
	//Check if interrupt was because data is received
	if (USART_GetITStatus(UART7, USART_IT_RXNE)) {
		#ifdef TM_UART7_USE_CUSTOM_IRQ
			//Call user function
			TM_UART7_ReceiveHandler(UART7->DR);
		#else
			//Put received data into internal buffer
			TM_USART_InsertToBuffer(6, UART7->DR);
		#endif
	}
}
#endif

#ifdef UART8
void UART8_IRQHandler(void) {
	//Check if interrupt was because data is received
	if (USART_GetITStatus(UART8, USART_IT_RXNE)) {
		#ifdef TM_UART8_USE_CUSTOM_IRQ
			//Call user function
			TM_UART8_ReceiveHandler(UART8->DR);
		#else
			//Put received data into internal buffer
			TM_USART_InsertToBuffer(7, UART8->DR);
		#endif
	}
}
#endif

void TM_USART_InsertToBuffer(uint8_t usart_num, uint8_t c) {
	//uint8_t usart_num = TM_USART_GetUsartNumber(USARTx);
	//Still available space in buffer
	if (tm_usart_buf_num[usart_num] < TM_USART_BUFFER_SIZE) {
		if (tm_usart_buf_in[usart_num] > (TM_USART_BUFFER_SIZE - 1)) {
			tm_usart_buf_in[usart_num] = 0;
		}
		TM_USART_Buffer[usart_num][tm_usart_buf_in[usart_num]] = c;
		tm_usart_buf_in[usart_num]++;
		tm_usart_buf_num[usart_num]++;
	}
}

uint8_t TM_USART_Getc(USART_TypeDef* USARTx) {
	uint8_t usart_num = TM_USART_GetUsartNumber(USARTx);
	uint8_t c = 0;
	//Check if we have any data in buffer
	if (tm_usart_buf_num[usart_num] > 0) {
		if (tm_usart_buf_out[usart_num] > (TM_USART_BUFFER_SIZE - 1)) {
			tm_usart_buf_out[usart_num] = 0;
		}
		c = TM_USART_Buffer[usart_num][tm_usart_buf_out[usart_num]];
		TM_USART_Buffer[usart_num][tm_usart_buf_out[usart_num]] = 0;
		tm_usart_buf_out[usart_num]++;
		tm_usart_buf_num[usart_num]--;
	}
	return c;
}

uint16_t TM_USART_Gets(USART_TypeDef* USARTx, char* buffer, uint16_t bufsize) {
	uint16_t i = 0;                             
	uint8_t eol = 0;
	if (TM_USART_BufferEmpty(USARTx)) {
		return 0;
	}
	if (bufsize > 0) {
		while (!eol) {
			while (TM_USART_BufferEmpty(USARTx));
			buffer[i] = (char) TM_USART_Getc(USARTx);   
			if (buffer[i] == '\n') {
				eol = 1;                
			} else {            
				if (i < (bufsize - 1)) {
					i++; 	
				}
			}
		}
		//Add zero to the end of string
		buffer[i] = 0;               
	}

	return (i);
}

uint8_t TM_USART_BufferEmpty(USART_TypeDef* USARTx) {
	uint8_t usart_num = TM_USART_GetUsartNumber(USARTx);
	return !(tm_usart_buf_num[usart_num] > 0);
}

uint8_t TM_USART_GetUsartNumber(USART_TypeDef* USARTx) {
#ifdef USART1
	if (USARTx == USART1) {
		return 0;
	}
#endif
#ifdef USART2
	if (USARTx == USART2) {
		return 1;
	}
#endif
#ifdef USART3
	if (USARTx == USART3) {
		return 2;
	}
#endif
#ifdef UART4
	if (USARTx == UART4) {
		return 3;
	}
#endif
#ifdef UART5
	if (USARTx == UART5) {
		return 4;
	}
#endif
#ifdef USART6
	if (USARTx == USART6) {
		return 5;
	}
#endif
#ifdef UART7
	if (USARTx == UART7) {
		return 6;
	}
#endif
#ifdef UART8
	if (USARTx == UART8) {
		return 7;
	}
#endif

	return 0;
}
