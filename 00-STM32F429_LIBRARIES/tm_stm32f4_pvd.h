/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   PVD Voltage detector for STM32F4xx
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
#ifndef TM_PVD_H
#define TM_PVD_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_PVD
 * @brief    PVD - Power Voltage Detector for STM32F4xx
 * @{
 *
 * PVD can detect voltage changes in your system. It features 8 different voltage levels between 2 and 3 volts. For detailed voltages, check datasheet.
 *
 * According to EXTI line 16 settings, PVD will trigger interrupts for rising, falling or both edges.
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
 - defines.h
@endverbatim
 */

#include "stm32f4xx.h"
#include "defines.h"

/**
 * @defgroup TM_PVD_Macros
 * @brief    Library defines
 * @{
 */

/* NVIC preemption priority */
#ifndef PVD_NVIC_PRIORITY
#define PVD_NVIC_PRIORITY      0x04
#endif

/* NVIC subpriority */
#ifndef PVD_NVIC_SUBPRIORITY
#define PVD_NVIC_SUBPRIORITY   0x00
#endif

/**
 * @}
 */
 
/**
 * @defgroup TM_PVD_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  PVD Triggers for interrupt
 */
typedef enum {
	TM_PVD_Trigger_Rising = 0x00,  /*!< PVD will trigger interrupt when voltage rises above treshold */
	TM_PVD_Trigger_Falling = 0x01, /*!< PVD will trigger interrupt when voltage falls below treshold */
	TM_PVD_Trigger_Rising_Falling  /*!< PVD will trigger interrupt when voltage rises or falls above/below treshold */
} TM_PVD_Trigger_t;

/**
 * @brief  PVD levels for interrupt triggering
 * @note   Check datasheets for proper values for voltages
 */
typedef enum {
	TM_PVD_Level_0 = 0x00, /*!< PVD level 0 is used as treshold value */
	TM_PVD_Level_1 = 0x01, /*!< PVD level 1 is used as treshold value */
	TM_PVD_Level_2 = 0x02, /*!< PVD level 2 is used as treshold value */
	TM_PVD_Level_3 = 0x03, /*!< PVD level 3 is used as treshold value */
	TM_PVD_Level_4 = 0x04, /*!< PVD level 4 is used as treshold value */
	TM_PVD_Level_5 = 0x05, /*!< PVD level 5 is used as treshold value */
	TM_PVD_Level_6 = 0x06, /*!< PVD level 6 is used as treshold value */
	TM_PVD_Level_7 = 0x07  /*!< PVD level 7 is used as treshold value */
} TM_PVD_Level_t;

/**
 * @}
 */

/**
 * @defgroup TM_PVD_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Enables PVD feature for interrupt generation when voltage changes are detected
 * @param  Level: Level voltage when interrupt will happen. This parameter can be a value of @ref TM_PVD_Level_t enumeration
 * @param  Trigger: Trigger option when interrupt will be generated. This parameter can be a value of @ref TM_PVD_Trigger_t enumeration
 * @retval None
 */
void TM_PVD_Enable(TM_PVD_Level_t Level, TM_PVD_Trigger_t Trigger);

/**
 * @brief  Disables PVD feature
 * @param  None
 * @retval None
 */
void TM_PVD_Disable(void);

/**
 * @brief  Callback for handling PVD interrupts
 * @param  status: Status about voltage detected
 *            - 0: Voltage is above treshold level
 *            - > 0: Voltage is below treshold value
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_PVD_Handler(uint8_t status);

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
