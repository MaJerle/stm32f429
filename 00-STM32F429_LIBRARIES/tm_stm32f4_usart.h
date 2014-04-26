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
 * When you initialize USARTx, you have to select which pins pack you will use
 *
 * Pinspack 3 can be used only for USART3
 */
typedef enum {
	TM_USART_PinsPack_1,
	TM_USART_PinsPack_2,
	TM_USART_PinsPack_3
} TM_USART_PinsPack_t;

typedef struct {
	GPIO_TypeDef* GPIOx;
	uint16_t RCCx;
	uint16_t GPIO_Pin;
	uint8_t GPIO_PinSource;
} TM_USART_Pin_t;

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
 * Initialize USART1 pins
 *
 * Parameters:
 * 	- TM_USART_PinsPack_t pinspack: Pinspack used
 * 		- TM_USART_PinsPack_1
 * 			- TX: PA9
 * 			- RX: PA10
 * 		- TM_USART_PinsPack_2
 * 			- TX: PB6
 * 			- RX: PB7
 */
extern void TM_USART1_InitPins(TM_USART_PinsPack_t pinspack);

/**
 * Initialize USART2 pins
 *
 * Parameters:
 * 	- TM_USART_PinsPack_t pinspack: Pinspack used
 * 		- TM_USART_PinsPack_1
 * 			- TX: PA2
 * 			- RX: PA3
 * 		- TM_USART_PinsPack_2
 * 			- TX: PD5
 * 			- RX: PD6
 */
extern void TM_USART2_InitPins(TM_USART_PinsPack_t pinspack);

/**
 * Initialize USART3 pins
 *
 * Parameters:
 * 	- TM_USART_PinsPack_t pinspack: Pinspack used
 * 		- TM_USART_PinsPack_1
 * 			- TX: PB10
 * 			- RX: PB11
 * 		- TM_USART_PinsPack_2
 * 			- TX: PC10
 * 			- RX: PC11
 * 		- TM_USART_PinsPack_3
 * 			- TX: PD8
 * 			- RX: PD9
 */
extern void TM_USART3_InitPins(TM_USART_PinsPack_t pinspack);

/**
 * Initialize UART4 pins
 *
 * Parameters:
 * 	- TM_USART_PinsPack_t pinspack: Pinspack used
 * 		- TM_USART_PinsPack_1
 * 			- TX: PA0
 * 			- RX: PA1
 * 		- TM_USART_PinsPack_2
 * 			- TX: PC10
 * 			- RX: PC11
 */
extern void TM_UART4_InitPins(TM_USART_PinsPack_t pinspack);

/**
 * Initialize UART5 pins
 *
 * Parameters:
 * 	- TM_USART_PinsPack_t pinspack: Pinspack used
 * 		- TM_USART_PinsPack_1
 * 			- TX: PC12
 * 			- RX: PD2
 */
extern void TM_UART5_InitPins(TM_USART_PinsPack_t pinspack);

/**
 * Initialize USART6 pins
 *
 * Parameters:
 * 	- TM_USART_PinsPack_t pinspack: Pinspack used
 * 		- TM_USART_PinsPack_1
 * 			- TX: PC6
 * 			- RX: PC7
 * 		- TM_USART_PinsPack_2
 * 			- TX: PG14
 * 			- RX: PG9
 */
extern void TM_USART6_InitPins(TM_USART_PinsPack_t pinspack);

/**
 * Initialize UART7 pins
 *
 * Parameters:
 * 	- TM_USART_PinsPack_t pinspack: Pinspack used
 * 		- TM_USART_PinsPack_1
 * 			- TX: PE8
 * 			- RX: PE7
 * 		- TM_USART_PinsPack_2
 * 			- TX: PF7
 * 			- RX: PF6
 */
extern void TM_UART7_InitPins(TM_USART_PinsPack_t pinspack);

/**
 * Initialize UART8 pins
 *
 * Parameters:
 * 	- TM_USART_PinsPack_t pinspack: Pinspack used
 * 		- TM_USART_PinsPack_1
 * 			- TX: PE1
 * 			- RX: PE0
 */
extern void TM_UART8_InitPins(TM_USART_PinsPack_t pinspack);

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
 * USART1 Interrupt handler
 *
 * Called when data is received on USART1
 */
extern void USART1_IRQHandler(void);

/**
 * Insert received data into internal buffer
 * Called in interrupt
 *
 * Parameters:
 * 	- uint8_t USARTnum
 * 		- USART number: starting from 0
 * 	- char c: character to be inserted in buffer
 */
extern void TM_USART_InsertToBuffer(uint8_t USARTnum, char c);

/**
 * Check's if internal buffer is empty
 *
 * Parameters:
 * 	- USART_TypeDef* USARTx: which USART channel
 * 		USART1, USART2, USART3, UART4, UART5, USART6, UART7. UART8
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
 * 		USART1, USART2, USART3, UART4, UART5, USART6, UART7. UART8
 *
 * Returns x - 1 number.
 */
uint8_t TM_USART_GetUsartNumber(USART_TypeDef* USARTx);
#endif
