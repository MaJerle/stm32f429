/**
 *	USART Library for STM32F4 with receive interrupt
 *
 *	It currently works for USART1
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@version 	v1.1
 *	@gcc		v4.7 20013qr3
 *	@ide		CooCox CoIDE v1.7.6
 */

/**
 * USART1 pins are
 *  - TX: PA9
 *  - RX: PA10
 *
 */

#ifndef TM_USART_
#define TM_USART_
/**
 * Includes
 */
#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
/**
 * Buffer size
 */
#ifndef TM_USART_BUFFER_SIZE
#define TM_USART_BUFFER_SIZE 32
#endif

/**
 * Initialize USART
 *
 * Currently supported only USART1
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6
 *	- uint32_t baudrate: baudrate for USART
 */
extern void TM_USART_Init(USART_TypeDef* USARTx, uint32_t baudrate);

/**
 * Put character to USARTx
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6
 *	- volatile char c: char to be sent to USART
 */
extern void TM_USART_Putc(USART_TypeDef* USARTx, volatile char c);

/**
 * Put string to USARTx
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6
 *	- char* str: pointer to first character in string
 */
extern void TM_USART_Puts(USART_TypeDef* USARTx, char* str);

/**
 * Get character from internal buffer
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6
 *
 * Returned character if exists in buffer, otherwise zero
 */
extern uint8_t TM_USART_Getc(USART_TypeDef* USARTx);

/**
 * USART1 Interrupt handler
 *
 * Called when data is received on USART1
 *
 */
extern void USART1_IRQHandler(void);

/**
 * Insert received data into internal buffer
 * Called in interrupt
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6
 * 	- char c: character to be inserted in buffer
 */
extern void TM_USART_InsertToBuffer(USART_TypeDef* USARTx, char c);

/**
 * Check's if internal buffer is empty
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6
 *
 * Returns 1 if buffer is empty, otherwise 0
 */
extern uint8_t TM_USART_BufferEmpty(USART_TypeDef* USARTx);

/**
 * Get USART number
 *
 * Called internally
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6
 *
 * Returns x - 1 number.
 */
uint8_t TM_USART_GetUsartNumber(USART_TypeDef* USARTx);
#endif
