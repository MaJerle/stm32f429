/**
 *	Graphic library for LCD using DMA2D for transferring data to memory
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2015/01/library-51-chrom-art-accelerator-dma2d-graphic-library-on-stm32f429-discovery
 *	@version 	v1.0
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2015
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
 * This DMA2D graphic library is used only for graphic data transmission to memory.
 * You still need some peripheral (LTDC, SPI DMA, etc) to transmit data from memory
 * to LCD.
 * In case of STM32F429-Discovery, LTDC is in use and this peripheral just writes data
 * to external RAM address.
 *
 * If you want to make custom application, you can use this library for that.
 * Changes you need to make are:
 * 	- Set custom start address in memory where pixel 0,0 is for LCD
 * 	- Set LCD width and height
 *
 * With this library you can also rotate LCD.
 * This allows you to draw graphic in different LCD orientation.
 *
 * Also, this library should be used for moving elements on screen, like playing movies.
 * Transmissions between memory is very fast which allows you to make smooth transmissions
 */
#ifndef TM_DMA2DGRAPHIC_H
#define TM_DMA2DGRAPHIC_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx DMA2D
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma2d.h"
#include "defines.h"

/* Default LCD width in pixels */
#ifndef DMA2D_GRAPHIC_LCD_WIDTH
#define DMA2D_GRAPHIC_LCD_WIDTH		240
#endif
/* Default LCD height in pixels */
#ifndef DMA2D_GRAPHIC_LCD_HEIGHT
#define DMA2D_GRAPHIC_LCD_HEIGHT	320
#endif
/* Number of LCD pixels */
#ifndef DMA2D_GRAPHIC_PIXELS
#define DMA2D_GRAPHIC_PIXELS		DMA2D_GRAPHIC_LCD_WIDTH * DMA2D_GRAPHIC_LCD_HEIGHT
#endif
/* RAM Start address for LCD */
/* On STM32F429-Discovery, this is address for SDRAM which operate with LCD and LTDC peripheral */
#ifndef DMA2D_GRAPHIC_RAM_ADDR
#define DMA2D_GRAPHIC_RAM_ADDR		0xD0000000
#endif
/* Timeout for DMA2D */
#ifndef DMA2D_GRAPHIC_TIMEOUT
#define DMA2D_GRAPHIC_TIMEOUT		(uint32_t)1000000
#endif

/* Colors */
#define GRAPHIC_COLOR_WHITE			0xFFFF
#define GRAPHIC_COLOR_BLACK			0x0000
#define GRAPHIC_COLOR_RED			0xF800
#define GRAPHIC_COLOR_GREEN			0x07E0
#define GRAPHIC_COLOR_GREEN2		0xB723
#define GRAPHIC_COLOR_BLUE			0x001F
#define GRAPHIC_COLOR_BLUE2			0x051D
#define GRAPHIC_COLOR_YELLOW		0xFFE0
#define GRAPHIC_COLOR_ORANGE		0xFBE4
#define GRAPHIC_COLOR_CYAN			0x07FF
#define GRAPHIC_COLOR_MAGENTA		0xA254
#define GRAPHIC_COLOR_GRAY			0x7BEF
#define GRAPHIC_COLOR_BROWN			0xBBCA

/**
 * Struct for polygon line
 *
 * Parameters:
 *	- uint16_t X:
 *		X coordinate for poly line
 *	- uint16_t Y:
 *		Y coordinate for poly line
 */
typedef struct {
	uint16_t X;
	uint16_t Y;
} TM_DMA2DRAPHIC_Poly_t;

/**
 * Initialize and prepare DMA2D for working.
 *
 * This function has to be called before anything can be used
 */
void TM_DMA2DGRAPHIC_Init(void);

/**
 * Set layer for LCD
 *
 * This functions just works in memory, so when you set layer,
 * basically just address offset is used.
 * To show anything on LCD you need LTDC or anything else for transmission
 *
 * Parameters:
 * 	- uint8_t layer_number:
 * 		Layer number, starting from 1 to infinity.
 *
 * No return
 */
void TM_DMA2DGRAPHIC_SetLayer(uint8_t layer_number);

/**
 * Set orientation for DMA2D peripheral
 *
 * You have to match DMA2D graphic library with your LCD orientation in order to display correct.
 *
 * Parameters:
 * 	- uint8_t orientation:
 * 		- 0: Normal, no orientation.
 * 		- 1: 180 Degrees orientation
 * 		- 2: 90 Degrees orientation
 * 		- 3: 270 Degrees orientation
 */
/* 0: normal, 1: 180%, 2: 90 degrees, 3: 20 degrees */
void TM_DMA2DGRAPHIC_SetOrientation(uint8_t orientation);

/**
 * Fill entire LCD layer with custom color
 *
 * Parameters:
 * 	- uint32_t color:
 * 		Color in RGB565 format
 *
 * No return
 */
void TM_DMA2DGRAPHIC_Fill(uint32_t color);

/**
 * Draw single pixel on currently active layer
 *
 * Parameters:
 * 	- uint16_t x:
 * 		X coordinate on LCD
 * 	- uint16_t y:
 * 		Y coordinate on LCD
 * 	- uint32_t color:
 * 		Pixel color in RGB565 format
 *
 * No return
 */
void TM_DMA2DGRAPHIC_DrawPixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * Get single pixel on currently active layer
 *
 * Parameters:
 * 	- uint16_t x:
 * 		X coordinate on LCD
 * 	- uint16_t y:
 * 		Y coordinate on LCD
 *
 * RGB565 color format returned
 */
uint32_t TM_DMA2DGRAPHIC_GetPixel(uint16_t x, uint16_t y);

/**
 * Draw vertical line on currently active layer
 *
 * Parameters:
 * 	- int16_t x:
 * 		X coordinate on LCD
 * 	- int16_t y:
 * 		Y coordinate on LCD
 * 	- uint16_t length:
 * 		Vertical line length in pixels
 * 	- uint32_t color:
 * 		Color in RGB565 format
 *
 * No return
 */
void TM_DMA2DGRAPHIC_DrawVerticalLine(int16_t x, int16_t y, uint16_t length, uint32_t color);

/**
 * Draw Horizontal line on currently active layer
 *
 * Parameters:
 * 	- int16_t x:
 * 		X coordinate on LCD
 * 	- int16_t y:
 * 		Y coordinate on LCD
 * 	- uint16_t length:
 * 		Horizontal line length in pixels
 * 	- uint32_t color:
 * 		Color in RGB565 format
 *
 * No return
 */
void TM_DMA2DGRAPHIC_DrawHorizontalLine(int16_t x, int16_t y, uint16_t length, uint32_t color);

/**
 * Draw line on currently active layer
 *
 * Parameters:
 * 	- int16_t x1:
 * 		X1 coordinate on LCD
 * 	- int16_t y1:
 * 		Y1 coordinate on LCD
 * 	- int16_t x2:
 * 		X1 coordinate on LCD
 * 	- int16_t y2:
 * 		Y1 coordinate on LCD
 * 	- uint32_t color:
 * 		Color in RGB565 format
 *
 * No return
 */
void TM_DMA2DGRAPHIC_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color);

/**
 * Draw polygon line on currently active layer
 *
 * Parameters:
 * 	- TM_DMA2DRAPHIC_Poly_t* Coordinates:
 * 		Pointer to TM_DMA2DRAPHIC_Poly_t array of coordinates
 * 	- uint16_t count:
 * 		Number of coordinates
 * 	- uint32_t color:
 * 		color in RGB565 format
 *
 * No return
 */
void TM_DMA2DGRAPHIC_DrawPolyLine(TM_DMA2DRAPHIC_Poly_t* Coordinates, uint16_t count, uint32_t color);

/**
 * Draw rectangle on currently active layer
 *
 * Parameters:
 * 	- uint16_t x:
 * 		Top left X location for rectangle on LCD
 * 	- uint16_t y:
 * 		Top left Y location for rectangle on LCD
 * 	- uint16_t width:
 * 		Rectangle width in pixels
 * 	- uint16_t height:
 * 		Rectangle height in pixels
 * 	- uint32_t color:
 * 		Color in RGB565 format
 *
 * No return
 */
void TM_DMA2DGRAPHIC_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * Draw filled rectangle on currently active layer
 *
 * Parameters:
 * 	- uint16_t x:
 * 		Top left X location for rectangle on LCD
 * 	- uint16_t y:
 * 		Top left Y location for rectangle on LCD
 * 	- uint16_t width:
 * 		Rectangle width in pixels
 * 	- uint16_t height:
 * 		Rectangle height in pixels
 * 	- uint32_t color:
 * 		Color in RGB565 format
 *
 * No return
 */
void TM_DMA2DGRAPHIC_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * Draw rounded rectangle on currently active layer
 *
 * Parameters:
 * 	- uint16_t x:
 * 		Top left X location for rectangle on LCD
 * 	- uint16_t y:
 * 		Top left Y location for rectangle on LCD
 * 	- uint16_t width:
 * 		Rectangle width in pixels
 * 	- uint16_t height:
 * 		Rectangle height in pixels
 *	- uint16_t r:
 *		Corner radius in pixels
 * 	- uint32_t color:
 * 		Color in RGB565 format
 *
 * No return
 */
void TM_DMA2DGRAPHIC_DrawRoundedRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t r, uint32_t color);

/**
 * Draw filled rounded rectangle on currently active layer
 *
 * Parameters:
 * 	- uint16_t x:
 * 		Top left X location for rectangle on LCD
 * 	- uint16_t y:
 * 		Top left Y location for rectangle on LCD
 * 	- uint16_t width:
 * 		Rectangle width in pixels
 * 	- uint16_t height:
 * 		Rectangle height in pixels
 *	- uint16_t r:
 *		Corner radius in pixels
 * 	- uint32_t color:
 * 		Color in RGB565 format
 *
 * No return
 */
void TM_DMA2DGRAPHIC_DrawFilledRoundedRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t r, uint32_t color);

/**
 * Draw circle on currently active layer
 *
 * Parameters:
 * 	- uint16_t x0:
 * 		X coordinate of circle center on LCD
 * 	- uint16_t y0:
 * 		Y coordinate of circle center on LCD
 * 	- uint16_t r:
 * 		Circle radius
 * 	- uint32_t color:
 * 		Color in RGB565 format
 *
 * No return
 */
void TM_DMA2DGRAPHIC_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color);

/**
 * Draw filed circle on currently active layer
 *
 * Parameters:
 * 	- uint16_t x0:
 * 		X coordinate of circle center on LCD
 * 	- uint16_t y0:
 * 		Y coordinate of circle center on LCD
 * 	- uint16_t r:
 * 		Circle radius
 * 	- uint32_t color:
 * 		Color in RGB565 format
 *
 * No return
 */
void TM_DMA2DGRAPHIC_DrawFilledCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color);

/**
 * Draw triangle on currently active layer
 *
 * Parameters:
 * 	- uint16_t x1:
 * 		First point, X coordinate
 * 	- uint16_t y1:
 * 		First point, Y coordinate
 * 	- uint16_t x2:
 * 		Second point, X coordinate
 * 	- uint16_t y2:
 * 		Second point, Y coordinate
 * 	- uint16_t x3:
 * 		Third point, X coordinate
 * 	- uint16_t y3:
 * 		Third point, Y coordinate
 * 	- uint32_t color:
 * 		Color in RGB565 format
 *
 * No return
 */
void TM_DMA2DGRAPHIC_DrawTriangle(uint16_t x1, uint16_t y1,  uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color);

/**
 * Draw filled triangle on currently active layer
 *
 * Parameters:
 * 	- uint16_t x1:
 * 		First point, X coordinate
 * 	- uint16_t y1:
 * 		First point, Y coordinate
 * 	- uint16_t x2:
 * 		Second point, X coordinate
 * 	- uint16_t y2:
 * 		Second point, Y coordinate
 * 	- uint16_t x3:
 * 		Third point, X coordinate
 * 	- uint16_t y3:
 * 		Third point, Y coordinate
 * 	- uint32_t color:
 * 		Color in RGB565 format
 *
 * No return
 */
void TM_DMA2DGRAPHIC_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
