#include "tm_stm32f4_usart.h"

/**
 * Variables
 */
volatile char TM_USART_Buffer[TM_USART_BUFFER_SIZE];
uint16_t tm_usart_buf_in = 0;
uint16_t tm_usart_buf_out = 0;
uint16_t tm_usart_buf_num = 0;

void TM_USART_Init(uint32_t baudrate) {
	/**
	 * Initialization structures declared
	 *
	 * GPIO: General Purpose Input Output
	 * USART: Universal Synchronous/Asynchronous Receiver/Transmitter
	 * NVIC: Nested Vector Interrupt Controller
	 */
	GPIO_InitTypeDef 	GPIO_InitStruct;
	USART_InitTypeDef 	USART_InitStruct;
	NVIC_InitTypeDef	NVIC_InitStruct;

	// Enable clock for GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/**
	 * Tell pins PA9 and PA10 which alternating function you will use
	 * @important Make sure, these lines are before pins configuration!
	 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	/**
	 * Configure pins PA9 (TX) and PA10 (RX) for USART1
	 * Set mode to AF (alternating function)
	 * Set output type to push-pull
	 * Enable pull UP resistor
	 * Set speed to 100MHz
	 *
	 * Initialize GPIOA
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/**
	 * Enable clock for USART1 peripheral
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/**
	 * Set Baudrate to value you pass to function
	 * Disable Hardware Flow control
	 * Set Mode To TX and RX, so USART will work in full-duplex mode
	 * Disable parity bit
	 * Set 1 stop bit
	 * Set Data bits to 8
	 *
	 * Initialize USART1
	 * Activate USART1
	 */
	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);

	/**
	 * Enable RX interrupt
	 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/**
	 * Set Channel to USART1
	 * Set Channel Cmd to enable. That will enable USART1 channel in NVIC
	 * Set Both priorities to 0. This means high priority
	 *
	 * Initialize NVIC
	 */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

void TM_USART_Putc(volatile char c) {
	//Wait until transmitter is empty
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
	//Send data
	USART_SendData(USART1, c);
}

void TM_USART_Puts(char* str) {
	while (*str) {
		TM_USART_Putc(*str++);
	}
}

void USART1_IRQHandler(void) {
	//Check if interrupt was because data is received
	if (USART_GetITStatus(USART1, USART_IT_RXNE)) {
		//Put received data into internal buffer
		TM_USART_InsertToBuffer(USART1->DR);
	}
}

void TM_USART_InsertToBuffer(char c) {
	//Still available space in buffer
	if (tm_usart_buf_num < TM_USART_BUFFER_SIZE) {
		if (tm_usart_buf_in > (TM_USART_BUFFER_SIZE - 1)) {
			tm_usart_buf_in = 0;
		}
		TM_USART_Buffer[tm_usart_buf_in] = c;
		tm_usart_buf_in++;
		tm_usart_buf_num++;
	}
}

uint8_t TM_USART_Getc(void) {
	uint8_t c = 0;
	//Check if we have any data in buffer
	if (tm_usart_buf_num > 0) {
		if (tm_usart_buf_out > (TM_USART_BUFFER_SIZE - 1)) {
			tm_usart_buf_out = 0;
		}
		c = TM_USART_Buffer[tm_usart_buf_out];
		TM_USART_Buffer[tm_usart_buf_out] = 0;
		tm_usart_buf_out++;
		tm_usart_buf_num--;
	}
	return c;
}

uint8_t TM_USART_BufferEmpty(void) {
	return !(tm_usart_buf_num > 0);
}

