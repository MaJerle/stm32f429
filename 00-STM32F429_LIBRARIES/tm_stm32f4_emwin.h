/**
 *	STemWin implementation for STM32F429-Discovery
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2015/01/library-50-stemwin-for-stm32f429-discovery
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
#ifndef TM_EMWIN_H
#define TM_EMWIN_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx DMA2D
 * - STM32F4xx LTDC
 * - STM32F4xx FMC
 * - STM32F4xx I2C
 * - STM32F4xx SPI
 * - defines.h
 * - TM ILI9341 LTDC
 * - TM FONTS
 * - TM I2C
 * - TM SDRAM
 * - TM SPI
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "defines.h"
#include "tm_stm32f4_sdram.h"
#include "tm_stm32f4_ili9341_ltdc.h"
#include "tm_stm32f4_stmpe811.h"
#include "GUI.h"

/* If you want to rotate feature for LCD, set this define to 1 */
#ifndef TM_EMWIN_ROTATE_LCD
#define TM_EMWIN_ROTATE_LCD				0
#endif

/* Update touch every 50ms */
#ifndef EMWIN_UPDATE_TOUCH_MILLIS
#define EMWIN_UPDATE_TOUCH_MILLIS		50
#endif

/**
 * Result enumeration
 *
 * Parameters:
 * 	- TM_EMWIN_Result_Ok:
 * 		Everything OK
 * 	- TM_EMWIN_Result_Error:
 * 		An error occured
 * 	- TM_EMWIN_Result_TouchError:
 * 		Error with touch controller
 * 	- TM_EMWIN_Result_GUIError:
 * 		Error with emWin GUI
 */
typedef enum {
	TM_EMWIN_Result_Ok = 0,
	TM_EMWIN_Result_Error,
	TM_EMWIN_Result_TouchError,
	TM_EMWIN_Result_GUIError
} TM_EMWIN_Result_t;

/**
 * Rotation enumeration
 *
 * Parameters:
 * 	- TM_EMWIN_Rotate_0:
 * 		Rotate LCD for 0 degrees, default state
 * 	- TM_EMWIN_Rotate_90:
 * 		Rotate LCD 90 degrees
 * 	- TM_EMWIN_Rotate_180:
 * 		Rotate LCD 180 degrees
 * 	- TM_EMWIN_Rotate_270:
 * 		Rotate LCD 170 degrees
 */
typedef enum {
	TM_EMWIN_Rotate_0 = 0,
	TM_EMWIN_Rotate_90,
	TM_EMWIN_Rotate_180,
	TM_EMWIN_Rotate_270,
} TM_EMWIN_Rotate_t;

/**
 * Initialize emWin peripheral
 * This function initialize LCD, SDRAM, STMPE811 touch and GUI
 *
 * Member of TM_EMWIN_Result_t is returned
 */
extern TM_EMWIN_Result_t TM_EMWIN_Init(void);

/**
 * This function is used to rotate LCD
 * It does not have any sense, if you have disabled define TM_EMWIN_ROTATE_LCD.
 * By default, rotation is disabled for memory purpose.
 *
 * To rotate LCD you need additional RAM, but it is available on F429-Discovery board
 *
 * Parameters:
 *	- TM_EMWIN_Rotate_t rotation:
 *		Rotate direction
 *
 * Member of TM_EMWIN_Result_t is returned
 */
extern TM_EMWIN_Result_t TM_EMWIN_Rotate(TM_EMWIN_Rotate_t rotation);

/**
 * This function is used to update touch for emWin
 * It must be called periodically every 1ms.
 *
 * It makes touch controller check every number of ms as defined in EMWIN_UPDATE_TOUCH_MILLIS define
 *
 * Member of TM_EMWIN_Result_t is returned
 */
extern TM_EMWIN_Result_t TM_EMWIN_UpdateTouch(void);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
