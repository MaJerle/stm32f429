/**
 *	RS485 library for STM43F4xx
 *	Supports half-duplex RS485 mode
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/12/library-47-rs485-implementation-for-stm32f4
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
 *
 * Default pinout
 *
 *	RS485 TRANSCEIVER	STM32F4xx		Description
 *
 *  D					PB6				RX pin for transceiver. Connected to TX pin on STM32F4.
 *	R					PB7				TX pin for transceiver. Connected to XX pin on STM32F4.
 *	DE / RE(active low)	PA8				Enable TX/RX mode for transceiver. 
 *
 */
#ifndef TM_RS485_H
#define TM_RS485_H 100
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx USART
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

/* Default RS485 USART settings */
#ifndef RS485_USART
#define RS485_USART				USART1
#define RS485_USART_PINSPACK	TM_USART_PinsPack_2
#endif

/* Pin for enable/disable RX or TX mode */
/* When low, RX is enabled, when HIGH, TX is enabled */
#ifndef RS485_DIR_PIN
#define RS485_DIR_RCC			RCC_AHB1Periph_GPIOA
#define RS485_DIR_PORT			GPIOA
#define RS485_DIR_PIN			GPIO_Pin_8
#endif

/* Define RS485 START byte */
#ifndef RS485_START_BYTE	
#define RS485_START_BYTE		(0xAA)
#endif

/* Define RS485 STOP byte */
#ifndef RS485_STOP_BYTE			
#define RS485_STOP_BYTE			(0x55)
#endif

/* Default RS485 buffer size */
/* If must be at least the same size as USART buffer length */
#ifndef RS485_BUFFER_LENGTH
#define RS485_BUFFER_LENGTH		(32)
#endif

/* Clock cycles before send and after send */
#ifndef RS485_DELAY
#define RS485_DELAY 			300
#endif

/* Data timeout */
#ifndef RS485_TIMEOUT
#define RS485_TIMEOUT			10000
#endif

/* Pin defines */
#define RS485_ENABLE_RX			RS485_DIR_PORT->BSRRH = RS485_DIR_PIN
#define RS485_ENABLE_TX			RS485_DIR_PORT->BSRRL = RS485_DIR_PIN

/* Clear buffer command */
#define RS485_CLEAR_BUFFER		TM_USART_ClearBuffer(RS485_USART);

/**
 * RS485 Struct
 *
 * Parameters:
 *	- uint8_t MyID:
 *		Used for transmit/receive data to detect proper ID
 *	- uint8_t ReceivedID:
 *		ID from packet arrived for us
 *	- uint8_t ReceivedCount:
 *		Number of available data arrived for us
 */
typedef struct {
	uint8_t MyID;
	uint8_t ReceivedID;
	uint8_t ReceivedCount;
} TM_RS485_t;

/**
 * Initialize RS485, prepare USART and set to receive mode
 *
 * Parameters:
 *	- TM_RS485_t* RS485:
 *		Pointer to RS485 struct
 * 	- uint8_t id:
 * 		My ID address to identify devices on network
 * 	- uint32_t baudrate:
 * 		Working baudrate for USART
 */
extern void TM_RS485_Init(TM_RS485_t* RS485, uint8_t id, uint32_t baudrate);

/**
 * Send data over RS485
 * Function automaticall sets RS485 transceiver to TX mode,
 * sends data and set it back to RX mode, so you will be able to receive new data.
 *
 * Parameters:
 *	- TM_RS485_t* RS485:
 *		Pointer to RS485 struct
 * 	- uint8_t receiver:
 * 		Receiver's ID address
 * 	- uint8_t* data:
 * 		Pointer to data to be send
 * 	- uint8_t count:
 * 		Number of data to be send
 *
 * Returns number of data bytes sent over RS485 bus
 */
extern uint8_t TM_RS485_Send(TM_RS485_t* RS485, uint8_t receiver, uint8_t* data, uint8_t count);

/**
 * Receive data over RS485
 * Function will returns 0 until all bytes are not available in USART internal buffer.
 * It can happen, that if you use the same data bytes value as START/STOP byte,
 * then it will think that all data are available and it will wait for all bytes to receive.
 * It real data will not receive in specific timeout, function will delete entire USART receive
 * buffer and it will return 0
 *
 * Parameters:
 *	- TM_RS485_t* RS485:
 *		Pointer to RS485 struct
 * 	- uint8_t* data:
 * 		Pointer to save data to
 *
 * Returns > 0: Data valid
 * Returns 0: Data not valid
 */
extern uint8_t TM_RS485_Receive(TM_RS485_t* RS485, uint8_t* data);

#endif
