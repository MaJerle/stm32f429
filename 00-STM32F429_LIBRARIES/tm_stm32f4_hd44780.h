/**
 *	HD44780 LCD driver library for STM32F4xx
 *	It also support's all HD44780 compatible LCD drivers
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/06/library-16-interfacing-hd44780-lcd-controller-with-stm32f4/
 *	@version 	v1.2
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
 * Version 1.2
 *	- March 11, 2015
 *	- Added support for my new GPIO library
 *
 * Version 1.1
 *	- November 08, 2014
 *	- D6 and D7 default pins changed from PC14,15 to PB12,13, because RTC crystal is on PC14,15
 *
 *	Default pinout
 *	
 *	LCD		STM32F4XX		DESCRIPTION
 *	
 *	GND		GND				Ground
 *	VCC		+5V				Power supply for LCD
 *	V0		Potentiometer	Contrast voltage. Connect to potentiometer
 *	RS		PB2				Register select, can be overwritten in your project’s defines.h file
 *	RW		GND				Read/write
 *	E		PB7				Enable pin, can be overwritten in your project’s defines.h file
 *	D0		-				Data 0 – doesn’t care
 *	D1		-				Data 1 - doesn’t care
 *	D2		-				Data 2 - doesn’t care
 *	D3		-				Data 3 - doesn’t  care
 *	D4		PC12			Data 4, can be overwritten in your project’s defines.h file
 *	D5		PC13			Data 5, can be overwritten in your project’s defines.h file
 *	D6		PB12			Data 6, can be overwritten in your project’s defines.h file
 *	D7		PB13			Data 7, can be overwritten in your project’s defines.h file
 *	A		+3V3			Backlight positive power
 *	K		GND				Ground for backlight
 *	
 *	
 *	If you want to change pinout, do this in your defines.h file with lines below and set your own settings:
 *	
 *	//RS - Register select pin
 *	#define TM_HD44780_RS_PORT		GPIOB
 *	#define TM_HD44780_RS_PIN		GPIO_PIN_2
 *	//E - Enable pin
 *	#define TM_HD44780_E_PORT		GPIOB
 *	#define TM_HD44780_E_PIN		GPIO_PIN_7
 *	//D4 - Data 4 pin
 *	#define TM_HD44780_D4_PORT		GPIOC
 *	#define TM_HD44780_D4_PIN		GPIO_PIN_12
 *	//D5 - Data 5 pin
 *	#define TM_HD44780_D5_PORT		GPIOC
 *	#define TM_HD44780_D5_PIN		GPIO_PIN_13
 *	//D6 - Data 6 pin
 *	#define TM_HD44780_D6_PORT		GPIOB
 *	#define TM_HD44780_D6_PIN		GPIO_PIN_12
 *	//D7 - Data 7 pin
 *	#define TM_HD44780_D7_PORT		GPIOB
 *	#define TM_HD44780_D7_PIN		GPIO_PIN_13
 */
#ifndef TM_HD44780_H
#define TM_HD44780_H 120
/**
 * Dependencies
 * 	- STM32F4xx
 * 	- STM32F4xx RCC
 * 	- STM32F4xx GPIO
 * 	- defines.h
 * 	- TM DELAY
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_gpio.h"

/* 4 bit mode */
/* Control pins, can be overwritten */
/* RS - Register select pin */
#ifndef TM_HD44780_RS_PIN
#define TM_HD44780_RS_PORT				GPIOB
#define TM_HD44780_RS_PIN				GPIO_PIN_2
#endif
/* E - Enable pin */
#ifndef TM_HD44780_E_PIN
#define TM_HD44780_E_PORT				GPIOB
#define TM_HD44780_E_PIN				GPIO_PIN_7
#endif
/* Data pins */
/* D4 - Data 4 pin */
#ifndef TM_HD44780_D4_PIN
#define TM_HD44780_D4_PORT				GPIOC
#define TM_HD44780_D4_PIN				GPIO_PIN_12
#endif
/* D5 - Data 5 pin */
#ifndef TM_HD44780_D5_PIN
#define TM_HD44780_D5_PORT				GPIOC
#define TM_HD44780_D5_PIN				GPIO_PIN_13
#endif
/* D6 - Data 6 pin */
#ifndef TM_HD44780_D6_PIN
#define TM_HD44780_D6_PORT				GPIOB
#define TM_HD44780_D6_PIN				GPIO_PIN_12
#endif
/* D7 - Data 7 pin */
#ifndef TM_HD44780_D7_PIN
#define TM_HD44780_D7_PORT				GPIOB
#define TM_HD44780_D7_PIN				GPIO_PIN_13
#endif

#define TM_HD44780_RS_LOW				TM_GPIO_SetPinLow(TM_HD44780_RS_PORT, TM_HD44780_RS_PIN)
#define TM_HD44780_RS_HIGH				TM_GPIO_SetPinHigh(TM_HD44780_RS_PORT, TM_HD44780_RS_PIN)
#define TM_HD44780_E_LOW				TM_GPIO_SetPinLow(TM_HD44780_E_PORT, TM_HD44780_E_PIN)
#define TM_HD44780_E_HIGH				TM_GPIO_SetPinHigh(TM_HD44780_E_PORT, TM_HD44780_E_PIN)

#define TM_HD44780_E_BLINK				TM_HD44780_E_HIGH; TM_HD44780_Delay(20); TM_HD44780_E_LOW; TM_HD44780_Delay(20)
#define TM_HD44780_Delay(x)				Delay(x)

/* Commands*/
#define TM_HD44780_CLEARDISPLAY			0x01
#define TM_HD44780_RETURNHOME			0x02
#define TM_HD44780_ENTRYMODESET			0x04
#define TM_HD44780_DISPLAYCONTROL		0x08
#define TM_HD44780_CURSORSHIFT			0x10
#define TM_HD44780_FUNCTIONSET			0x20
#define TM_HD44780_SETCGRAMADDR			0x40
#define TM_HD44780_SETDDRAMADDR			0x80

/* Flags for display entry mode */
#define TM_HD44780_ENTRYRIGHT			0x00
#define TM_HD44780_ENTRYLEFT			0x02
#define TM_HD44780_ENTRYSHIFTINCREMENT 	0x01
#define TM_HD44780_ENTRYSHIFTDECREMENT 	0x00

/* Flags for display on/off control */
#define TM_HD44780_DISPLAYON			0x04
#define TM_HD44780_CURSORON				0x02
#define TM_HD44780_BLINKON				0x01

/* Flags for display/cursor shift */
#define TM_HD44780_DISPLAYMOVE			0x08
#define TM_HD44780_CURSORMOVE			0x00
#define TM_HD44780_MOVERIGHT			0x04
#define TM_HD44780_MOVELEFT				0x00

/* Flags for function set */
#define TM_HD44780_8BITMODE				0x10
#define TM_HD44780_4BITMODE				0x00
#define TM_HD44780_2LINE				0x08
#define TM_HD44780_1LINE				0x00
#define TM_HD44780_5x10DOTS				0x04
#define TM_HD44780_5x8DOTS				0x00

/**
 * Initialize LCD
 *
 * Parameters:
 * 	- uint8_t cols: width of lcd
 * 	- uint8_t rows: height of lcd
 *
 * No return
 */
extern void TM_HD44780_Init(uint8_t cols, uint8_t rows);

/**
 * Turn display on
 *
 * No return
 */
extern void TM_HD44780_DisplayOn(void);

/**
 * Turn display off
 *
 * No return
 */
extern void TM_HD44780_DisplayOff(void);

/**
 * Clear entire LCD
 *
 * No return
 */
extern void TM_HD44780_Clear(void);

/**
 * Put string on lcd
 *
 * Parameters:
 * 	- uint8_t x: x location
 * 	- uint8_t y: y location
 * 	- char* str: pointer to string
 *
 * No return
 */
extern void TM_HD44780_Puts(uint8_t x, uint8_t y, char* str);

/**
 * Enable cursor blink
 *
 * No return
 */
extern void TM_HD44780_BlinkOn(void);

/**
 * Disable cursor blink
 *
 * No return
 */
extern void TM_HD44780_BlinkOff(void);

/**
 * Show cursor
 *
 * No return
 */
extern void TM_HD44780_CursorOn(void);

/**
 * Hide cursor
 *
 * No return
 */
extern void TM_HD44780_CursorOff(void);

/**
 * Scroll display to the left
 *
 * No return
 */
extern void TM_HD44780_ScrollLeft(void);

/**
 * Scroll display to the right
 *
 * No return
 */
extern void TM_HD44780_ScrollRight(void);

/**
 * Create custom character
 *
 * Parameters:
 *  - uint8_t location:
 *  	Location where to save character on LCD. LCD supports up to 8 custom characters, so locations are 0 - 7
 *  - uint8_t *data
 *  	Pointer to 8bytes of data for one character
 *
 * No return
 */
extern void TM_HD44780_CreateChar(uint8_t location, uint8_t* data);

/**
 * Put custom created character on LCD
 *
 * Parameters:
 * 	- uint8_t location
 * 		Location on LCD where character is stored, 0 - 7
 *
 * No return
 */
extern void TM_HD44780_PutCustom(uint8_t x, uint8_t y, uint8_t location);

#endif

