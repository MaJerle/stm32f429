/**
 *	SDRAM on STM32F429 Discovery board
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/05/library-14-working-with-sdram-on-stm32f429-discovery/
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
 *	- March 10 2015
 *	- Added support for my new GPIO library
 *
 * Version 1.1
 *	- November 23, 2014
 *	- Instead of calling functions to read/write data to RAM, functions are now defined as macros
 *	- Much faster execution because we don't need to call functions and put data on stack.
 */
#ifndef TM_SDRAM_H
#define TM_SDRAM_H 120
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx FMC
 * - defines.h
 * - TM GPIO
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_fmc.h"
#include "defines.h"
#include "tm_stm32f4_gpio.h"

/* SDRAM start address = FMC peripheral start address */
#define SDRAM_START_ADR			(uint32_t)0xD0000000
/* SDRAM max memory width = 8MB */
#define SDRAM_MEMORY_WIDTH		(uint32_t)0x800000
/* Timeout for SDRAM initialization */
#define SDRAM_TIMEOUT			((uint32_t)0xFFFF) 

/**
 * Initialize SD ram
 *
 * Returns 0 if SDRAM was not properly configured, otherwise 1
 */
extern uint8_t TM_SDRAM_Init(void);

/**
 * Write 8 bit value to SDRAM
 *
 * Parameters:
 * 	- uint32_t address:
 * 		address from 0x000000 to 0x7FFFFF for 8MBytes
 * 	- uint8_t value:
 * 		value to be saved in SDRAM
 */
#define TM_SDRAM_Write8(address, value)		*(__IO uint8_t *) (SDRAM_START_ADR + address) = value

/**
 * Read 8 bit value from SDRAM
 *
 * Parameters:
 * 	- uint32_t address:
 * 		address to read from SDRAM location
 * Return: value stored at desired location
 */
#define TM_SDRAM_Read8(address)				(*(__IO uint8_t *) (SDRAM_START_ADR + address))

/**
 * Write 16 bit value to SDRAM
 *
 * Parameters:
 * 	- uint32_t address:
 * 		address from 0x000000 to 0x7FFFFF for 8MBytes
 * 	- uint16_t value:
 * 		value to be saved in SDRAM
 */
#define TM_SDRAM_Write16(address, value)	*(__IO uint16_t *) (SDRAM_START_ADR + address) = value

/**
 * Read 16 bit value from SDRAM
 *
 * Parameters:
 * 	- uint32_t address:
 * 		address to read from SDRAM location
 * Return: value stored at desired location
 */
#define TM_SDRAM_Read16(address)			(*(__IO uint16_t *) (SDRAM_START_ADR + address))

/**
 * Write 32 bit value to SDRAM
 *
 * Parameters:
 * 	- uint32_t address:
 * 		address from 0x00 to 0x7FFFFF for 8MBytes
 * 	- uint32_t value:
 * 		value to be saved in SDRAM
 */
#define TM_SDRAM_Write32(address, value)	*(__IO uint32_t *) (SDRAM_START_ADR + address) = value

/**
 * Read 32 bit value from SDRAM
 *
 * Parameters:
 * 	- uint32_t address:
 * 		address to read from SDRAM location
 * Return: value stored at desired location
 */
#define TM_SDRAM_Read32(address)			(*(__IO uint32_t *) (SDRAM_START_ADR + address))

#endif
