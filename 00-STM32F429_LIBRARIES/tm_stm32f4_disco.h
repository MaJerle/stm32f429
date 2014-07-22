/**
 *	Leds and button library for STM32F4 & STM32F429 Discovery board
 *	Library also supports F401 Nucleo board
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/04/stm32f429-discovery-gpio-tutorial-with-onboard-leds-and-button/
 *	@version 	v1.2
 *	@ide		Keil uVision
 *
 * 	Library works for both Discovery boards.
 *	It can also be used with F401-NUCLEO board
 *	
 *	STM32F4 Discovery: (STM32F407VG)
 *		- Leds:
 *			- LED_GREEN 	on PD12
 *			- LED_ORANGE	on PD13
 *			- LED_RED 		on PD14
 *			- LED_BLUE 		on PD15
 *		- Button: (pressed when low state)
 *			- Blue button	on PA0
 *			
 *	STM32F429 Discovery: (STM32F429ZI)
 *		- Leds:
 *			- LED_GREEN 	on PG13
 *			- LED_RED 		on PG14
 *		- Button: (pressed when low state)
 *			- Blue button	on PA0
 *
 *	F401 NUCLEO: (STM32F401RE)
 *		- Led:
 *			- LED_GREEN 	on PA5
 *		- Button: (pressed when high state)
 *			- Blue button	on PC13
 */
#ifndef TM_DISCO_
#define TM_DISCO_ 120
/**
 * Library dependencies
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

//Recognize STM32F4 or STM32F429 Discovery
//Added Nucleo board
#if defined(STM32F429_439xx)
	#define STM32F429_DISCOVERY
#elif defined(STM32F407VG) || defined(STM32F40_41xxx)
	#define STM32F4_DISCOVERY
#elif defined (STM32F401xx)
	#define NUCLEO_F401
#endif

/**
 * Defines
 */
#if defined(STM32F429_DISCOVERY)
	#define LED_GREEN					GPIO_Pin_13
	#define LED_RED						GPIO_Pin_14
	#define TM_DISCO_LED_RCC			RCC_AHB1Periph_GPIOG
	#define TM_DISCO_LED_PORT			GPIOG
	#define TM_DISCO_LED_PINS			LED_GREEN | LED_RED

	#define TM_DISCO_BUTTON_RCC			RCC_AHB1Periph_GPIOA
	#define TM_DISCO_BUTTON_PORT		GPIOA
	#define TM_DISCO_BUTTON_PIN			GPIO_Pin_0
#elif defined(STM32F4_DISCOVERY)
	#define LED_GREEN					GPIO_Pin_12
	#define LED_ORANGE					GPIO_Pin_13
	#define LED_RED						GPIO_Pin_14
	#define LED_BLUE					GPIO_Pin_15
	#define TM_DISCO_LED_RCC			RCC_AHB1Periph_GPIOD
	#define TM_DISCO_LED_PORT			GPIOD
	#define TM_DISCO_LED_PINS			LED_GREEN | LED_RED | LED_ORANGE | LED_BLUE

	#define TM_DISCO_BUTTON_RCC			RCC_AHB1Periph_GPIOA
	#define TM_DISCO_BUTTON_PORT		GPIOA
	#define TM_DISCO_BUTTON_PIN			GPIO_Pin_0
#elif defined(NUCLEO_F401)
	#define LED_GREEN					GPIO_Pin_5
	#define TM_DISCO_LED_RCC			RCC_AHB1Periph_GPIOA
	#define TM_DISCO_LED_PORT			GPIOA
	#define TM_DISCO_LED_PINS			LED_GREEN

	#define TM_DISCO_BUTTON_RCC			RCC_AHB1Periph_GPIOC
	#define TM_DISCO_BUTTON_PORT		GPIOC
	#define TM_DISCO_BUTTON_PIN			GPIO_Pin_13
#else
	#error "Please select your discovery board"
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
 */
extern void TM_DISCO_LedOn(uint16_t led);

/**
 * Turn off LED
 *
 * Parameters:
 * 	- uint16_t led:
 * 		LED_GREEN
 * 		LED_RED
 */
extern void TM_DISCO_LedOff(uint16_t led);

/**
 * Toggle LED
 *
 * Parameters:
 * 	- uint16_t led:
 * 		LED_GREEN
 * 		LED_RED
 */
extern void TM_DISCO_LedToggle(uint16_t led);

/**
 * Check's if led is on
 *
 * Parameters:
 * 	- uint16_t led:
 * 		LED_GREEN
 * 		LED_RED
 * 
 * Return 1 if turned on, otherwise 0
 */
extern uint8_t TM_DISCO_LedIsOn(uint16_t led);

/**
 * Checks if button is pressed
 *
 * Returns 1 if is pressed, otherwise 0
 */
extern uint8_t TM_DISCO_ButtonPressed(void);

#endif
