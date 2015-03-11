/**
 *	MCO Output for STM32F4
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/10/library-40-output-clocks-stm32f4/
 *	@version 	v1.1
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
 * Version 1.1
 *	- March 11, 2015
 *	- Support for my new GPIO library
 *
 * This library allows you to output different frequencies.
 * They are split into 2 different output sections
 *
 * MCO1 output
 * 	- Pin PA8
 * 	- HSI: High Speed Internal clock, 16MHz RC oscillator
 * 	- HSE: High Speed External clock, external crystal or user clock
 * 	- LSE: External 32738Hz low-speed oscillator or ceramic resonator
 * 	- PLLCLK: Output from PLL
 *
 * MCO2 output
 * 	- Pin PC9
 * 	- SYSCLK: Output system core clock
 * 	- PLLI2SCLK: Accurate clock for high-quality audio performance in I2S and SAI interfaces
 * 	- HSE: High Speed External clock, external crystal or user clock
 * 	- PLLCLK: Output from PLL
 */
#ifndef TM_MCOOUTPUT_H
#define TM_MCOOUTPUT_H 110

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - defines.h
 * - TM GPIO
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "defines.h"
#include "tm_stm32f4_gpio.h"

/**
 * Prescaler value for output clock
 *
 * Parameters:
 * 	- TM_MCOOUTPUT_Prescaler_1:
 * 		Output original value of clock on MCO pin
 * 	- TM_MCOOUTPUT_Prescaler_2:
 * 		Output clock is orignial / 2
 * 	- TM_MCOOUTPUT_Prescaler_3:
 * 		Output clock is original / 3
 * 	- TM_MCOOUTPUT_Prescaler_4:
 * 		Output clock is original / 4
 * 	- TM_MCOOUTPUT_Prescaler_5:
 * 		Output clock is original / 5
 */
typedef enum {
	TM_MCOOUTPUT_Prescaler_1,
	TM_MCOOUTPUT_Prescaler_2,
	TM_MCOOUTPUT_Prescaler_3,
	TM_MCOOUTPUT_Prescaler_4,
	TM_MCOOUTPUT_Prescaler_5
} TM_MCOOUTPUT_Prescaler_t;

/**
 * Possible output clocks on MCO1 pin
 *
 * Parameters:
 *	- TM_MCOOUTPUT1_Source_HSI:
 *		High Speed Internal clock, 16MHz RC oscillator
 *	- TM_MCOOUTPUT1_Source_HSE:
 *		High Speed External clock, external crystal or user clock
 *	- TM_MCOOUTPUT1_Source_LSE:
 *		External 32738Hz low-speed oscillator or ceramic resonator
 *	- TM_MCOOUTPUT1_Source_PLLCLK:
 *		Output from PLL
 */
typedef enum {
	TM_MCOOUTPUT1_Source_HSI,
	TM_MCOOUTPUT1_Source_HSE,
	TM_MCOOUTPUT1_Source_LSE,
	TM_MCOOUTPUT1_Source_PLLCLK
} TM_MCOOUTPUT1_Source_t;

/**
 * Possible output clocks on MCO2 pin
 *
 * Parameters:
 * 	- TM_MCOOUTPUT2_Source_SYSCLK:
 * 		System core clock
 * 	- TM_MCOOUTPUT2_Source_PLLI2SCLK:
 * 		Accurate clock for high-quality audio performance in I2S and SAI interfaces
 * 	- TM_MCOOUTPUT2_Source_HSE:
 * 		High Speed External clock, external crystal or user clock
* 	- TM_MCOOUTPUT2_Source_PLLCLK:
 * 		Output from PLL
 *
 */
typedef enum {
	TM_MCOOUTPUT2_Source_SYSCLK,
	TM_MCOOUTPUT2_Source_PLLI2SCLK,
	TM_MCOOUTPUT2_Source_HSE,
	TM_MCOOUTPUT2_Source_PLLCLK
} TM_MCOOUTPUT2_Source_t;

/**
 * Initialize and prepare MCO1 pin to output clock
 *
 * No return
 */
extern void TM_MCOOUTPUT_InitMCO1(void);

/**
 * Set output for MCO1 pin
 *
 * Parameters:
 * 	- TM_MCOOUTPUT1_Source_t Source:
 * 		Specific clock source
 * 	- TM_MCOOUTPUT_Prescaler_t Prescaler:
 * 		Prescaler used for clock
 *
 * No return
 */
extern void TM_MCOOUTPUT_SetOutput1(TM_MCOOUTPUT1_Source_t Source, TM_MCOOUTPUT_Prescaler_t Prescaler);

/**
 * Initialize and prepare MCO2 pin to output clock
 *
 * No return
 */
extern void TM_MCOOUTPUT_InitMCO2(void);
 
/**
 * Set output for MCO2 pin
 *
 * Parameters:
 * 	- TM_MCOOUTPUT2_Source_t Source:
 *		Specific clock source for MCO2
 *	- TM_MCOOUTPUT_Prescaler_t Prescaler:
 *		Prescaler used for clock
 *
 * No return
 */
extern void TM_MCOOUTPUT_SetOutput2(TM_MCOOUTPUT2_Source_t Source, TM_MCOOUTPUT_Prescaler_t Prescaler);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
