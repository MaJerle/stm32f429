/**
 *	USART Library for STM32F4 with receive interrupt
 *
 *	It works for USART1.
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@version 	v1.0
 *	@gcc		v4.7 20013qr3
 *	@ide		CooCox CoIDE v1.7.6
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
 * Initialize USART1
 *
 * @param uint32_t baudrate: specify USART baudrate ex. 9600
 */
extern void TM_USART_Init(uint32_t baudrate);
/**
 * Put character to USART1
 *
 * @param volatile char c: char to put
 */
extern void TM_USART_Putc(volatile char c);
/**
 * Put string to USART1
 *
 * @param char* str: pointer to first character in string
 */
extern void TM_USART_Puts(char* str);
/**
 * USART1 Interrupt handler
 *
 */
extern void USART1_IRQHandler(void);
/**
 * Insert received data into internal buffer
 *
 */
void TM_USART_InsertToBuffer(char c);
/**
 *	Get character from internal buffer
 *
 */
uint8_t TM_USART_Getc(void);

#endif
