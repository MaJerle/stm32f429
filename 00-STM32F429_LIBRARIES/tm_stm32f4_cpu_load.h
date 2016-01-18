/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/05/library-60-cpu-load-monitor-for-stm32f4xx-devices
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   CPU load monitoring for STM32F4xx
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
#ifndef TM_CPU_LOAD_H
#define TM_CPU_LOAD_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_CPULOAD
 * @brief    CPU load monitoring for STM32F4xx - http://stm32f4-discovery.com/2015/05/library-60-cpu-load-monitor-for-stm32f4xx-devices
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
 - STM32F4xx PWR
 - defines.h
 - TM LOWPOWER
 - TM GENERAL
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_pwr.h"
#include "defines.h"
#include "tm_stm32f4_low_power.h"
#include "tm_stm32f4_general.h"

/**
 * @defgroup TM_CPULOAD_Macros
 * @brief    Library defines
 * @{
 */
/* Macros here */
/**
 * @}
 */
 
/**
 * @defgroup TM_CPULOAD_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  CPU LOAD structure
 */
typedef struct {
	uint32_t WCNT;   /*!< Number of working cycles in one period */
	uint32_t SCNT;   /*!< Number of sleeping cycles in one period */
	float Load;      /*!< CPU load percentage */
	uint8_t Updated; /*!< Set to 1 when new CPU load is calculated */
} TM_CPULOAD_t;

/**
 * @}
 */

/**
 * @defgroup TM_CPULOAD_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes DWT functionality for sleep counter and prepares everything for CPU LOAD measurement
 * @param  *CPU_Load: Pointer to empty @ref TM_CPULOAD_t structure
 * @retval Returns DWT counter status:
 *           - 0: DWT counter has not started, measurement will fail
 *           - > 0: DWT counter has started, everything OK
 */
uint8_t TM_CPULOAD_Init(TM_CPULOAD_t* CPU_Load);

/** 
 * @brief  Goes to low power mode and measures sleeping time and working time using DWT cycle counter
 * @note   You need wake up source, otherwise you might stay forever inside low power mode
 * @param  *CPU_Load: Pointer to @ref TM_CPULOAD_t structure 
 * @param  PowerMode: Select power mode you want to use for measure CPU load. Valid parameters are:
 *            - TM_LOWPOWERMODE_SleepUntilInterrupt: Go to sleep mode with __WFI() instruction
 *            - TM_LOWPOWERMODE_SleepUntilEvent: Go to sleep mode with __WFE() instruction
 * @retval CPU load value updated status.
 *           - 0: CPU load value is not updated, still old results
 *           - > 0: CPU load value is updated
 */
uint8_t TM_CPULOAD_GoToSleepMode(TM_CPULOAD_t* CPU_Load, TM_LOWPOWERMODE_t PowerMode);

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
