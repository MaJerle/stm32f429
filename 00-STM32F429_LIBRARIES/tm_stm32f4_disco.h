/**
 *	Leds and button library for STM32F429Discovery board
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@version 	v1.0
 *	@gcc		v4.7 20013qr3
 *	@ide		CooCox CoIDE v1.7.6
 */

#ifndef TM_DISCO_
#define TM_DISCO_

/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"

/**
 * Defines
 */
#define LED_GREEN	GPIO_Pin_13
#define LED_RED		GPIO_Pin_14

/**
 * Configure led pins to output
 *
 */
void TM_DISCO_LedInit(void);

/**
 * Configure blue button as input
 *
 */
void TM_DISCO_ButtonInit(void);

/**
 * Turn on LED
 *
 * Parameters:
 * 	- uint16_t led:
 * 		LED_GREEN
 * 		LED_RED
 */
void TM_DISCO_LedOn(uint16_t led);

/**
 * Turn off LED
 *
 * Parameters:
 * 	- uint16_t led:
 * 		LED_GREEN
 * 		LED_RED
 */
void TM_DISCO_LedOff(uint16_t led);

/**
 * Checks if button is pressed
 *
 * Returns 1 if is pressed, otherwise 0
 */
uint8_t TM_DISCO_ButtonPressed(void);

#endif
