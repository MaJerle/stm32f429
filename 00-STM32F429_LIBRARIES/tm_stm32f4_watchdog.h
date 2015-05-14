/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/07/library-20-independent-watchdog-timer-on-stm32f4xx/
 * @version v1.2
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Independent watchdog timer for STM32F4xx MCUs
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2014
    
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
#ifndef TM_WATCHDOG_H
#define TM_WATCHDOG_H 120
/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_WATCHDOG
 * @brief    Independent Watchdog timer for STM32F4xx - http://stm32f4-discovery.com/2014/07/library-20-independent-watchdog-timer-on-stm32f4xx/
 * @{
 *
 * \par Changelog
 *
@verbatim
 Version 1.2
  - March 25, 2015
  - Added support for more timeouts
  
 Version 1.1
  - March 10, 2015
  - Support for STD/HAL drivers

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
 * @defgroup TM_WATCHDOG_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief Watchdog timeout settings
 */
typedef enum {
	TM_WATCHDOG_Timeout_5ms = 0x00,   /*!< System reset called every 5ms */
	TM_WATCHDOG_Timeout_10ms = 0x01,  /*!< System reset called every 10ms */
	TM_WATCHDOG_Timeout_15ms = 0x02,  /*!< System reset called every 15ms */
	TM_WATCHDOG_Timeout_30ms = 0x03,  /*!< System reset called every 30ms */
	TM_WATCHDOG_Timeout_60ms = 0x04,  /*!< System reset called every 60ms */
	TM_WATCHDOG_Timeout_120ms = 0x05, /*!< System reset called every 120ms */
	TM_WATCHDOG_Timeout_250ms = 0x06, /*!< System reset called every 250ms */
	TM_WATCHDOG_Timeout_500ms = 0x07, /*!< System reset called every 500ms */
	TM_WATCHDOG_Timeout_1s = 0x08,    /*!< System reset called every 1s */
	TM_WATCHDOG_Timeout_2s = 0x09,    /*!< System reset called every 2s */
	TM_WATCHDOG_Timeout_4s = 0x0A,    /*!< System reset called every 4s */
	TM_WATCHDOG_Timeout_8s = 0x0B,    /*!< System reset called every 8s */
	TM_WATCHDOG_Timeout_16s = 0x0C,   /*!< System reset called every 16s */
	TM_WATCHDOG_Timeout_32s = 0x0D    /*!< System reset called every 32s. This is maximum value allowed with IWDG timer */
} TM_WATCHDOG_Timeout_t;

/**
 * @}
 */

/**
 * @defgroup TM_WATCHDOG_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initialize Watchdog timer
 * @note   After you initialize it, you can't disable it unless reset occur.
 * @param  timeout. Timeout value when watchdog reset happen if not reset by user.
 *            This parameter can be a value of @ref TM_WATCHDOG_Timeout_t enumeration
 * @retval Value if system was reset because of watchdog timer
 *            - 1: Reset happen because of watchdog
 *            - 0: Otherwise
 */
uint8_t TM_WATCHDOG_Init(TM_WATCHDOG_Timeout_t timeout);

/**
 * @brief  Reset Watchdog timer
 * @note   This function have to be called periodically to reset watchdog timer, or your system will reset
 * @param  None
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_WATCHDOG_Reset()     (IWDG->KR = 0xAAAA)

/**
 * @}
 */
 
/**
 * @}
 */

/**
 * @}
 */

#endif

