/**
 *	CRC Library for STM32F4xx
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2015/01/library-47-crc-module-on-stm32f4
 *	@version 	v1.1
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2015
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
 * Library uses hardware CRC unit in STM32F4xx device
 */
#ifndef TM_CRC_H
#define TM_CRC_H 110

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

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
 * Initialize CRC peripheral
 *
 * No return
 */
extern void TM_CRC_Init(void);

/**
 * Calculate 32-bit CRC value from 8-bit input array
 *
 * Parameters:
 * 	- uint8_t* arr:
 * 		Pointer to 8-bit data array for calculation
 * 	- uint16_t count:
 * 		Number of elements in array for calculation
 *	- uint8_t reset:
 *		0: CRC unit will not be reset before new calculations will happen and will use 
 *		   previous data to continue
 *		1: CRC unit is set to 0 before first calculation
 *
 * 32-bit CRC number is returned
 */
extern uint32_t TM_CRC_Calculate8(uint8_t* arr, uint16_t count, uint8_t reset);

/**
 * Calculate 32-bit CRC value from 16-bit input array
 *
 * Parameters:
 * 	- uint16_t* arr:
 * 		Pointer to 16-bit data array for calculation
 * 	- uint16_t count:
 * 		Number of elements in array for calculation
 *	- uint8_t reset:
 *		0: CRC unit will not be reset before new calculations will happen and will use 
 *		   previous data to continue
 *		1: CRC unit is set to 0 before first calculation
 *
 * 32-bit CRC number is returned
 */
extern uint32_t TM_CRC_Calculate16(uint16_t* arr, uint16_t count, uint8_t reset);

/**
 * Calculate 32-bit CRC value from 8-bit input array
 *
 * Parameters:
 * 	- uint32_t* arr:
 * 		Pointer to 32-bit data array for calculation
 * 	- uint16_t count:
 * 		Number of elements in array for calculation
 *	- uint8_t reset:
 *		0: CRC unit will not be reset before new calculations will happen and will use 
 *		   previous data to continue
 *		1: CRC unit is set to 0 before first calculation
 *
 * 32-bit CRC number is returned
 */
extern uint32_t TM_CRC_Calculate32(uint32_t* arr, uint16_t count, uint8_t reset);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
