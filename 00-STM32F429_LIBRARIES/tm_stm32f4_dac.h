/**
 * 	DAC library for STM32F4xx
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/04/library-07-da-converter-on-stm32f4xx/
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
 *	- March 12, 2015
 *	- Support for my new GPIO library
 *
 * This library provides 12bit digital to analog output, values from 0 to 4095
 *	
 * Pinout
 *
 * 	DAC	channel		STM32F4xx
 * 		
 * 	DAC1			PA4
 * 	DAC2			PA5
 */
#ifndef TM_DAC_H
#define TM_DAC_H 110
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx DAC
 * - defines.h
 * - TM GPIO
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_dac.h"
#include "defines.h"
#include "tm_stm32f4_gpio.h"

/**
 * Select which DAC channel
 */
typedef enum {
	TM_DAC1,
	TM_DAC2
} TM_DAC_Channel_t;

/**
 * Initialize DAC channel and it's pin
 *
 * Parameters:
 * 	- TM_DAC_Channel_t DACx:
 * 		TM_DAC1, TM_DAC2
 */
extern void TM_DAC_Init(TM_DAC_Channel_t DACx);

/**
 * Set analog value to ADCx
 *
 * Parameters:
 * 	- TM_DAC_Channel_t DACx:
 * 		- TM_DAC1, TM_DAC2
 * 	- uint16_t value
 * 		12Bit unsigned value for 12bit DAC value
 */
extern void TM_DAC_SetValue(TM_DAC_Channel_t DACx, uint16_t value);

#endif
