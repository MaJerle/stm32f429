/**
 *	Matrix keyboard for STM32F4xx device
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/09/library-32-matrix-keypad-on-stm32f4xx
 *	@version 	v1.1
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
 * Version 1.1
 *	- March 11, 2015
 *	- Added support for my new GPIO library
 *
 * Library supports 2 versions of keyboards:
 * 	- Large
 * 		4x4 = 4 rows and 4 columns
 * 	- Small
 * 		3x4 = 4 rows and 3 columns
 *
 * Default pinout
 *
 * 	Keypad		STM32F4xx		Description
 *
 * 	C1			PD0				Column 1
 *	C2			PD1				Column 2
 *	C3			PD2				Column 3
 *	C4			PD3				Column 4
 *	R1			PC1				Row 1
 *	R2			PC2				Row 2
 *	R3			PC3				Row 3
 *	R4			PC5				Row 4
 *
 * You can change pinout. Open defines.h file and change lines below:
 *
 * 	//Change rows pinout. Change X from 1 to 4, according to ROW you want to change
 *	#define KEYPAD_ROW_X_PORT			GPIOC
 *	#define KEYPAD_ROW_X_PIN			GPIO_PIN_0
 *
 *	//Change columns pinout. Change X from 1 to 4, according to COLUMN you want to change
 *	#define KEYPAD_COLUMN_X_PORT		GPIOD
 *	#define KEYPAD_COLUMN_X_PIN			GPIO_PIN_0
 *
 * You can set this for all 4 columns and all 4 rows
 *
 * Library uses some kind of debounce counter.
 * I set to an optimal value, but if you are working in an application, where you can not high read keyboard,
 * change define in defines.h file
 *
 * 	//Change keyboard debounce counter
 *  #define KEYPAD_DEBOUNCE 			5000
 *
 * Library also supports long press (hold feature), but this is not accurate on all devices.
 * I was making tests on F407 device with 168MHz clock. I gave about 10Hz refresh rate.
 *
 * If you are working with lower speed, 84MHz on F401, you can set some defines to increase refresh rate.
 * Open defines.h file and change define
 *
 *	//Larger this file is, higher refresh rate you will get if you hold pressed button
 * 	#define KEYPAD_HOLD_DEBOUNCE_DIVIDER	1
 *
 * When you press button, this library works like on PC. You press button, button is activated immideatelly,
 * but if you sill hold is, there is some delay between next character, just like on PC, when you first press,
 * you have to wait some time. Minimum time is the same as DEFAULT refresh rate, but if you want to increase it more,
 * you can change some constants
 *
 *	//Increase time between first press and hold feature
 *	//By default, 6 is used, so on 168MHz device, minimum od 600ms you have to wait.
 *	//Minumum can be set to 1 => 100ms wait
 *	#define KEYPAD_FIRST_HOLD_MULTIPLIER 6
 */
#ifndef TM_KEYPAD_H
#define TM_KEYPAD_H 110
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

/* Debounce counter */
#ifndef KEYPAD_DEBOUNCE
#define KEYPAD_DEBOUNCE 				5000
#endif

#ifndef KEYPAD_HOLD_DEBOUNCE_DIVIDER
#define KEYPAD_HOLD_DEBOUNCE_DIVIDER	1
#endif

/* First hold button debounce counters = 1 second between presses */
/* Multiplier according to normal debounce */
/* 10 times more time to wait before start */
#ifndef KEYPAD_FIRST_HOLD_MULTIPLIER
#define KEYPAD_FIRST_HOLD_MULTIPLIER	6
#endif

/* Hold button debounce counters, at 168MHz clock ~ 10 Hz max */
#define KEYPAD_HOLD_DEBOUNCE_0		32653
#define KEYPAD_HOLD_DEBOUNCE_1		77777
#define KEYPAD_HOLD_DEBOUNCE_2		40000
#define KEYPAD_HOLD_DEBOUNCE_3		26936
#define KEYPAD_HOLD_DEBOUNCE_4		67796
#define KEYPAD_HOLD_DEBOUNCE_5		37037
#define KEYPAD_HOLD_DEBOUNCE_6		25581
#define KEYPAD_HOLD_DEBOUNCE_7		61100
#define KEYPAD_HOLD_DEBOUNCE_8		35010
#define KEYPAD_HOLD_DEBOUNCE_9		24560
#define KEYPAD_HOLD_DEBOUNCE_STAR	54421
#define KEYPAD_HOLD_DEBOUNCE_HASH	23323
#define KEYPAD_HOLD_DEBOUNCE_A		20000
#define KEYPAD_HOLD_DEBOUNCE_B		19230
#define KEYPAD_HOLD_DEBOUNCE_C		18691
#define KEYPAD_HOLD_DEBOUNCE_D		18000

/* First debounce */
/* Counter for first debounce, from first valid data to hold valid */
#ifndef KEYPAD_FIRST_DEBOUNCE
#define KEYPAD_FIRST_DEBOUNCE		300000
#endif

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

/* Pins configuration, columns are outputs */
#define KEYPAD_COLUMN_1_HIGH		TM_GPIO_SetPinHigh(KEYPAD_COLUMN_1_PORT, KEYPAD_COLUMN_1_PIN)
#define KEYPAD_COLUMN_1_LOW			TM_GPIO_SetPinLow(KEYPAD_COLUMN_1_PORT, KEYPAD_COLUMN_1_PIN)
#define KEYPAD_COLUMN_2_HIGH		TM_GPIO_SetPinHigh(KEYPAD_COLUMN_2_PORT, KEYPAD_COLUMN_2_PIN)
#define KEYPAD_COLUMN_2_LOW			TM_GPIO_SetPinLow(KEYPAD_COLUMN_2_PORT, KEYPAD_COLUMN_2_PIN)
#define KEYPAD_COLUMN_3_HIGH		TM_GPIO_SetPinHigh(KEYPAD_COLUMN_3_PORT, KEYPAD_COLUMN_3_PIN)
#define KEYPAD_COLUMN_3_LOW			TM_GPIO_SetPinLow(KEYPAD_COLUMN_3_PORT, KEYPAD_COLUMN_3_PIN)
#define KEYPAD_COLUMN_4_HIGH		TM_GPIO_SetPinHigh(KEYPAD_COLUMN_4_PORT, KEYPAD_COLUMN_4_PIN)
#define KEYPAD_COLUMN_4_LOW			TM_GPIO_SetPinLow(KEYPAD_COLUMN_4_PORT, KEYPAD_COLUMN_4_PIN)

/* Read input pins */
#define KEYPAD_ROW_1_CHECK			(!TM_GPIO_GetInputPinValue(KEYPAD_ROW_1_PORT, KEYPAD_ROW_1_PIN))
#define KEYPAD_ROW_2_CHECK			(!TM_GPIO_GetInputPinValue(KEYPAD_ROW_2_PORT, KEYPAD_ROW_2_PIN))
#define KEYPAD_ROW_3_CHECK			(!TM_GPIO_GetInputPinValue(KEYPAD_ROW_3_PORT, KEYPAD_ROW_3_PIN))
#define KEYPAD_ROW_4_CHECK			(!TM_GPIO_GetInputPinValue(KEYPAD_ROW_4_PORT, KEYPAD_ROW_4_PIN))

/* Keypad no pressed */
#define KEYPAD_NO_PRESSED			(uint8_t)0xFF

/**
 * Pressed button enumeration
 *
 * Returned when you press button
 */
typedef enum {
	TM_KEYPAD_Button_0 = 0x00,
	TM_KEYPAD_Button_1 = 0x01,
	TM_KEYPAD_Button_2 = 0x02,
	TM_KEYPAD_Button_3 = 0x03,
	TM_KEYPAD_Button_4 = 0x04,
	TM_KEYPAD_Button_5 = 0x05,
	TM_KEYPAD_Button_6 = 0x06,
	TM_KEYPAD_Button_7 = 0x07,
	TM_KEYPAD_Button_8 = 0x08,
	TM_KEYPAD_Button_9 = 0x09,
	TM_KEYPAD_Button_STAR = 0x0A,
	TM_KEYPAD_Button_HASH = 0x0B,
	TM_KEYPAD_Button_A = 0x0C,	/* Only on large size */
	TM_KEYPAD_Button_B = 0x0D,	/* Only on large size */
	TM_KEYPAD_Button_C = 0x0E,	/* Only on large size */
	TM_KEYPAD_Button_D = 0x0F,	/* Only on large size */
	TM_KEYPAD_Button_NOPRESSED = KEYPAD_NO_PRESSED
} TM_KEYPAD_Button_t;

/**
 * Select keypad type
 *
 * Parameters:
 * 	- TM_KEYPAD_Type_Large
 * 		keypad 4x4 size
 * 	- TM_KEYPAD_Type_Small
 * 		keypad 3x4 size
 */
typedef enum {
	TM_KEYPAD_Type_Large,
	TM_KEYPAD_Type_Small
} TM_KEYPAD_Type_t;

/**
 * Initialize keypad peripheral
 *
 * Parameters:
 * 	- TM_KEYPAD_Type_t type
 * 		Select keyboard size
 */
extern void TM_KEYPAD_Init(TM_KEYPAD_Type_t type);

/**
 * Read keypad data
 *
 * Returned TM_KEYPAD_Button_NOPRESSED if no button pressed,
 * otherwise button value is returned
 */
extern TM_KEYPAD_Button_t TM_KEYPAD_Read(void);

#endif

