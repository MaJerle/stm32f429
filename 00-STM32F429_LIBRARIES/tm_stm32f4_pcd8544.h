/**
 * PCD8544 LCD controller driver.
 * This controller is used in Nokia's 3310 and 5110 LCD's.
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/pcd8544-nokia-33105110-lcd-stm32f429-discovery-library/
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
 *	Default pinout
 *	
 *	LCD BOARD	STM32F444	DESCRIPTION
 *	
 *	RST			PC15		Reset pin for LCD
 *	CE			PC13		Chip enable for SPI2
 *	DC			PC14		Data/Command pin
 *	DIN			PC3			MOSI pin for SPI2
 *	CLK			PB10		CLOCK pin for SPI2
 *	VCC			3.3V		VCC Power
 *	LIGHT		GND			If you connect this to GND, backlight is turned on
 *	GND			GND			Ground
 *	
 * If you want to change your SPI, set lines below in defines.h file:
 * 
 *	//Set default SPI
 * 	#define PCD8544_SPI				SPI2
 * 	#define PCD8544_SPI_PINSPACK 	TM_SPI_PinsPack_1
 * 	
 * Also, other pins can be changed as well, in your defines.h file
 * 
 *	//Default RST pin
 *	#define PCD8544_RST_RCC			RCC_AHB1Periph_GPIOC
 *	#define PCD8544_RST_PORT		GPIOC
 *	#define PCD8544_RST_PIN			GPIO_Pin_15
 *	
 *	//Default DC pin
 *	#define PCD8544_DC_RCC			RCC_AHB1Periph_GPIOC
 *	#define PCD8544_DC_PORT			GPIOC
 *	#define PCD8544_DC_PIN			GPIO_Pin_14
 *	
 *	//Default CE pin
 *	#define PCD8544_CE_RCC			RCC_AHB1Periph_GPIOC
 *	#define PCD8544_CE_PORT			GPIOC
 *	#define PCD8544_CE_PIN			GPIO_Pin_13
 *	
 */
#ifndef PCD8544_H
#define PCD8544_H 100
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - TM_SPI
 */
/**
 * Includes
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "tm_stm32f4_spi.h"

//SPI used
#ifndef PCD8544_SPI
#define PCD8544_SPI				SPI2
#define PCD8544_SPI_PINSPACK 	TM_SPI_PinsPack_1
#endif

//Default pins used
//Default RST pin
#ifndef PCD8544_RST_PIN
#define PCD8544_RST_RCC			RCC_AHB1Periph_GPIOC
#define PCD8544_RST_PORT		GPIOC
#define PCD8544_RST_PIN			GPIO_Pin_15
#endif
//Default DC pin
#ifndef PCD8544_DC_PIN
#define PCD8544_DC_RCC			RCC_AHB1Periph_GPIOC
#define PCD8544_DC_PORT			GPIOC
#define PCD8544_DC_PIN			GPIO_Pin_14
#endif
//Default CE pin
#ifndef PCD8544_CE_PIN
#define PCD8544_CE_RCC			RCC_AHB1Periph_GPIOC
#define PCD8544_CE_PORT			GPIOC
#define PCD8544_CE_PIN			GPIO_Pin_13
#endif

#define PCD8544_CE_LOW			GPIO_ResetBits(PCD8544_CE_PORT, PCD8544_CE_PIN)
#define PCD8544_CE_HIGH			GPIO_SetBits(PCD8544_CE_PORT, PCD8544_CE_PIN)

/**
 * Private enums
 */
typedef enum {
	PCD8544_COMMAND = 0,
	PCD8544_DATA = !PCD8544_COMMAND
} PCD8544_WriteType_t;

typedef enum {
	PCD8544_State_Low = 0,
	PCD8544_State_High = !PCD8544_State_Low
} PCD8544_State_t;

typedef enum {
	PCD8544_Pin_DC = 1,
	PCD8544_Pin_RST = 2
} PCD8544_Pin_t;

/**
 * Public enums
 */
/**
 * Used for pixel "color"
 * LCD is 2 bit, so pixel can be just set or clear
 */
typedef enum {
	PCD8544_Pixel_Clear = 0,
	PCD8544_Pixel_Set = !PCD8544_Pixel_Clear
} PCD8544_Pixel_t;

/**
 * Font size for text
 * There are 2 sizes included
 */
typedef enum {
	PCD8544_FontSize_5x7 = 0,
	PCD8544_FontSize_3x5 = !PCD8544_FontSize_5x7
} PCD8544_FontSize_t;

/**
 * Used to invert pixels
 */
typedef enum {
	PCD8544_Invert_Yes,
	PCD8544_Invert_No
} PCD8544_Invert_t;


#define PCD8544_WIDTH				84
#define PCD8544_HEIGHT				48

// PCD8544 Commandset
// ------------------
// General commands
#define PCD8544_POWERDOWN			0x04
#define PCD8544_ENTRYMODE			0x02
#define PCD8544_EXTENDEDINSTRUCTION	0x01
#define PCD8544_DISPLAYBLANK		0x00
#define PCD8544_DISPLAYNORMAL		0x04
#define PCD8544_DISPLAYALLON		0x01
#define PCD8544_DISPLAYINVERTED		0x05
// Normal instruction set
#define PCD8544_FUNCTIONSET			0x20
#define PCD8544_DISPLAYCONTROL		0x08
#define PCD8544_SETYADDR			0x40
#define PCD8544_SETXADDR			0x80
// Extended instruction set
#define PCD8544_SETTEMP				0x04
#define PCD8544_SETBIAS				0x10
#define PCD8544_SETVOP				0x80
// Display presets
#define PCD8544_LCD_BIAS			0x03	// Range: 0-7 (0x00-0x07)
#define PCD8544_LCD_TEMP			0x02	// Range: 0-3 (0x00-0x03)
#define PCD8544_LCD_CONTRAST		0x46	// Range: 0-127 (0x00-0x7F)

#define PCD8544_CHAR5x7_WIDTH		6 //5*8
#define PCD8544_CHAR5x7_HEIGHT		8
#define PCD8544_CHAR3x5_WIDTH		4 //3*5
#define PCD8544_CHAR3x5_HEIGHT		6

#define PCD8544_BUFFER_SIZE 		PCD8544_WIDTH * PCD8544_HEIGHT / 8


/**
 * Initialize all IO pins and peripheral
 *
 */
extern void PCD8544_InitIO(void);

/**
 * Send data to LCD
 *
 */
extern void PCD8544_send(unsigned char data);

/**
 * Set DC pin state
 *
 */
extern void PCD8544_Pin(PCD8544_Pin_t pin, PCD8544_State_t state);

/**
 * Specify delay in microseconds
 *
 */
extern void PCD8544_Delay(unsigned long micros);

/**
 * Write data/command to LCD
 *
 */
extern void PCD8544_Write(PCD8544_WriteType_t cd, unsigned char data);

/**
 * Set area for refresh display
 *
 */
extern void PCD8544_UpdateArea(unsigned char xMin, unsigned char yMin, unsigned char xMax, unsigned char yMax);

/**
 * Initialize LCD
 *
 * Parameters
 * - unsigned char contrast: Contrast can be software selectable, from 0x00 to 0x7F
 */
extern void PCD8544_Init(unsigned char contrast);

/**
 * Set contrast for LCD
 *
 * Parameters
 * - unsigned char contrast: Contrast can be software selectable, from 0x00 to 0x7F
 */
extern void PCD8544_SetContrast(unsigned char contrast);

/**
 * Invert LCD
 *
 * Parameters
 * - PCD8544_Invert_t invert
 * 		- PCD8544_Invert_Yes: Invert pixels on lcd
 * 		- PCD8544_Invert_No: Return pixels to orignal
 */
extern void PCD8544_Invert(PCD8544_Invert_t invert);

/**
 * Clear lcd
 */
extern void PCD8544_Clear(void);

/**
 * Set/clear pixel at specific location
 *
 * Parameters
 * - unsigned char x: x position of pixel
 * - unsigned char y: y position of pixel
 * - PCD8544_PCD8544_Pixel_t pixel: Set or clear pixel
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_DrawPixel(unsigned char x, unsigned char y, PCD8544_Pixel_t pixel);

/**
 * Set cursor on lcd at 0, 0 location
 *
 */
extern void PCD8544_Home(void);

/**
 * Put data from internal buffer to lcd
 *
 */
extern void PCD8544_Refresh(void);

/**
 * Set cursor to desired position
 *
 * Parameters:
 * - unsigned char x: x from 0 to 83
 * - unsigned char y: y from 0 to 47
 */
extern void PCD8544_GotoXY(unsigned char x, unsigned char y);

/**
 * Put character on LCD
 *
 * Parameters:
 * - char c: char to display
 * - PCD8544_FontSize_t size: Font size
 * 		- PCD8544_FontSize_5x7
 * 		- PCD8544_FontSize_3x5
 * - PCD8544_PCD8544_Pixel_t color
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_Putc(char c, PCD8544_Pixel_t color, PCD8544_FontSize_t size);

/**
 * Put string on LCD
 *
 * Parameters:
 * - char *c: pointer to first character of string
 * - PCD8544_FontSize_t size: Font size
 * 		- PCD8544_FontSize_5x7
 * 		- PCD8544_FontSize_3x5
 * - PCD8544_PCD8544_Pixel_t color
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_Puts(char *c, PCD8544_Pixel_t color, PCD8544_FontSize_t size);

/**
 * Draw line on LCD
 *
 * Parameters:
 * - unsigned char x0: X coordinate of starting point
 * - unsigned char y0: Y coordinate of starting point
 * - unsigned char x1: X coordinate of ending point
 * - unsigned char y1: Y coordinate of ending point
 * - PCD8544_PCD8544_Pixel_t color
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_DrawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, PCD8544_Pixel_t color);

/**
 * Draw rectangle on LCD
 *
 * Parameters:
 * - unsigned char x0: X coordinate of top left point
 * - unsigned char y0: Y coordinate of top left point
 * - unsigned char x1: X coordinate of bottom right point
 * - unsigned char y1: Y coordinate of bottom right point
 * - PCD8544_PCD8544_Pixel_t color
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_DrawRectangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, PCD8544_Pixel_t color);

/**
 * Draw filled rectangle on LCD
 *
 * Parameters:
 * - unsigned char x0: X coordinate of top left point
 * - unsigned char y0: Y coordinate of top left point
 * - unsigned char x1: X coordinate of bottom right point
 * - unsigned char y1: Y coordinate of bottom right point
 * - PCD8544_PCD8544_Pixel_t color
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_DrawFilledRectangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, PCD8544_Pixel_t color);

/**
 * Draw circle on LCD
 *
 * Parameters:
 * - char x0: X coordinate of center circle point
 * - char y0: Y coordinate of center circle point
 * - char r: circle radius
 * - PCD8544_PCD8544_Pixel_t color
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_DrawCircle(char x0, char y0, char r, PCD8544_Pixel_t color);

/**
 * Draw filled circle on LCD
 *
 * Parameters:
 * - char x0: X coordinate of center circle point
 * - char y0: Y coordinate of center circle point
 * - char r: circle radius
 * - PCD8544_PCD8544_Pixel_t color
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_DrawFilledCircle(char x0, char y0, char r, PCD8544_Pixel_t color);

#endif
