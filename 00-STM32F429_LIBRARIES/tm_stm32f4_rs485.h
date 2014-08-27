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
 *	@license	GNU GPL v3
 *	
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
#ifndef TM_RS485_
#define TM_RS485_ 100
/**
 * Library dependencies
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - defines.h
 * - TM USART
 * - TM SOFTCRC
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_softcrc.h"

//Default RS485 USART settings
#ifndef RS485_USART
//Select your USART
#define RS485_USART				USART1
//Select your USART pins used
#define RS485_USART_PINSPACK	TM_USART_PinsPack_1
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

//Clock cycles before send and after send
#ifndef RS485_DELAY
#define RS485_DELAY 			10000
#endif

//Define RS485 START byte
#ifndef RS485_START_BYTE	
#define RS485_START_BYTE		0xAA
#endif
//Define RS485 Stop byte
#ifndef RS485_STOP_BYTE			
#define RS485_STOP_BYTE			0x55
#endif

#define RS485_ENABLE_RX			GPIO_ResetBits(RS485_RXTX_ENABLE_PORT, RS485_RXTX_ENABLE_PIN)	//RS485_RXTX_ENABLE_PORT->BSRRH = RS485_RXTX_ENABLE_PIN
#define RS485_ENABLE_TX			GPIO_SetBits(RS485_RXTX_ENABLE_PORT, RS485_RXTX_ENABLE_PIN)		//RS485_RXTX_ENABLE_PORT->BSRRL = RS485_RXTX_ENABLE_PIN

/**
 * RS485 Struct
 */
typedef struct {
	uint8_t id;	//My id
} TM_RS485_t;

/**
 * Initialize RS485
 *
 * Parameters:
 * 	- uint8_t id
 * 		My ID address to identify devices on network
 * 	- uint32_t baudrate
 * 		Working baudrate for USART
 */
extern void TM_RS485_Init(uint8_t id, uint32_t baudrate);

/**
 * Send data over RS485
 *
 * Parameters:
 * 	- uint8_t receiver
 * 		Receiver ID address
 * 	- uint8_t* data
 * 		Pointer to data to be send
 * 	- uint8_t count
 * 		Number of data to be send
 */
extern uint8_t TM_RS485_Send(uint8_t receiver, uint8_t* data, uint8_t count);

/**
 * Receive data over RS485
 *
 * Parameters:
 * 	- uint8_t* from
 * 		Pointer to store ID address from sender
 * 	- uint8_t* data
 * 		Pointer to save data to
 * 	- uint8_t* count
 * 		Pointer to store data count received
 *
 * Returns 1 if data are valid, or zero if not
 */
extern uint8_t TM_RS485_Receive(uint8_t* from, uint8_t* data, uint8_t* count);

/**
 * Private
 */
/**
 * Initialize RS485 pins
 *
 *
 */
extern void TM_RS485_InitPins(void);

#endif
