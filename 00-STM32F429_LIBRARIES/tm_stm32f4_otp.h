/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/01/library-49-one-time-programmable-otp-bytes-on-stm32f4xx
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   OTP (One-Time Programmable) flash section library for STM32F4xx
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
#ifndef TM_OTP_H
#define TM_OTP_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_OTP
 * @brief    OTP (One-Time Programmable) flash section library for STM32F4xx - http://stm32f4-discovery.com/2015/01/library-49-one-time-programmable-otp-bytes-on-stm32f4xx
 * @{
 *
 * \par Changelog
 *
@verbatim
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - STM32F4xx FLASH
 - defines.h
@endverbatim
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_flash.h"
#include "defines.h"

/**
 * @defgroup TM_OTP_Macros
 * @brief    Library defines
 * @{
 */
 
/**
 * @brief  OTP memory start address
 */
#define OTP_START_ADDR		(0x1FFF7800)

/**
 * @brief  OTP memory lock address
 */
#define OTP_LOCK_ADDR		(0x1FFF7A00)
	
/**
 * @brief  Number of OTP blocks 
 */
#define OTP_BLOCKS			16

/**
 * @brief  Number of bytes in one block
 */
#define OTP_BYTES_IN_BLOCK	32

/**
 * @brief  Number of all OTP bytes
 */
#define OTP_SIZE			(OTP_BLOCKS * OTP_BYTES_IN_BLOCK)

/**
 * @}
 */
 
/**
 * @defgroup TM_OTP_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Result enumeration
 */
typedef enum {
    TM_OTP_Result_Ok = 0,  /*!< Everything OK */
    TM_OTP_Result_Error    /*!< An error occurred.
	                            This is returned when you try to write data to location which does not exists,
                                or if you try to write data to locked block */
} TM_OTP_Result_t;

/**
 * @}
 */

/**
 * @defgroup TM_OTP_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Writes one-time data to specific block and specific byte in this block
 * @note   You can only ONCE write data at desired byte in specific block, if you will try to do it more times, you can have broken data at this location.
 * @param  block: OTP block number, 0 to 15 is allowed
 * @param  byte: OTP byte inside one block, 0 to 31 is allowed
 * @param  data: Data to be written to OTP memory
 * @retval Member of @ref TM_OTP_Result_t enumeration 
 */
TM_OTP_Result_t TM_OTP_Write(uint8_t block, uint8_t byte, uint8_t data);

/**
 * @brief  Reads data from specific block and specific byte in this block
 * @note   You can read data unlimited times from locations
 * @param  block: OTP block number, 0 to 15 is allowed
 * @param  byte: OTP byte inside one block, 0 to 31 is allowed
 * @retval Value at specific block and byte location, or 0 if location is invalid
 */
uint8_t TM_OTP_Read(uint8_t block, uint8_t byte);

/**
 * @brief  Locks entire block to prevent future programming inside
 * @note   When you lock your block, then you are not able to program it anymore.
 * 	       Even, if it is totally empty. You can't unlock it back!
 * @param  block: OTP block number, 0 to 15 is allowed
 * @retval Member of @ref TM_OTP_Result_t enumeration
 */
TM_OTP_Result_t TM_OTP_BlockLock(uint8_t block);

/**
 * @brief  Checks if block is locked or not
 * @param  block: OTP block number, 0 to 15 is allowed
 * @retval Block lock status
 *            - 0: Block is not locked
 *            - > 0: Block locked
 */
#define TM_OTP_BlockLocked(block)	((*(__IO uint8_t *) (OTP_LOCK_ADDR + block)) == 0x00 ? 1 : 0)

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
