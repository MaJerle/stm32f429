/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/01/library-47-crc-module-on-stm32f4
 * @version v1.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   CRC for STM32F4xx devices
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
#ifndef TM_CRC_H
#define TM_CRC_H 110

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_CRC
 * @brief    CRC library for STM32F4xx devices - http://stm32f4-discovery.com/2015/01/library-47-crc-module-on-stm32f4
 * @{
 *
 * Library uses hardware CRC unit in STM32F4xx device
 *
 * \par Changelog
 *
@verbatim
 Version 1.1
  - March 10, 2015
  - Added support for STD/HAL drivers

 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - defines.h
@endverbatim
 */
#include "stm32f4xx.h"
#include "defines.h"

/**
 * @defgroup TM_CRC_Functions
 * @brief    Library Functions
 * @{
 */

/** 
 * @brief  Initializes and enables CRC peripheral
 * @param  None
 * @retval None
 */
void TM_CRC_Init(void);

/** 
 * @brief  De initializes and disable CRC peripheral
 * @param  None
 * @retval None
 */
void TM_CRC_DeInit(void);

/**
 * @brief  Calculates 32-bit CRC value from 8-bit input array
 * @param  *arr: Pointer to 8-bit data array for CRC calculation
 * @param  count: Number of elements in array for calculation
 * @param  reset: Reset CRC peripheral to 0 state before starting new calculations
 *            - 0: CRC unit will not be reset before new calculations will happen and will use previous data to continue
 *            - > 0: CRC unit is set to 0 before first calculation
 * @retval 32-bit CRC calculated number
 */
uint32_t TM_CRC_Calculate8(uint8_t* arr, uint32_t count, uint8_t reset);

/**
 * @brief  Calculates 32-bit CRC value from 16-bit input array
 * @param  *arr: Pointer to 16-bit data array for CRC calculation
 * @param  count: Number of elements in array for calculation
 * @param  reset: Reset CRC peripheral to 0 state before starting new calculations
 *            - 0: CRC unit will not be reset before new calculations will happen and will use previous data to continue
 *            - > 0: CRC unit is set to 0 before first calculation
 * @retval 32-bit CRC calculated number
 */
uint32_t TM_CRC_Calculate16(uint16_t* arr, uint32_t count, uint8_t reset);

/**
 * @brief  Calculates 32-bit CRC value from 32-bit input array
 * @param  *arr: Pointer to 32-bit data array for CRC calculation
 * @param  count: Number of elements in array for calculation
 * @param  reset: Reset CRC peripheral to 0 state before starting new calculations
 *            - 0: CRC unit will not be reset before new calculations will happen and will use previous data to continue
 *            - > 0: CRC unit is set to 0 before first calculation
 * @retval 32-bit CRC calculated number
 */
uint32_t TM_CRC_Calculate32(uint32_t* arr, uint32_t count, uint8_t reset);

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
