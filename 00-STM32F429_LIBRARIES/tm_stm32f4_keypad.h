/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/09/library-32-matrix-keypad-on-stm32f4xx
 * @version v2.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Matrix keyboard for STM32F4xx device
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
#ifndef TM_KEYPAD_H
#define TM_KEYPAD_H 200

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_KEYPAD
 * @brief    Matrix keyboard for STM32F4xx device - http://stm32f4-discovery.com/2014/09/library-32-matrix-keypad-on-stm32f4xx
 * @{
 *
 * Library supports 2 versions of keypads:
 * 	- Large
 * 		4x4 = 4 rows and 4 columns
 * 	- Small
 * 		3x4 = 4 rows and 3 columns
 *
 * \par Default pinout
 *
@verbatim
Keypad		STM32F4xx		Description

C1			PD0				Column 1
C2			PD1				Column 2
C3			PD2				Column 3
C4			PD3				Column 4
R1			PC1				Row 1
R2			PC2				Row 2
R3			PC3				Row 3
R4			PC5				Row 4
@endverbatim
 *
 * You can change pinout. Open defines.h file and change lines below:
 *
@verbatim
//Change rows pinout. Change X from 1 to 4, according to ROW you want to change
#define KEYPAD_ROW_X_PORT			GPIOC
#define KEYPAD_ROW_X_PIN			GPIO_PIN_0

//Change columns pinout. Change X from 1 to 4, according to COLUMN you want to change
#define KEYPAD_COLUMN_X_PORT		GPIOD
#define KEYPAD_COLUMN_X_PIN			GPIO_PIN_0
@endverbatim
 *
 * You can set this for all 4 columns and all 4 rows.
 *
 * \par Changelog
 *
@verbatim 
 Version 2.0
  - April 11, 2015
  - Library rewritten, now with more accuracy and interrupt based
  
 Version 1.1
  - March 11, 2015
  - Added support for my new GPIO library
  
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - defines.h
@endverbatim
 */
/**
 * Library dependencies
 * - STM32F4xx
 * - defines.h
 * - TM GPIO
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "defines.h"
#include "tm_stm32f4_gpio.h"


/**
 * @defgroup TM_KEYPAD_Macros
 * @brief    Library defines
 * @{
 */
 
/* Rows */
/* Row 1 default */
#ifndef KEYPAD_ROW_1_PIN
#define KEYPAD_ROW_1_PORT			GPIOC
#define KEYPAD_ROW_1_PIN			GPIO_PIN_1
#endif
/* Row 2 default */
#ifndef KEYPAD_ROW_2_PIN
#define KEYPAD_ROW_2_PORT			GPIOC
#define KEYPAD_ROW_2_PIN			GPIO_PIN_2
#endif
/* Row 3 default */
#ifndef KEYPAD_ROW_3_PIN
#define KEYPAD_ROW_3_PORT			GPIOC
#define KEYPAD_ROW_3_PIN			GPIO_PIN_3
#endif
/* Row 4 default */
#ifndef KEYPAD_ROW_4_PIN
#define KEYPAD_ROW_4_PORT			GPIOC
#define KEYPAD_ROW_4_PIN			GPIO_PIN_5
#endif

/* Columns */
/* Column 1 default */
#ifndef KEYPAD_COLUMN_1_PIN
#define KEYPAD_COLUMN_1_PORT		GPIOD
#define KEYPAD_COLUMN_1_PIN			GPIO_PIN_0
#endif
/* Column 2 default */
#ifndef KEYPAD_COLUMN_2_PIN
#define KEYPAD_COLUMN_2_PORT		GPIOD
#define KEYPAD_COLUMN_2_PIN			GPIO_PIN_1
#endif
/* Column 3 default */
#ifndef KEYPAD_COLUMN_3_PIN
#define KEYPAD_COLUMN_3_PORT		GPIOD
#define KEYPAD_COLUMN_3_PIN			GPIO_PIN_2
#endif
/* Column 4 default */
#ifndef KEYPAD_COLUMN_4_PIN
#define KEYPAD_COLUMN_4_PORT		GPIOD
#define KEYPAD_COLUMN_4_PIN			GPIO_PIN_3
#endif

/* Number of milliseconds between 2 reads */
#ifndef KEYPAD_READ_INTERVAL
#define KEYPAD_READ_INTERVAL        100
#endif

/* Keypad no pressed */
#define KEYPAD_NO_PRESSED			(uint8_t)0xFF

/**
 * @}
 */
 
/**
 * @defgroup TM_KEYPAD_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Keypad button enumeration
 */
typedef enum {
	TM_KEYPAD_Button_0 = 0x00,                     /*!< Button 0 code */
	TM_KEYPAD_Button_1 = 0x01,                     /*!< Button 1 code */
	TM_KEYPAD_Button_2 = 0x02,                     /*!< Button 2 code */
	TM_KEYPAD_Button_3 = 0x03,                     /*!< Button 3 code */
	TM_KEYPAD_Button_4 = 0x04,                     /*!< Button 4 code */
	TM_KEYPAD_Button_5 = 0x05,                     /*!< Button 5 code */
	TM_KEYPAD_Button_6 = 0x06,                     /*!< Button 6 code */
	TM_KEYPAD_Button_7 = 0x07,                     /*!< Button 7 code */
	TM_KEYPAD_Button_8 = 0x08,                     /*!< Button 8 code */
	TM_KEYPAD_Button_9 = 0x09,                     /*!< Button 9 code */
	TM_KEYPAD_Button_STAR = 0x0A,                  /*!< Button START code */
	TM_KEYPAD_Button_HASH = 0x0B,                  /*!< Button HASH code */
	TM_KEYPAD_Button_A = 0x0C,	                   /*!< Button A code. Only on large size */
	TM_KEYPAD_Button_B = 0x0D,	                   /*!< Button B code. Only on large size */
	TM_KEYPAD_Button_C = 0x0E,	                   /*!< Button C code. Only on large size */
	TM_KEYPAD_Button_D = 0x0F,	                   /*!< Button D code. Only on large size */
	TM_KEYPAD_Button_NOPRESSED = KEYPAD_NO_PRESSED /*!< No button pressed */
} TM_KEYPAD_Button_t;

/**
 * @brief  Keypad size enumeration
 */
typedef enum {
	TM_KEYPAD_Type_Large = 0x00, /*!< Keypad 4x4 size */
	TM_KEYPAD_Type_Small         /*!< Keypad 3x4 size */
} TM_KEYPAD_Type_t;

/**
 * @}
 */

/**
 * @defgroup TM_KEYPAD_Functions
 * @brief    Library Functions
 * @{
 */
 
/**
 * @brief  Initializes keypad functionality
 * @param  type: Keypad type you will use. This parameter can be a value of @ref TM_KEYPAD_Type_t enumeration
 * @retval None
 */
void TM_KEYPAD_Init(TM_KEYPAD_Type_t type);

/**
 * @brief  Reads keypad data
 * @param  None
 * @retval Button status. This parameter will be a value of @ref TM_KEYPAD_Button_t enumeration
 */
TM_KEYPAD_Button_t TM_KEYPAD_Read(void);

/**
 * @brief  Updates keypad
 * @note   This function must be called from interrupt routine every 1ms
 * @param  None
 * @retval None
 */
void TM_KEYPAD_Update(void);

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

