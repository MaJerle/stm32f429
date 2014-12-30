/**
 *	USART Library for STM32F4 with receive interrupt
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/04/library-04-connect-stm32f429-discovery-to-computer-with-usart/
 *	@version 	v2.0
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
 * Version 2.0
 *	- December 21, 2014
 *	- New cyclic buffer system,
 *		each U(S)ART can have different buffer size (less RAM can be used for USART purpose)
 *	- Added function to check if buffer is full,
 *	- TM_USART_Gets now returns 0 till '\n' is not available in buffer or buffer is full
 *		Useful for prevent infinite loop if '\n' never happen
 *
 *	Library works for all 8 U(S)ARTs which are supported on STM32F429.
 *
 *	Every USART channel has it's own receive interrupt which stores incoming data into cyclic buffer.
 *	If you want to use your own receive handler, then you have to open defines.h files and set a define
 *
 *	//Use custom IRQ Receive handler
 *
 *	//Change X with possible U(S)ARTs: USART1, USART2, USART3, UART4, UART5, USART6, UART7, UART8
 *	//#define TM_X_USE_CUSTOM_IRQ
 *
 *	After you set define, you have to create a function, which will handle custom request
 *
 *	//Change X with possible U(S)ARTs: USART1, USART2, USART3, UART4, UART5, USART6, UART7, UART8
 *	//Parameter c is a received character
 *	void TM_X_ReceiveHandler(uint8_t c) {
 *		//Do your stuff here when byte received
 *	}
 *
 *	In your project you can set internal cyclic buffer length, default is 32Bytes, with:
 *
 *	#define TM_USART_BUFFER_SIZE number_of_bytes
 *
 *	in your project's defines.h file. This will set default length for each buffer.
 *	So if you are working with F429 (it has 8 U(S)ARTs) then you will use 8kB RAM if 
 *	you set define above to 1024.
 *
 * 	As of version 2.0, you can now set different buffer sizes for different U(S)ARTs.
 *	If you don't change anything, then all USART's have buffer length of value, stored in
 *	TM_USART_BUFFER_SIZE define. If you want let's say just for USART1 to be 1kB, but others default value,
 *	you can add define below in defines.h file:
 *
 *	//Buffer length for USART1 is 1kB
 *	#define TM_USART1_BUFFER_SIZE 1024
 *
 *	Other possible settings are (for other U(S)ARTs):
 *		- TM_USART1_BUFFER_SIZE
 *		- TM_USART2_BUFFER_SIZE
 *		- TM_USART3_BUFFER_SIZE
 *		- TM_UART4_BUFFER_SIZE
 *		- TM_UART5_BUFFER_SIZE
 *		- TM_USART6_BUFFER_SIZE
 *		- TM_UART7_BUFFER_SIZE
 *		- TM_UART8_BUFFER_SIZE
 *	
 *	Pinout
 *
 *				|PINSPACK 1		|PINSPACK 2		|PINSPACK 3	
 *	U(S)ARTX	|TX		RX		|TX		RX		|TX		RX
 *	
 *	USART1		|PA9	PA10	|PB6	PB7		|-		-
 *	USART2		|PA2	PA3		|PD5	PD6		|-		-
 *	USART3		|PB10	PB11	|PC10	PC11	|PD8	PD9
 *	UART4		|PA0	PA1		|PC10	PC11	|-		-
 *	UART5		|PC12	PD2		|-		-		|-		-
 *	USART6		|PC6	PC7		|PG14	PG9		|-		-
 *	UART7		|PE8	PE7		|PF7	PF6		|-		-
 *	UART8		|PE1	PE0		|-		-		|-		-
 *
 * Possible changes in USART operation
 * 
 * 	//Change X with possible U(S)ARTs: USART1, USART2, USART3, UART4, UART5, USART6, UART7, UART8
 * 	//Set flow control
 *	#define TM_X_HARDWARE_FLOW_CONTROL		USART_HardwareFlowControl_None
 *	//Set mode
 *	#define TM_X_MODE						USART_Mode_Tx | USART_Mode_Rx
 *	//Set parity
 *	#define TM_X_PARITY						USART_Parity_No
 *	//Set stopbits
 *	#define TM_X_STOP_BITS					USART_StopBits_1
 *	//Set USART datasize
 *	#define TM_X_WORD_LENGTH				USART_WordLength_8b
 */
#ifndef TM_USART_H
#define TM_USART_H 200
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx USART
 * - defines.h
 * - attributes
 */
/**
 * Includes
 */
#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "defines.h"
#include "attributes.h"

#if defined (STM32F40_41xxx)
#define TM_USE_USART1
#define TM_USE_USART2
#define TM_USE_USART3
#define TM_USE_UART4
#define TM_USE_UART5
#define TM_USE_USART6
#endif /* STM32F40_41xxx */
#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
#define TM_USE_USART1
#define TM_USE_USART2
#define TM_USE_USART3
#define TM_USE_UART4
#define TM_USE_UART5
#define TM_USE_USART6
#define TM_USE_UART7
#define TM_USE_UART8
#endif /* STM32F427_437x || STM32F429_439xx */
#if defined (STM32F401xx) || defined(STM32F411xE)
#define TM_USE_USART1
#define TM_USE_USART2
#define TM_USE_USART6
#endif /* STM32F401xx || STM32F411xx */

/* Default buffer size for each USART */
#ifndef TM_USART_BUFFER_SIZE
#define TM_USART_BUFFER_SIZE 				32
#endif

/* Set default buffer size for specific USART if not set by user */
#ifndef TM_USART1_BUFFER_SIZE
#define TM_USART1_BUFFER_SIZE				TM_USART_BUFFER_SIZE
#endif
#ifndef TM_USART2_BUFFER_SIZE
#define TM_USART2_BUFFER_SIZE				TM_USART_BUFFER_SIZE
#endif
#ifndef TM_USART3_BUFFER_SIZE
#define TM_USART3_BUFFER_SIZE				TM_USART_BUFFER_SIZE
#endif
#ifndef TM_UART4_BUFFER_SIZE
#define TM_UART4_BUFFER_SIZE				TM_USART_BUFFER_SIZE
#endif
#ifndef TM_UART5_BUFFER_SIZE
#define TM_UART5_BUFFER_SIZE				TM_USART_BUFFER_SIZE
#endif
#ifndef TM_USART6_BUFFER_SIZE
#define TM_USART6_BUFFER_SIZE				TM_USART_BUFFER_SIZE
#endif
#ifndef TM_UART7_BUFFER_SIZE
#define TM_UART7_BUFFER_SIZE				TM_USART_BUFFER_SIZE
#endif
#ifndef TM_UART8_BUFFER_SIZE
#define TM_UART8_BUFFER_SIZE				TM_USART_BUFFER_SIZE
#endif

/* NVIC Priority */
#ifndef TM_USART_NVIC_PRIORITY
#define TM_USART_NVIC_PRIORITY				0x00
#endif

//U(S)ART settings, can be changed in your defines.h project file
//USART1 default settings
#ifndef TM_USART1_HARDWARE_FLOW_CONTROL
#define TM_USART1_HARDWARE_FLOW_CONTROL		USART_HardwareFlowControl_None
#endif
#ifndef TM_USART1_MODE
#define TM_USART1_MODE						USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef TM_USART1_PARITY
#define TM_USART1_PARITY					USART_Parity_No
#endif
#ifndef TM_USART1_STOP_BITS
#define TM_USART1_STOP_BITS					USART_StopBits_1
#endif
#ifndef TM_USART1_WORD_LENGTH
#define TM_USART1_WORD_LENGTH				USART_WordLength_8b
#endif

//USART2 default settings
#ifndef TM_USART2_HARDWARE_FLOW_CONTROL
#define TM_USART2_HARDWARE_FLOW_CONTROL		USART_HardwareFlowControl_None
#endif
#ifndef TM_USART2_MODE
#define TM_USART2_MODE						USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef TM_USART2_PARITY
#define TM_USART2_PARITY					USART_Parity_No
#endif
#ifndef TM_USART2_STOP_BITS
#define TM_USART2_STOP_BITS					USART_StopBits_1
#endif
#ifndef TM_USART2_WORD_LENGTH
#define TM_USART2_WORD_LENGTH				USART_WordLength_8b
#endif

//USART3 default settings
#ifndef TM_USART3_HARDWARE_FLOW_CONTROL
#define TM_USART3_HARDWARE_FLOW_CONTROL		USART_HardwareFlowControl_None
#endif
#ifndef TM_USART3_MODE
#define TM_USART3_MODE						USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef TM_USART3_PARITY
#define TM_USART3_PARITY					USART_Parity_No
#endif
#ifndef TM_USART3_STOP_BITS
#define TM_USART3_STOP_BITS					USART_StopBits_1
#endif
#ifndef TM_USART3_WORD_LENGTH
#define TM_USART3_WORD_LENGTH				USART_WordLength_8b
#endif

//UART4 default settings
#ifndef TM_UART4_HARDWARE_FLOW_CONTROL
#define TM_UART4_HARDWARE_FLOW_CONTROL		USART_HardwareFlowControl_None
#endif
#ifndef TM_UART4_MODE
#define TM_UART4_MODE						USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef TM_UART4_PARITY
#define TM_UART4_PARITY						USART_Parity_No
#endif
#ifndef TM_UART4_STOP_BITS
#define TM_UART4_STOP_BITS					USART_StopBits_1
#endif
#ifndef TM_UART4_WORD_LENGTH
#define TM_UART4_WORD_LENGTH				USART_WordLength_8b
#endif

//UART5 default settings
#ifndef TM_UART5_HARDWARE_FLOW_CONTROL
#define TM_UART5_HARDWARE_FLOW_CONTROL		USART_HardwareFlowControl_None
#endif
#ifndef TM_UART5_MODE
#define TM_UART5_MODE						USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef TM_UART5_PARITY
#define TM_UART5_PARITY						USART_Parity_No
#endif
#ifndef TM_UART5_STOP_BITS
#define TM_UART5_STOP_BITS					USART_StopBits_1
#endif
#ifndef TM_UART5_WORD_LENGTH
#define TM_UART5_WORD_LENGTH				USART_WordLength_8b
#endif

//USART6 default settings
#ifndef TM_USART6_HARDWARE_FLOW_CONTROL
#define TM_USART6_HARDWARE_FLOW_CONTROL		USART_HardwareFlowControl_None
#endif
#ifndef TM_USART6_MODE
#define TM_USART6_MODE						USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef TM_USART6_PARITY
#define TM_USART6_PARITY					USART_Parity_No
#endif
#ifndef TM_USART6_STOP_BITS
#define TM_USART6_STOP_BITS					USART_StopBits_1
#endif
#ifndef TM_USART6_WORD_LENGTH
#define TM_USART6_WORD_LENGTH				USART_WordLength_8b
#endif

//UART7 default settings
#ifndef TM_UART7_HARDWARE_FLOW_CONTROL
#define TM_UART7_HARDWARE_FLOW_CONTROL		USART_HardwareFlowControl_None
#endif
#ifndef TM_UART7_MODE
#define TM_UART7_MODE						USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef TM_UART7_PARITY
#define TM_UART7_PARITY						USART_Parity_No
#endif
#ifndef TM_UART7_STOP_BITS
#define TM_UART7_STOP_BITS					USART_StopBits_1
#endif
#ifndef TM_UART7_WORD_LENGTH
#define TM_UART7_WORD_LENGTH				USART_WordLength_8b
#endif

//UART8 default settings
#ifndef TM_UART8_HARDWARE_FLOW_CONTROL
#define TM_UART8_HARDWARE_FLOW_CONTROL		USART_HardwareFlowControl_None
#endif
#ifndef TM_UART8_MODE
#define TM_UART8_MODE						USART_Mode_Tx | USART_Mode_Rx
#endif
#ifndef TM_UART8_PARITY
#define TM_UART8_PARITY						USART_Parity_No
#endif
#ifndef TM_UART8_STOP_BITS
#define TM_UART8_STOP_BITS					USART_StopBits_1
#endif
#ifndef TM_UART8_WORD_LENGTH
#define TM_UART8_WORD_LENGTH				USART_WordLength_8b
#endif

/**
 * When you initialize USARTx, you have to select which pins pack you will use
 *
 * Pinspack 3 can be used only for USART3
 */
typedef enum {
	TM_USART_PinsPack_1,
	TM_USART_PinsPack_2,
	TM_USART_PinsPack_3
} TM_USART_PinsPack_t;

/**
 * Initialize USARTx
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6, UART7. UART8
 * 	- TM_USART_PinsPack_t pinspack: Pinspack used
 * 		- TM_USART_PinsPack_1
 * 		- TM_USART_PinsPack_2
 * 		- TM_USART_PinsPack_3 (only for USART3)
 *	- uint32_t baudrate: baudrate for USARTx
 */
extern void TM_USART_Init(USART_TypeDef* USARTx, TM_USART_PinsPack_t pinspack, uint32_t baudrate);

/**
 * Put character to USARTx
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6, UART7. UART8
 *	- volatile char c: char to be sent to USART
 */
extern void TM_USART_Putc(USART_TypeDef* USARTx, volatile char c);

/**
 * Put string to USARTx
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6, UART7. UART8
 *	- char* str: pointer to first character in string
 */
extern void TM_USART_Puts(USART_TypeDef* USARTx, char* str);

/**
 * Get character from internal buffer
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6, UART7. UART8
 *
 * Returned character if exists in buffer, otherwise zero
 */
extern uint8_t TM_USART_Getc(USART_TypeDef* USARTx);

/**
 * Get string from USART
 *
 * This function can create a string from USART received data.
 * It generates string until "\n" is not recognized or buffer length is full.
 *
 * As of version 1.5, this function automatically adds "\n" at the end of string
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6, UART7. UART8
 * 	- char* buffer: pointer to character buffer
 * 	- uint16_t bufsize: size of your buffer
 *
 * Returns number of valid characters in buffer
 */
extern uint16_t TM_USART_Gets(USART_TypeDef* USARTx, char* buffer, uint16_t bufsize);

/**
 * Check if character is available in internal buffer
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6, UART7. UART8
 * 	- uint8_t c: character you want to check if is available in buffer
 *
 * Returns 1 if character is available or 0 if not
 */
extern uint8_t TM_USART_FindCharacter(USART_TypeDef* USARTx, uint8_t c);

/**
 * Checks if internal buffer is empty
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6, UART7. UART8
 *
 * Returns 1 if buffer is empty, otherwise 0
 */
extern uint8_t TM_USART_BufferEmpty(USART_TypeDef* USARTx);

/**
 * Checks if internal buffer is full
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6, UART7. UART8
 *
 * Returns 1 if buffer is full, otherwise 0
 */
extern uint8_t TM_USART_BufferFull(USART_TypeDef* USARTx);

/**
 * Clear USART internal cyclic buffer
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6, UART7. UART8
 *
 * No return
 */
extern void TM_USART_ClearBuffer(USART_TypeDef* USARTx);

/**
 * These functions are used, if you want to make yourself interrupt handler.
 *
 * To enable them, you have to add corresponsing define in you defines.h file.
 * For example:
 *	- If you want to enable USART1 custom handler, then add following line in defines.h file:
 *		#define TM_USART_USE_CUSTOM_IRQ
 *		and create function TM_USART1_ReceiveHandler somewhere in your project
 *
 * Parameters:
 *	- uint8_t c: Character, received from USART
 *
 * Returns void
 * With '__weak' parameter for prevent link errors if function is not declared
 */
__weak void TM_USART1_ReceiveHandler(uint8_t c);
__weak void TM_USART2_ReceiveHandler(uint8_t c);
__weak void TM_USART3_ReceiveHandler(uint8_t c);
__weak void TM_UART4_ReceiveHandler(uint8_t c);
__weak void TM_UART5_ReceiveHandler(uint8_t c);
__weak void TM_USART6_ReceiveHandler(uint8_t c);
__weak void TM_UART7_ReceiveHandler(uint8_t c);
__weak void TM_UART8_ReceiveHandler(uint8_t c);


#endif
