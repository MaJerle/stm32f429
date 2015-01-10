/**
 *	OTP (One-Time Programmable) flash section library for STM32F4xx
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2015/01/library-49-one-time-programmable-otp-bytes-on-stm32f4xx
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
 * STM32F4xx devices have 512Bytes of OTP (One-Time Programming) Memory.
 * You can program it only once, and then you are unable to rewrite data at
 * already programmed OTP location.
 * 
 * OTP memory has 16 data block, of each has 32 bytes for user data.
 * It has also 17th block, where are stored bytes which represents if data
 * block is locked or not.
 */
#ifndef TM_OTP_H
#define TM_OTP_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx FLASH
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_flash.h"
#include "defines.h"

/* OTP start & lock addresses */
#define OTP_START_ADDR		(0x1FFF7800)
#define OTP_LOCK_ADDR		(0x1FFF7A00)
	
/* OTP size definitions */
#define OTP_BLOCKS			16
#define OTP_BYTES_IN_BLOCK	32
#define OTP_SIZE			(OTP_BLOCKS * OTP_BYTES_IN_BLOCK)

/**
 * OTP Result enumeration
 *
 * Parameters:
 *	- TM_OTP_Result_Ok:
 *		Everything OK
 *	- TM_OTP_Result_Error:
 *		An error occured
 *		This is returned when you try to write data to location which does not exists,
 *		or if you try to write data to locked block
 */
typedef enum {
	TM_OTP_Result_Ok = 0,
	TM_OTP_Result_Error
} TM_OTP_Result_t;

/**
 * Write one-time data to specific block and specific byte in this block
 * 
 * Note:
 *  - You can only ONCE write data at desired byte in specific block,
 *    if you will try to do it more times, you can have broken data at this location.
 * 
 * Parameters:
 * 	- uint8_t block:
 * 		OTP block number, 0 to 15 is allowed
 * 	- uint8_t byte:
 * 		OTP byte inside one block, 0 to 31 is allowed
 * 	- uint8_t data:
 * 		Data to be written to OTP memory
 * 
 * Member of TM_OTP_Result_t enumeration is returned
 */
extern TM_OTP_Result_t TM_OTP_Write(uint8_t block, uint8_t byte, uint8_t data);

/**
 * Read data from specific block and specific byte in this block
 * 
 * You can read data unlimited times from locations
 * 
 * Parameters:
 * 	- uint8_t block:
 * 		OTP block number, 0 to 15 is allowed
 * 	- uint8_t byte:
 * 		OTP byte inside one block, 0 to 31 is allowed
 * 
 * Returns value at specific block and byte location, or 0 if location is invalid
 */
extern uint8_t TM_OTP_Read(uint8_t block, uint8_t byte);

/**
 * Lock entire block to prevent future programming inside
 * 
 * Note:
 * 	- When you lock your block, then you are not able to program it anymore.
 * 	  Even, if it is totally empty. You can't unlock it back!
 * 
 * Parameters:
 * 	- uint8_t block:
 * 		OTP block number, 0 to 15 is allowed
 * 
 * Member of TM_OTP_Result_t enumeration is returned
 */
extern TM_OTP_Result_t TM_OTP_BlockLock(uint8_t block);

/**
 * Check if block is locked or not
 * 
 * Parameters:
 * 	- uint8_t block:
 * 		OTP block number, 0 to 15 is allowed
 * 
 * Returns 1 if locked or 0 if not
 */
#define TM_OTP_BlockLocked(block)	((*(__IO uint8_t *) (OTP_LOCK_ADDR + block)) == 0x00 ? 1 : 0)

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
