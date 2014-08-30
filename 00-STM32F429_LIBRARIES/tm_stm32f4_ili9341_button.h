/**
 *	Button library for ILI9341 LCD controller
 * 	and STMPE811 Resistive touch screen controller
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/05/11-button-library-for-ili9341-lcd-and-stmpe811-touch-controller-on-stm32f429-discovery-board/
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
 *	
 *	By default, library supports up to 10 buttons.
 *	If you want to use more of them, change line below in your defines.h file:
 *	
 *	#define TM_ILI9341_BUTTON_MAX_BUTTONS	10
 *	
 */
#ifndef TM_ILI9341_BUTTON_H
#define TM_ILI9341_BUTTON_H
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - defines.h
 * - TM_I2C
 * - TM_DELAY
 * - TM_ILI9341
 * - TM_FONTS
 * - TM_STMPE811
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"
#include "tm_stm32f4_stmpe811.h"

// Maximum number of buttons available
#ifndef TM_ILI9341_BUTTON_MAX_BUTTONS
#define TM_ILI9341_BUTTON_MAX_BUTTONS	10
#endif

//Button flags
#define TM_BUTTON_FLAG_ENABLED			0x0001	// Button is enabled - internal flag
#define TM_BUTTON_FLAG_NOBORDER			0x0002	// No border used in
#define TM_BUTTON_FLAG_IMAGE			0x0004	// Use image for background
#define TM_BUTTON_FLAG_NOLABEL			0x0008	// Do not display button label
#define TM_BUTTON_FLAG_USED				0x8000	// Button is used - internal flag

/**
 * Button options struct
 *
 * Parameters:
 *	- uint16_t x: X coordinate of top left button corner
 *	- uint16_t y: Y coordinate of top left button corner
 *	- uint16_t width: button width in pixels
 *	- uint16_t height: button height in pixels
 *	- uint16_t background: 16bit background color
 *	- uint16_t borderColor: 16bit border color
 *	- uint16_t flags: Button flags
 *	- char *label: pointer to first character of label
 *	- TM_FontDef_t *font: pointer to font structure
 *	- uint16_t color: label color
 *	- uint16_t *image: pointer to location at first color for pixel
 */
typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	uint16_t background;
	uint16_t borderColor;
	uint16_t flags;
	char* label;
	TM_FontDef_t* font;
	uint16_t color;
	uint16_t* image;
} TM_ILI9341_Button_t;

extern TM_ILI9341_Button_t TM_ILI9341_Buttons[TM_ILI9341_BUTTON_MAX_BUTTONS];

/**
 * Add new button to library
 *
 * Parameters:
 * 	- TM_ILI9341_Button_t *button: pointer to TM_ILI9341_Button_t structure
 *
 * Returns button id on success (starting from 0), or -1 on failure
 */
extern int8_t TM_ILI9341_Button_Add(TM_ILI9341_Button_t* button);

/**
 * Draw button on LCD
 *
 * Parameters:
 * 	uint8_t id: button id to draw
 *
 * Return SUCCESS on success, otherwise ERROR
 */
extern ErrorStatus TM_ILI9341_Button_Draw(uint8_t id);

/**
 * Draw all buttons in library
 *
 *
 */
extern void TM_ILI9341_Button_DrawAll(void);

/**
 * Enable button
 *
 * Parameters:
 * 	- uint8_t id: button id
 */
extern void TM_ILI9341_Button_Enable(uint8_t id);

/**
 * Disable button
 *
 * Parameters:
 * 	- uint8_t id: button id
 */
extern void TM_ILI9341_Button_Disable(uint8_t id);

/**
 * Delete all buttons in library
 *
 *
 */
extern void TM_ILI9341_Button_DeleteAll(void);

/**
 * Delete button
 *
 * Parameters:
 * 	- uint8_t id: button id
 */
extern void TM_ILI9341_Button_Delete(uint8_t id);

/**
 * Get pressed button
 *
 * Parameters:
 * 	- TM_STMPE811_TouchData *TouchData: pointer to TM_STMPE811_TouchData structure
 *
 * Returns button id on success, otherwise -1
 */
extern int8_t TM_ILI9341_Button_Touch(TM_STMPE811_TouchData* TouchData);

#endif

