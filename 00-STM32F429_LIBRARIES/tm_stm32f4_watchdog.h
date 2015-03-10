/**
 * Independent watchdog timer for STM32F4xx MCUs
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/07/library-20-independent-watchdog-timer-on-stm32f4xx/
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
 *	- Support for STD/HAL drivers
 */
#ifndef TM_WATCHDOG_H
#define TM_WATCHDOG_H 110
/**
 * Library dependencies
 * - STM32F4xx
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "defines.h"

/**
 * Watchdog timeout settings
 */
typedef enum {
	TM_WATCHDOG_Timeout_5ms,	//System reset called every 5ms
	TM_WATCHDOG_Timeout_10ms,	//System reset called every 10ms
	TM_WATCHDOG_Timeout_15ms,	//System reset called every 15ms
	TM_WATCHDOG_Timeout_30ms,	//System reset called every 30ms
	TM_WATCHDOG_Timeout_60ms,	//System reset called every 60ms
	TM_WATCHDOG_Timeout_120ms,	//System reset called every 120ms
	TM_WATCHDOG_Timeout_250ms,	//System reset called every 250ms
	TM_WATCHDOG_Timeout_500ms,	//System reset called every 500ms
	TM_WATCHDOG_Timeout_1s,		//System reset called every 1s
	TM_WATCHDOG_Timeout_2s,		//System reset called every 2s
	TM_WATCHDOG_Timeout_4s		//System reset called every 4s
} TM_WATCHDOG_Timeout_t;

/**
 * Initialize Watchdog timer
 *
 * Parameters:
 *	- TM_WATCHDOG_Timeout_t timeout:
 *		Select time when watchdog reset accur if Watchdog timer is not reset before
 *
 * Returns 1 if system was reset by Watchdog, otherwise 0
 */
uint8_t TM_WATCHDOG_Init(TM_WATCHDOG_Timeout_t timeout);

/**
 * Reset Watchdog timer
 *
 *
 */
void TM_WATCHDOG_Reset(void);

#endif

