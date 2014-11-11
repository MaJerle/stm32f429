/**
 *	Leds and button library for STM32F401, STM32F4 and STM32F429 Discovery boards.
 *	Also works with Nucleo boards
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/04/stm32f429-discovery-gpio-tutorial-with-onboard-leds-and-button/
 *	@version 	v1.5
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
 * 	CHANGELOG
 *
 *	- Version 1.5
 *		November 06, 2014
 *		Added function TM_DISCO_SetLed()
 *
 *	- Version 1.4
 *		Added support for Nucleo F411-RE board
 *
 *	- Version 1.3
 *		Added support for STM32F401 Discovery board
 *
 *	- Version 1.2
 *		Added support for Nucleo F401-RE board
 *	
 *	- Version 1.1
 *		Check if LED is on or off
 *
 * 	Library works for all three F4 Discovery boards.
 *	It can also be used with NUCLEO F401RE board
 *	
 *	Supported boards:
 *
 *	STM32F4 Discovery: (STM32F407VG)
 * 		Open project options and add "STM32F407VG" define (without quotes)
 *		- Leds:
 *			- LED_GREEN 	on PD12
 *			- LED_ORANGE	on PD13
 *			- LED_RED 		on PD14
 *			- LED_BLUE 		on PD15
 *		- Button: (HIGH when pressed)
 *			- Blue button	on PA0
 *			
 *	STM32F429 Discovery: (STM32F429ZI)
 * 		Open project options and add "STM32F429ZI" define (without quotes)
 *		- Leds:
 *			- LED_GREEN 	on PG13
 *			- LED_RED 		on PG14
 *		- Button: (HIGH when pressed)
 *			- Blue button	on PA0	
 *
 *	NUCLEO-F401: (STM32F401RE)
 *	NUCLEO-F411: (STM32F411RE)
 * 		Open project options and add "STM32F401RE" or "STM32F411RE" define (without quotes)
 *		- Led:
 *			- LED_GREEN 	on PA5
 *		- Button: (LOW when pressed)
 *			- Blue button	on PC13
 *
 *	STM32F401 Discovery: (STM32F401VC)
 * 		Open project options and add "STM32F401VC" define (without quotes)
 *		- Leds:
 *			- LED_GREEN 	on PD12
 *			- LED_ORANGE	on PD13
 *			- LED_RED 		on PD14
 *			- LED_BLUE 		on PD15
 *		- Button: (HIGH when pressed)
 *			- Blue button	on PA0
 */
#ifndef TM_DISCO_H
#define TM_DISCO_H 150
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"

/* Recognize correct board */
/* CooCox support */
#if defined(STM32F429_439xx) || defined(STM32F429ZI)
	/* STM32F429 Discovery support */
	#ifndef TM_DISCO_STM32F429_DISCOVERY
		#define TM_DISCO_STM32F429_DISCOVERY
	#endif 
#elif defined(STM32F407VG) || defined(STM32F401VC)// || defined(STM32F40_41xxx)
	/* STM32F4 and STM32F401 Discovery support */
	#ifndef TM_DISCO_STM32F4_DISCOVERY
		#define TM_DISCO_STM32F4_DISCOVERY
	#endif
#elif defined (STM32F401xx) || defined(STM32F401RE) || defined(STM32F401RB)
	/* Nucleo F401RE board support */
	#ifndef TM_DISCO_NUCLEO_F401
		#define TM_DISCO_NUCLEO_F401
	#endif
#elif defined (STM32F411xx) || defined(STM32F411RE) || defined(STM32F411RB)
	/* Nucleo F411RE board support */
	#ifndef TM_DISCO_NUCLEO_F411
		#define TM_DISCO_NUCLEO_F411
	#endif
#endif

/**
 * Defines
 */
/* STM32F429 Discovery */
#if defined(TM_DISCO_STM32F429_DISCOVERY)
	#define LED_GREEN					GPIO_Pin_13
	#define LED_RED						GPIO_Pin_14
	#define LED_ORANGE					0
	#define LED_BLUE					0
	#define LED_ALL						LED_GREEN | LED_RED
	
	#define TM_DISCO_LED_RCC			RCC_AHB1Periph_GPIOG
	#define TM_DISCO_LED_PORT			GPIOG
	#define TM_DISCO_LED_PINS			LED_GREEN | LED_RED

	#define TM_DISCO_BUTTON_RCC			RCC_AHB1Periph_GPIOA
	#define TM_DISCO_BUTTON_PORT		GPIOA
	#define TM_DISCO_BUTTON_PIN			GPIO_Pin_0
	#define TM_DISCO_BUTTON_PRESSED		Bit_SET
/* STM32F4 & STM32F401 Discovery */
#elif defined(TM_DISCO_STM32F4_DISCOVERY) || defined(TM_DISCO_STM32F401_DISCOVERY)
	#define LED_GREEN					GPIO_Pin_12
	#define LED_ORANGE					GPIO_Pin_13
	#define LED_RED						GPIO_Pin_14
	#define LED_BLUE					GPIO_Pin_15
	#define LED_ALL						LED_GREEN | LED_RED | LED_ORANGE | LED_BLUE
	
	#define TM_DISCO_LED_RCC			RCC_AHB1Periph_GPIOD
	#define TM_DISCO_LED_PORT			GPIOD
	#define TM_DISCO_LED_PINS			LED_GREEN | LED_RED | LED_ORANGE | LED_BLUE

	#define TM_DISCO_BUTTON_RCC			RCC_AHB1Periph_GPIOA
	#define TM_DISCO_BUTTON_PORT		GPIOA
	#define TM_DISCO_BUTTON_PIN			GPIO_Pin_0
	#define TM_DISCO_BUTTON_PRESSED		Bit_SET
/* Nucleo F401-RE & F411-RE */
#elif defined(TM_DISCO_NUCLEO_F401) || defined(TM_DISCO_NUCLEO_F411)
	#define LED_GREEN					GPIO_Pin_5
	#define LED_RED						0
	#define LED_ORANGE					0
	#define LED_BLUE					0
	#define LED_ALL						LED_GREEN
	
	#define TM_DISCO_LED_RCC			RCC_AHB1Periph_GPIOA
	#define TM_DISCO_LED_PORT			GPIOA
	#define TM_DISCO_LED_PINS			LED_GREEN

	#define TM_DISCO_BUTTON_RCC			RCC_AHB1Periph_GPIOC
	#define TM_DISCO_BUTTON_PORT		GPIOC
	#define TM_DISCO_BUTTON_PIN			GPIO_Pin_13
	#define TM_DISCO_BUTTON_PRESSED		Bit_RESET
#else
	#error "tm_stm32f4_disco.h: Please select your board"
#endif

/**
 * Configure led pins to output
 *
 */
extern void TM_DISCO_LedInit(void);

/**
 * Configure blue button as input
 *
 */
extern void TM_DISCO_ButtonInit(void);

/**
 * Turn on LED
 *
 * Parameters:
 * 	- uint16_t led:
 * 		LED_GREEN
 * 		LED_RED
 *		LED_ORANGE
 *		LED_BLUE
 */
extern void TM_DISCO_LedOn(uint16_t led);

/**
 * Turn off LED
 *
 * Parameters:
 * 	- uint16_t led:
 * 		LED_GREEN
 * 		LED_RED
 *		LED_ORANGE
 *		LED_BLUE
 */
extern void TM_DISCO_LedOff(uint16_t led);

/**
 * Toggle LED
 *
 * Parameters:
 * 	- uint16_t led:
 * 		LED_GREEN
 * 		LED_RED
 *		LED_ORANGE
 *		LED_BLUE
 */
extern void TM_DISCO_LedToggle(uint16_t led);

/**
 * Check's if led is on
 *
 * Parameters:
 * 	- uint16_t led:
 * 		LED_GREEN
 * 		LED_RED
 *		LED_ORANGE
 *		LED_BLUE
 * 
 * Return 1 if turned on, otherwise 0
 */
extern uint8_t TM_DISCO_LedIsOn(uint16_t led);

/**
 * Set led's state with one function
 *
 * Parameters:
 * 	- uint16_t led:
 * 		LED_GREEN
 * 		LED_RED
 *		LED_ORANGE
 *		LED_BLUE
 *	- uint8_t state:
 *		0: led is off
 *		> 0: led is on
 * 
 * Return 1 if turned on, otherwise 0
 */
extern void TM_DISCO_SetLed(uint16_t led, uint8_t state);

/**
 * Checks if button is pressed
 *
 * Returns 1 if is pressed, otherwise 0
 */
extern uint8_t TM_DISCO_ButtonPressed(void);

#endif
