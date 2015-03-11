/**
 *	Measure distance with HC-SR04 Ultrasonic distance sensor on STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/08/library-30-measure-distance-with-hc-sr04-and-stm32f4xx
 *	@version 	v1.2
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
 * Version 1.2
 *	- March 11, 2015
 *	- Added support for my new GPIO library
 *
 * Version 1.1
 *	- November 29, 2014
 * 	- Update to fit new delay system 
 *
 * Default pinout
 * 
 * 	HC-SR04			STM32F4xx		Description
 * 
 * 	TRIGGER			PB2				Trigger pin, to start sensor measurement
 * 	ECHO			PB3				Echo pin, read sensor signal
 * 	VCC				-				5V
 * 	GND				-				Ground
 * 
 * If you want to change TRIGGER and ECHO pin, add lines below in defines.h file and edit them
 * 
 *	//Trigger PIN
 *	#define HCSR04_TRIGGER_PORT		GPIOB
 *	#define HCSR04_TRIGGER_PIN		GPIO_PIN_2
 *	
 *	//Echo PIN
 *	#define HCSR04_ECHO_PORT		GPIOB
 *	#define HCSR04_ECHO_PIN			GPIO_PIN_3
 * 
 */
#ifndef TM_HCSR04_H
#define TM_HCSR04_H 120
/**
 * Library dependencies
 * - STM32F4xx
 * - defines.h
 * - TM DELAY
 * - TM GPIO
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_gpio.h"

/* Trigger PIN */
#ifndef HCSR04_TRIGGER_PIN
#define HCSR04_TRIGGER_PORT		GPIOB
#define HCSR04_TRIGGER_PIN		GPIO_PIN_2
#endif

/* Echo PIN */
#ifndef HCSR04_ECHO_PIN
#define HCSR04_ECHO_PORT		GPIOB
#define HCSR04_ECHO_PIN			GPIO_PIN_3
#endif

/* Default timeout pulses */
#ifndef HCSR04_TIMEOUT
#define HCSR04_TIMEOUT			1000000
#endif

/* Pin operations */
#define HCSR04_TRIGGER_LOW		TM_GPIO_SetPinLow(HCSR04_TRIGGER_PORT, HCSR04_TRIGGER_PIN)
#define HCSR04_TRIGGER_HIGH		TM_GPIO_SetPinHigh(HCSR04_TRIGGER_PORT, HCSR04_TRIGGER_PIN)
#define HCSR04_ECHO_CHECK		TM_GPIO_GetInputPinValue(HCSR04_ECHO_PORT, HCSR04_ECHO_PIN)

/* Time to cm */
#define HCSR04_NUMBER			((float)0.0171821)

/**
 * Initialize HC-SR04 sensor
 * 
 * Returns 1 if sensor is ready to use, or 0 if not
 */
extern uint8_t TM_HCSR04_Init(void);

/**
 * Start sensor measurement and read it's data
 * 
 * This function returns -1 if there was an error, or distance in cm if everything is OK
 */
extern float TM_HCSR04_Read(void);

#endif
