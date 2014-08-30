/**
 * Rotary encoder library for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/08/library-26-rotary-encoder-stm32f4
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
 *	This library supports one rotary encoder.
 *
 *	Default pinout
 *
 *	ROTARY ENCODER		STM32F4XX		DESCRIPTION
 *
 *	A					PD0				A pin, connected to interrupt pin
 *	B					PD1				B pin, connected to any input pin
 *	C					GND				Common pin, connect to ground
 *
 *	By default, if you turn encoder CW, you will increase counter,
 *	if CCW, you will decrease counter.
 *	But there is option to flip this setting. If you want to enable it,
 *	add line below in your defines.h file:
 *
 *	#define TM_RE_CHANGE_ROTATION
 *
 *	Check link above for more settings
 *
 */
#ifndef TM_RE_H
#define TM_RE_H 100
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx EXTI
 * - STM32F4xx SYSCFG
 * - misc.h
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"
#include "defines.h"

/* Rotary encoder A pin */
/* Connected to interrupt channel */
#ifndef TM_RE_A_PIN
#define TM_RE_A_RCC					RCC_AHB1Periph_GPIOD			
#define TM_RE_A_PORT				GPIOD
#define TM_RE_A_PIN					GPIO_Pin_0
#define TM_RE_A_EXTI_LINE			EXTI_Line0
#define TM_RE_A_EXTIPINSOURCE		EXTI_PinSource0
#define TM_RE_A_EXTIPORTSOURCE		EXTI_PortSourceGPIOD
#define TM_RE_A_NVIC				EXTI0_IRQn
#define TM_RE_A_DEFAULT
#endif

/* Rotary encoder B pin */
#ifndef TM_RE_B_PIN
#define TM_RE_B_RCC					RCC_AHB1Periph_GPIOD			
#define TM_RE_B_PORT				GPIOD
#define TM_RE_B_PIN					GPIO_Pin_1
#endif

/* Default NVIC priority */
#ifndef RE_NVIC_PRIORITY
#define RE_NVIC_PRIORITY			0x03
#endif

/* Default NVIC sub priority */
#ifndef RE_NVIC_SUBPRIORITY
#define RE_NVIC_SUBPRIORITY			0x00
#endif

/**
 * Used for struct settings
 *
 * Parameters:
 * 	- TM_RE_Rotate_Increment
 * 		Encoder was incremented
 * 	- TM_RE_Rotate_Decrement
 * 		Encoder was decremented
 * 	- TM_RE_Rotate_Nothing
 * 		Encoder stop at it was before
 */
typedef enum {
	TM_RE_Rotate_Increment,
	TM_RE_Rotate_Decrement,
	TM_RE_Rotate_Nothing
} TM_RE_Rotate_t;

/**
 * Main struct
 *
 * Parameters:
 * 	- int32_t Absolute
 * 		Absolute value of rotation from beginning of program
 *  - int32_t Diff
 *  	Rotary difference from last check
 *  - TM_RE_Rotate_t Rotation
 *  	It allows you to check if rotary was increased, decreased or nothing
 */
typedef struct {
	int32_t Absolute;				/* Absolute rotation from beginning */
	int32_t Diff; 					/* Rotary difference from last check */
	TM_RE_Rotate_t Rotation;		/* Increment, Decrement or nothing */
} TM_RE_t;


/**
 * Prepare Rotary Encoder to work
 *
 * Parameters:
 * 	- TM_RE_t* data
 * 		Pointer to TM_RE_t struct
 */
extern void TM_RE_Init(TM_RE_t* data);

/**
 * Check and get new values of rotary encoder
 *
 * Parameters:
 * 	- TM_RE_t* data
 * 		Pointer to TM_RE_t struct
 *
 * Parameter of enumeration typedef TM_RE_Rotate_t is returned
 */
extern TM_RE_Rotate_t TM_RE_Get(TM_RE_t* data);

/**
 * Process function.
 * This function have to be called inside your interrupt handler.
 *
 *
 */
extern void TM_RE_Process(void);

#endif


