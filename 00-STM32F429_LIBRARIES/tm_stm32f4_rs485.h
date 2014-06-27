/**
 * RS485 library for STM43F4xx
 * Supports half duplex mode
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		
 *	@version 	v1.0
 *	@ide		Keil uVision
 */
#ifndef TM_RS485_
#define TM_RS485_ 100
/**
 * Library dependencies
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - defines.h
 * - TM USART
 * - TM CRC
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_crc.h"

#ifndef RS485_USART
//Select your USART
#define RS485_USART				USART1
//Select your USART pins used
#define RS485_USART_PINSPACK	TM_USART_PinsPack_1
//define USARTx interrupt handler selected
#define RS485_USART_IRQ_USART1			
#endif

//Pin for enable/disable RX or TX mode
//When low, RX is enabled, when HIGH, TX is enabled
#ifndef RS485_RXTX_ENABLE_PIN
#define RS485_RXTX_ENABLE_RCC	RCC_AHB1Periph_GPIOA
#define RS485_RXTX_ENABLE_PORT	GPIOA
#define RS485_RXTX_ENABLE_PIN	GPIO_Pin_11
#endif

#define RS485_BUFFER_LENGTH 	TM_USART_BUFFER_SIZE
#define RS485_CLEAR_BUFFER		while (!TM_USART_BufferEmpty(RS485_USART)) { TM_USART_Getc(RS485_USART); }

#ifndef RS485_DELAY
#define RS485_DELAY 			1000000
#endif

#define RS485_START_BYTE		0xAA
#define RS485_STOP_BYTE			0x55

#define RS485_ENABLE_RX			RS485_RXTX_ENABLE_PORT->BSRRH = RS485_RXTX_ENABLE_PIN
#define RS485_ENABLE_TX			RS485_RXTX_ENABLE_PORT->BSRRL = RS485_RXTX_ENABLE_PIN

typedef struct {
	uint8_t id;
	uint8_t inReceiving;
	uint8_t receivedCharacter;
	char buffer[RS485_BUFFER_LENGTH];
} TM_RS485_t;

extern void TM_RS485_Init(uint8_t id, uint32_t baudrate);
extern void TM_RS485_InitPins(void);

extern uint8_t TM_RS485_Send(uint8_t receiver, uint8_t* data, uint8_t count);
extern uint8_t TM_RS485_Receive(uint8_t *from, uint8_t* data, uint8_t *count);

#endif
