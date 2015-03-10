/**
 * Random number generator library for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/07/library-22-true-random-number-generator-stm32f4xx/
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
 *	- March 10, 2015
 *	- Added support for STD/HAL drivers
 *
 *	This peripheral provides you a true random 32bit number
 */
#ifndef TM_RNG_H
#define TM_RNG_H 110
/**
 * Dependencies
 * - STM32F4xx
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "defines.h"

/**
 * Initialize RNG
 *
 *
 */
extern void TM_RNG_Init(void);

/**
 * Get 32bit random number
 *
 * Returns 32bit random number
 */
extern uint32_t TM_RNG_Get(void);

#endif

