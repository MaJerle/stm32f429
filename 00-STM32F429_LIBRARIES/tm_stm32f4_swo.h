/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/12/library-46-debug-stm32f4-device-swo-feature/
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   SWO Debugging library for Cortex-M4/M3 processors including STM32F4xx
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
#ifndef TM_SWO_H
#define TM_SWO_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_SWO
 * @brief    SWO Debugging library for Cortex-M4/M3 processors including STM32F4xx - http://stm32f4-discovery.com/2014/12/library-46-debug-stm32f4-device-swo-feature/
 * @{
 *
 * Instead of using UART for debugging your program, you can use better solution for this.
 *
 * STM32F4xx (all Cortex-M4 MCUs) have SWD programming/debugging mode. 
 *
 * It also contains SWO (Single Wire Output) for debugging purpose.
 *
 * @note This output works only (!!) when you are in debug mode with your processor.
 * 
 * This library allows you to send data to PC using SWO peripheral.
 * 
 * Library is designed in a way that you can with simple define disable debug functionality,
 * but you don't need to clear all your debug statements somewhere in your code.
 *
 * By default, debug mode is active, but if you want to disable it, you can open defines.h file and add following define inside:
 *
@verbatim
//Disable debug mode, disable all functions
#define SWO_DEBUG_ENABLED     0
@endverbatim
 *
 * Note, that if you don't disable SWO debugging in end application, it will work,
 * but will take some processor cycles to try to output data. To prevent this checking,
 * using define above, my functions becomes empty defines and compiler will throw them out from program.
 *
 * Important notes:
 *	- Your IDE must include debugging functionality,
 *		- If it does not, you can download "STM32 ST-LINK Utility" from st.com and connect via ST-Link to your device,
 *		- In opened program click on "ST-LINK" -> "Printf via SWO viewer",
 *		- Set CPU frequency and hit "Start"
 *	- Keil uVIsion have debug option built-in,
 *		- In "Options for target" go to "Debug" tab, select your debugger and hit "Settings" (ST-Link for most cases)
 *		- Under "Trace" tab set CPU frequency under "Core Clock" and click on "Enable Trace"
 * 		- Under "ITM Stimulus Part" write "0x00000001" where you have "Enable" window,
 *		- You have 4 checkboxes for "Ports":
 *			- Enable "Port 31..24"
 *			- Disable other 3
 *		- Compile your application and click on "Start/Stop Debug Session"
 *		- On the left top, find button "Run" or press "F5" and if you don't have it, open Trace window under 
 *		  "View" tab
 *	- In case you are using STM32F4xx boards, you need to do some modifications to enable "Trace output":
 *		- STM32F4-Discovery: Enabled by default
 *		- STM32F429-Discovery: You have to solder SB9 jumper to enable this feature
 *		- STM32-Nucleo: Enabled by default
 *			- Nucleo boards have different ST-Link firmware and I've never got this working with my Nucleo F401 or F411,
 *			  I always get error "Cannot read memory" in Keil uVision, also ST-Link Utility does not work.
 *			- With external debugger connected to SWD pins, works great. It must be problem with ST-Link on Nucleo boards.
 * - Pin PB3 is output for SWO in STM32F4xx.
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
 - TM STDIO
 - stdio.h
@endverbatim
 */

#include "stm32f4xx.h"
#include "defines.h"
#include "stdio.h"
#include "tm_stm32f4_stdio.h"

/**
 * @defgroup TM_SWO_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  SWO debug status, enabled by default
 */
#ifndef SWO_DEBUG_ENABLED
#define SWO_DEBUG_ENABLED		1
#endif

#if SWO_DEBUG_ENABLED == 1
	/* FILE struct for working with stream */
	extern FILE TM_SWO_File;
#endif

/**
 * @}
 */

/**
 * @defgroup TM_SWO_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes SWO debug output
 * @note   Function prepares SWO on MCU for output data to debugger
 *
 * If debugging mode is enabled, then this is a function.
 * if not, then function becomes simple define statement without content
 * @param  None
 * @retval None
 */	
#if SWO_DEBUG_ENABLED == 1
	/* Debugging enabled, initialize functionality */
	extern void TM_SWO_Init(void);
#else
	/* Do nothing here, compiler will throw out this statement because it is empty */
	#define TM_SWO_Init()
#endif

/**
 * @brief  Formats and outputs data to the SWO output
 * @note   This function acts similar to printf except that custom stream is used
 * @param  args...: Parameters are like printf. First parameter is string, 
 *		then you can use unlimited parameters according to string to be formatted
 * @retval None
 * @note   Defined as macro for faster execution and 
 *
 * If debugging mode is enabled, then this is a function.
 * if not, then function becomes simple define statement without content
 */
#if SWO_DEBUG_ENABLED == 1
	/* Format and output data to the SWO output with stream */
	#define TM_SWO_Printf(args...)  { fprintf(&TM_SWO_File, ## args); }
#else
	/* Do nothing here, compiler will throw out this statement because it is empty */
	#define TM_SWO_Printf(args...) 
#endif


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
