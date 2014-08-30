/**
 *	ILI9341 library for STM32F4xx with SPI communication, without LTDC hardware
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/04/library-08-ili9341-lcd-on-stm32f429-discovery-board/
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
 *	This driver works for all STM32F4xx series with SPI built in.
 *	
 *	Default pinout
 *		ILI9341			STM32F4xx		DESCRIPTION
 *		
 *		SDO (MISO)		PF8				Output from LCD for SPI.	Not used, can be left
 *		LED				3.3V			Backlight
 *		SCK				PF7				SPI clock
 *		SDI (MOSI)		PF9				SPI master output
 *		WRX or D/C		PD13			Data/Command register
 *		RESET			PD12			Reset LCD
 *		CS				PC2				Chip select for SPI
 *		GND				GND				Ground
 *		VCC				3.3V			Positive power supply
 *		
 *	All pins can be changed in your defines.h file
 *		
 *	//Default SPI used is SPI5. Check my SPI library for other pinouts
 *	#define ILI9341_SPI 				SPI5
 *	#define ILI9341_SPI_PINS			TM_SPI_PinsPack_1
 *		
 *	//Default CS pin. Edit this in your defines.h file
 *	#define ILI9341_CS_CLK				RCC_AHB1Periph_GPIOC
 *	#define ILI9341_CS_PORT				GPIOC
 *	#define ILI9341_CS_PIN				GPIO_Pin_2
 *		
 *	//Default D/C (or WRX) pin. Edit this in your defines.h file
 *	#define ILI9341_WRX_CLK				RCC_AHB1Periph_GPIOD
 *	#define ILI9341_WRX_PORT			GPIOD
 *	#define ILI9341_WRX_PIN				GPIO_Pin_13
 *		
 *	Reset pin can be disabled, if you need GPIOs for other purpose.
 *	To disable RESET pin, add line below to defines.h file
 *	If you disable pin, then set LCD's RESET pin to VCC
 *	
 *	//Disable RESET pin
 *	#define ILI9341_USE_RST_PIN			0
 *		
 *	But if you want to use RESET pin, you can change its settings in defines.h file
 *	
 *	//Default RESET pin. Edit this in your defines.h file
 *	#define ILI9341_RST_CLK				RCC_AHB1Periph_GPIOD
 *	#define ILI9341_RST_PORT			GPIOD
 *	#define ILI9341_RST_PIN				GPIO_Pin_12
 */
#ifndef TM_ILI9341_H
#define TM_ILI9341_H 100
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - defines.h
 * - TM SPI
 * - TM FONTS
 */

/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_fonts.h"

//SPI used
//This SPI pins are used on STM32F429 Discovery board
#ifndef ILI9341_SPI
#define ILI9341_SPI 				SPI5
#define ILI9341_SPI_PINS			TM_SPI_PinsPack_1
#endif

#ifndef ILI9341_CS_PIN
//This pin is used on STM32F429 discovery board
#define ILI9341_CS_CLK				RCC_AHB1Periph_GPIOC
#define ILI9341_CS_PORT				GPIOC
#define ILI9341_CS_PIN				GPIO_Pin_2
#endif

#ifndef ILI9341_WRX_PIN
//This pin is used on STM32F429 discovery board
#define ILI9341_WRX_CLK				RCC_AHB1Periph_GPIOD
#define ILI9341_WRX_PORT			GPIOD
#define ILI9341_WRX_PIN				GPIO_Pin_13
#endif

//Reset pin
#ifndef ILI9341_RST_PIN
//Reset pin
#define ILI9341_RST_CLK				RCC_AHB1Periph_GPIOD
#define ILI9341_RST_PORT			GPIOD
#define ILI9341_RST_PIN				GPIO_Pin_12
#endif

#define ILI9341_RST_SET				GPIO_SetBits(ILI9341_RST_PORT, ILI9341_RST_PIN)
#define ILI9341_RST_RESET			GPIO_ResetBits(ILI9341_RST_PORT, ILI9341_RST_PIN)
#define ILI9341_CS_SET				GPIO_SetBits(ILI9341_CS_PORT, ILI9341_CS_PIN)
#define ILI9341_CS_RESET			GPIO_ResetBits(ILI9341_CS_PORT, ILI9341_CS_PIN)
#define ILI9341_WRX_SET				GPIO_SetBits(ILI9341_WRX_PORT, ILI9341_WRX_PIN)
#define ILI9341_WRX_RESET			GPIO_ResetBits(ILI9341_WRX_PORT, ILI9341_WRX_PIN)

//LCD settings
#define ILI9341_WIDTH 				240
#define ILI9341_HEIGHT				320
#define ILI9341_PIXEL				76800

//Colors
#define ILI9341_COLOR_WHITE			0xFFFF
#define ILI9341_COLOR_BLACK			0x0000
#define ILI9341_COLOR_RED			0xF800
#define ILI9341_COLOR_GREEN			0x07E0
#define ILI9341_COLOR_GREEN2		0xB723
#define ILI9341_COLOR_BLUE			0x001F
#define ILI9341_COLOR_BLUE2			0x051D
#define ILI9341_COLOR_YELLOW		0xFFE0
#define ILI9341_COLOR_ORANGE		0xFBE4
#define ILI9341_COLOR_CYAN			0x07FF
#define ILI9341_COLOR_MAGENTA		0xA254
#define ILI9341_COLOR_GRAY			0x7BEF //1111 0111 1101 1110
#define ILI9341_COLOR_BROWN			0xBBCA

//Commands
#define ILI9341_RESET				0x01
#define ILI9341_SLEEP_OUT			0x11
#define ILI9341_GAMMA				0x26
#define ILI9341_DISPLAY_OFF			0x28
#define ILI9341_DISPLAY_ON			0x29
#define ILI9341_COLUMN_ADDR			0x2A
#define ILI9341_PAGE_ADDR			0x2B
#define ILI9341_GRAM				0x2C
#define ILI9341_MAC					0x36
#define ILI9341_PIXEL_FORMAT		0x3A
#define ILI9341_WDB					0x51
#define ILI9341_WCD					0x53
#define ILI9341_RGB_INTERFACE		0xB0
#define ILI9341_FRC					0xB1
#define ILI9341_BPC					0xB5
#define ILI9341_DFC					0xB6
#define ILI9341_POWER1				0xC0
#define ILI9341_POWER2				0xC1
#define ILI9341_VCOM1				0xC5
#define ILI9341_VCOM2				0xC7
#define ILI9341_POWERA				0xCB
#define ILI9341_POWERB				0xCF
#define ILI9341_PGAMMA				0xE0
#define ILI9341_NGAMMA				0xE1
#define ILI9341_DTCA				0xE8
#define ILI9341_DTCB				0xEA
#define ILI9341_POWER_SEQ			0xED
#define ILI9341_3GAMMA_EN			0xF2
#define ILI9341_INTERFACE			0xF6
#define ILI9341_PRC					0xF7

/**
 * Public enum
 *
 * Select orientation for LCD
 */
typedef enum {
	TM_ILI9341_Orientation_Portrait_1,
	TM_ILI9341_Orientation_Portrait_2,
	TM_ILI9341_Orientation_Landscape_1,
	TM_ILI9341_Orientation_Landscape_2
} TM_ILI9341_Orientation_t;

/**
 * Orientation
 * Used private
 */
typedef enum {
	TM_ILI9341_Landscape,
	TM_ILI9341_Portrait
} TM_ILI9341_Orientation;

/**
 * LCD options
 * Used private
 */
typedef struct {
	uint16_t width;
	uint16_t height;
	TM_ILI9341_Orientation orientation; // 1 = portrait; 0 = landscape
} TM_ILI931_Options_t;


/**
 * Select font
 */
extern TM_FontDef_t TM_Font_7x10;
extern TM_FontDef_t TM_Font_11x18;
extern TM_FontDef_t TM_Font_16x26;

/**
 * Initialize ILI9341 LCD
 *
 */
extern void TM_ILI9341_Init(void);

/**
 * Init LCD pins
 *
 * Called private
 */
extern void TM_ILI9341_InitLCD(void);

/**
 * Send data to LCD via SPI
 *
 * Called private
 *
 * Parameters:
 * 	- uint8_t data: data to be sent
 */
extern void TM_ILI9341_SendData(uint8_t data);

/**
 * Send command to LCD via SPI
 *
 * Called private
 *
 * Parameters:
 * 	- uint8_t data: data to be sent
 */
extern void TM_ILI9341_SendCommand(uint8_t data);

/**
 * Simple delay
 *
 * Parameters:
 * 	- volatile unsigned int delay: clock cycles
 */
extern void TM_ILI9341_Delay(volatile unsigned int delay);

/**
 * Set cursor position
 *
 * Called private
 */
extern void TM_ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * Draw single pixel to LCD
 *
 * Parameters:
 * 	- uint16_t x: X position for pixel
 * 	- uint16_t y: Y position for pixel
 * 	- uint16_t color: color of pixel
 */
extern void TM_ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);

/**
 * Fill entire LCD with color
 *
 * Parameters:
 * 	- uint16_t color: Color to be used in fill
 */
extern void TM_ILI9341_Fill(uint16_t color);

/**
 * Rotate LCD
 * Select orientation
 *
 * Parameters:
 * 	- TM_ILI9341_Orientation_t orientation
 *		- TM_ILI9341_Orientation_Portrait_1: no rotation
 *		- TM_ILI9341_Orientation_Portrait_2: rotate 180deg
 *		- TM_ILI9341_Orientation_Landscape_1: rotate 90deg
 *		- TM_ILI9341_Orientation_Landscape_2: rotate -90deg
 */
extern void TM_ILI9341_Rotate(TM_ILI9341_Orientation_t orientation);

/**
 * Put single character to LCD
 *
 * Parameters:
 * 	- uint16_t x: X position of top left corner
 * 	- uint16_t y: Y position of top left corner
 * 	- char c: character to be displayed
 * 	- TM_FontDef_t *font: pointer to used font
 * 	- uint16_t foreground: color for char
 * 	- uint16_t background: color for char background
 */
extern void TM_ILI9341_Putc(uint16_t x, uint16_t y, char c, TM_FontDef_t *font, uint16_t foreground, uint16_t background);

/**
 * Put string to LCD
 *
 * Parameters:
 * 	- uint16_t x: X position of top left corner of first character in string
 * 	- uint16_t y: Y position of top left corner of first character in string
 * 	- char *str: pointer to first character
 * 	- TM_FontDef_t *font: pointer to used font
 * 	- uint16_t foreground: color for string
 * 	- uint16_t background: color for string background
 */
extern void TM_ILI9341_Puts(uint16_t x, uint16_t y, char *str, TM_FontDef_t *font, uint16_t foreground, uint16_t background);

/**
 * Get width and height of box with text
 *
 * Parameters:
 * 	- char *str: pointer to first character
 * 	- TM_FontDef_t *font: pointer to used font
 * 	- uint16_t *width: Pointer to variable to store width
 * 	- uint16_t *height: ointer to variable to store height
 */
extern void TM_ILI9341_GetStringSize(char *str, TM_FontDef_t *font, uint16_t *width, uint16_t *height);

/**
 * Draw line to LCD
 *
 * Parameters:
 * 	- uint16_t x0: X coordinate of starting point
 * 	- uint16_t y0: Y coordinate of starting point
 * 	- uint16_t x1: X coordinate of ending point
 * 	- uint16_t y1: Y coordinate of ending point
 * 	- uint16_t color: line color
 */
extern void TM_ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

/**
 * Draw rectangle on LCD
 *
 * Parameters:
 * - uint16_t x0: X coordinate of top left point
 * - uint16_t y0: Y coordinate of top left point
 * - uint16_t x1: X coordinate of bottom right point
 * - uint16_t y1: Y coordinate of bottom right point
 * - uint16_t color: rectangle color
 */
extern void TM_ILI9341_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

/**
 * Draw filled rectangle on LCD
 *
 * Parameters:
 * - uint16_t x0: X coordinate of top left point
 * - uint16_t y0: Y coordinate of top left point
 * - uint16_t x1: X coordinate of bottom right point
 * - uint16_t y1: Y coordinate of bottom right point
 * - uint16_t color: rectangle color
 */
extern void TM_ILI9341_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

/**
 * Draw circle on LCD
 *
 * Parameters:
 * - int16_t x0: X coordinate of center circle point
 * - int16_t y0: Y coordinate of center circle point
 * - int16_t r: circle radius
 * - uint16_t color: circle color
 */
extern void TM_ILI9341_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

/**
 * Draw filled on LCD
 *
 * Parameters:
 * - int16_t x0: X coordinate of center circle point
 * - int16_t y0: Y coordinate of center circle point
 * - int16_t r: circle radius
 * - uint16_t color: circle color
 */
extern void TM_ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

#endif

