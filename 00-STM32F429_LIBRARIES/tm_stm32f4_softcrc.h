/**
 * 16bit CRC Software Generator
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		
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
 */
#ifndef TM_SOFTCRC_
#define TM_SOFTCRC_ 100
/**
 * Library dependencies
 * 
 * None
 */
/**
 * Includes
 */
#include "stm32f4xx.h"

/**
 * Generate CRC from given data
 *
 * Parameters:
 * 	- uint8_t* data: pointer to data array
 * 	- uint8_t length: length of data
 *
 * Returns 16bit CRC generated
 */
extern uint16_t TM_SOFTCRC_Generate(uint8_t* data, uint8_t length);

#endif

