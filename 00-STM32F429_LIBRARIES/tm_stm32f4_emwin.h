/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/01/library-50-stemwin-for-stm32f429-discovery
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   STemWin implementation for STM32F429-Discovery
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
#ifndef TM_EMWIN_H
#define TM_EMWIN_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_EMWIN
 * @brief    STemWin implementation for STM32F429-Discovery - http://stm32f4-discovery.com/2015/01/library-50-stemwin-stm32f429-discovery/
 * @{
 *
 * Library is only low-layer implementation between STemWin GUI and STM32F429-Discovery board.
 *
 * To know how to write text and other stuff, you should take a look at emwin manual from segger.
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
 - STM32F4xx GPIO
 - STM32F4xx DMA2D
 - STM32F4xx LTDC
 - STM32F4xx FMC
 - STM32F4xx I2C
 - STM32F4xx SPI
 - defines.h
 - TM ILI9341 LTDC
 - TM FONTS
 - TM I2C
 - TM SDRAM
 - TM SPI
@endverbatim
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "defines.h"
#include "tm_stm32f4_sdram.h"
#include "tm_stm32f4_ili9341_ltdc.h"
#include "tm_stm32f4_stmpe811.h"
#include "GUI.h"

/**
 * @defgroup TM_EMWIN_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief If you want to rotate feature for LCD, set this define to 1
 */
#ifndef TM_EMWIN_ROTATE_LCD
#define TM_EMWIN_ROTATE_LCD				0
#endif

/**
 * @brief Number of milliseconds between 2 touch checks
 */
#ifndef EMWIN_UPDATE_TOUCH_MILLIS
#define EMWIN_UPDATE_TOUCH_MILLIS		50
#endif

/**
 * @}
 */
 
/**
 * @defgroup TM_EMWIN_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  EMWIN Result enumeration	
 */
typedef enum {
	TM_EMWIN_Result_Ok = 0,     /*!< Everything OK */
	TM_EMWIN_Result_Error,      /*!< An error occurred */
	TM_EMWIN_Result_TouchError, /*!< Error with touch controller */
	TM_EMWIN_Result_GUIError    /*!< Error with emWin GUI */
} TM_EMWIN_Result_t;

/**
 * @brief  EMWIN Rotation enumeration
 */
typedef enum {
	TM_EMWIN_Rotate_0 = 0, /*!< Rotate LCD for 0 degrees, default state*/
	TM_EMWIN_Rotate_90,    /*!< Rotate LCD 90 degrees */
	TM_EMWIN_Rotate_180,   /*!< Rotate LCD 180 degrees */
	TM_EMWIN_Rotate_270,   /*!< Rotate LCD 170 degrees */
} TM_EMWIN_Rotate_t;

/**
 * @}
 */

/**
 * @defgroup TM_EMWIN_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes emWin peripheral
 * @note   This function initialize LCD, SDRAM, STMPE811 touch and GUI for STM32F429-Discovery board
 * @retval Member of @ref TM_EMWIN_Result_t
 */
TM_EMWIN_Result_t TM_EMWIN_Init(void);

/**
 * @brief  Rotates LCD
 * @note   It does not have any sense, if you have disabled define TM_EMWIN_ROTATE_LCD.
 * 
 * By default, rotation is disabled for memory purpose.
 *
 * To rotate LCD you need additional RAM, but it is available on F429-Discovery board
 * @param  rotation: Rotate direction. This parameter can be a value of @ref TM_EMWIN_Result_t enumeration
 * @retval Member of @ref TM_EMWIN_Result_t
 */
TM_EMWIN_Result_t TM_EMWIN_Rotate(TM_EMWIN_Rotate_t rotation);

/**
 * @brief  Enables memory feature for EMWIN
 * @note   When you enable memory, then all drawings for EMWIN are stored on LCD layer 1,
 *            but layer 2 is shown to user. You have to use @ref TM_EMWIN_Exec() to update data from LCD layer 1 to layer 2
 *         This is used to avoid flickering system when doing a lot of redrawing, for example updating a big graph very fast 
 * @param  None
 * @retval None
 */
void TM_EMWIN_MemoryEnable(void);

/**
 * @brief  Disables memory feature for EMWIN
 * @param  None
 * @retval None
 */
void TM_EMWIN_MemoryDisable(void);

/**
 * @brief  Execute EMWIN pending tasks 
 * @note   This function was designed to be used in case memory is enabled using @ref TM_EMWIN_MemoryEnable function.
 *            You can also use it (and it is recommended) in case you don't have memory enabled
 * @param  None
 * @retval Return values of GUI_Exec() function from EMWIN
 */
uint32_t TM_EMWIN_Exec(void);

/**
 * @brief  Updates touch
 * @brief  It must be called periodically every 1ms.
 *
 * It makes touch controller check every number of ms as defined in EMWIN_UPDATE_TOUCH_MILLIS define
 * @param None
 * @retval Member of @ref TM_EMWIN_Result_t
 */
TM_EMWIN_Result_t TM_EMWIN_UpdateTouch(void);

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
