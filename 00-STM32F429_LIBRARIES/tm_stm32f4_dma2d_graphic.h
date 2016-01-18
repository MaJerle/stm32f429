/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/01/library-51-chrom-art-accelerator-dma2d-graphic-library-on-stm32f429-discovery
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Graphic library for LCD using DMA2D for transferring graphic data to memory for LCD display
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
#ifndef TM_DMA2DGRAPHIC_H
#define TM_DMA2DGRAPHIC_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_DMA2D_GRAPHIC
 * @brief    Graphic library for LCD using DMA2D for transferring graphic data to memory for LCD display - http://stm32f4-discovery.com/2015/01/library-51-chrom-art-accelerator-dma2d-graphic-library-on-stm32f429-discovery
 * @{
 *
 * This DMA2D graphic library is used only for graphic data transmission to memory.
 * You still need some peripheral (LTDC, SPI DMA, etc) to transmit data from memory
 * to LCD.
 * @note  In case of STM32F429-Discovery, LTDC is in use and this peripheral just writes data to external RAM address.
 *
 * \par Customize default settings
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
 * Transmissions between memory is very fast which allows you to make smooth transmissions.
 *
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
 - STM32F4xx DMA2D
 - defines.h
@endverbatim
 */
 
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma2d.h"
#include "defines.h"

/**
 * @defgroup TM_DMA2D_GRAPHIC_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  Default LCD width in pixels
 */
#ifndef DMA2D_GRAPHIC_LCD_WIDTH
	#if defined(DMA2D_GRAPHIC_USE_STM324x9_EVAL) || defined(TM_DISCO_STM324x9_EVAL)
		#define DMA2D_GRAPHIC_LCD_WIDTH     640 /*!< STM32439-Eval board */
	#else
		#define DMA2D_GRAPHIC_LCD_WIDTH     240 /*!< STM32F429-Discovery board */
	#endif
#endif

/**
 * @brief  Default LCD height in pixels
 */
#ifndef DMA2D_GRAPHIC_LCD_HEIGHT
	#if defined(DMA2D_GRAPHIC_USE_STM324x9_EVAL) || defined(TM_DISCO_STM324x9_EVAL)
		#define DMA2D_GRAPHIC_LCD_HEIGHT    480 /*!< STM32439-Eval board */
	#else
		#define DMA2D_GRAPHIC_LCD_HEIGHT    320 /*!< STM32F429-Discovery board */
	#endif
#endif

/**
 * @brief  RAM Start address for LCD
 * @note   On STM32F429-Discovery, this is address for SDRAM which operate with LCD and LTDC peripheral
 */
#ifndef DMA2D_GRAPHIC_RAM_ADDR
	#if defined(DMA2D_GRAPHIC_USE_STM324x9_EVAL) || defined(TM_DISCO_STM324x9_EVAL)
		#define DMA2D_GRAPHIC_RAM_ADDR      0xC0000000 /*!< STM32439-Eval board */
	#else
		#define DMA2D_GRAPHIC_RAM_ADDR      0xD0000000 /*!< STM32F429-Discovery board */
	#endif
#endif

/**
 * @brief  Timeout for DMA2D
 */
#ifndef DMA2D_GRAPHIC_TIMEOUT
#define DMA2D_GRAPHIC_TIMEOUT       (uint32_t)10000000
#endif
/**
 * @brief  Number of LCD pixels
 */
#define DMA2D_GRAPHIC_PIXELS        DMA2D_GRAPHIC_LCD_WIDTH * DMA2D_GRAPHIC_LCD_HEIGHT

/**
 * @defgroup TM_DMA2D_GRAPHIC_COLORS
 * @brief    Colors for DMA2D graphic library in RGB565 format
 *
 * @{
 */
 
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

/* Waiting flags */
#define DMA2D_WORKING               ((DMA2D->CR & DMA2D_CR_START))
#define DMA2D_WAIT                  do { while (DMA2D_WORKING); DMA2D->IFCR = DMA2D_IFSR_CTCIF;} while (0);

/**
 * @}
 */

/**
 * @}
 */
 
/**
 * @defgroup TM_DMA2D_GRAPHIC_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Structure for polygon line
 * @note   If you have big poly line, you can use array of this structure for more coordinates
 */
typedef struct {
	uint16_t X; /*!< X coordinate for poly line */
	uint16_t Y; /*!< Y coordinate for poly line */
} TM_DMA2DRAPHIC_Poly_t;

/**
 * @brief  Configuration structure
 * @note   Meant for private use
 */
typedef struct {
	uint16_t Width;
	uint16_t Height;
	uint8_t BytesPerPixel;
	uint32_t BufferStart;
	uint32_t BufferOffset;
	uint8_t Orientation;
} TM_DMA2DGRAPHIC_INT_Conf_t;

/**
 * @}
 */

/**
 * @defgroup TM_DMA2D_GRAPHIC_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes and prepare DMA2D for working.
 * @note   This function has to be called before anything can be used
 * @param  None
 * @retval None
 */
void TM_DMA2DGRAPHIC_Init(void);

/**
 * @brief  Sets layer for DMA2D working memory.
 * @note   This functions just works in memory, so when you set layer,
 *         basically just address offset is used and changed.
 *         
 * @note   To show anything on LCD you need LTDC or anything else for transmission
 * @param  layer_number: Layer number, starting from 1 to infinity.
 *            You are limited by your LCD size and available memory size
 * @retval None
 */
void TM_DMA2DGRAPHIC_SetLayer(uint8_t layer_number);

/**
 * @brief  Sets orientation for DMA2D peripheral
 *
 * @note   You have to match DMA2D graphic library with your LCD orientation in order to display correct.
 * @param  orientation: Memory orientation for your LCD
 *            - 0: Normal, no orientation.
 *            - 1: 180 Degrees orientation
 *            - 2: 90 Degrees orientation
 *            - 3: 270 Degrees orientation
 * @retval None
 */
void TM_DMA2DGRAPHIC_SetOrientation(uint8_t orientation);

/**
 * @brief  Fills entire LCD memory layer with custom color
 * @param  color: Color in RGB565 format to use for LCD fill
 * @retval None
 */
void TM_DMA2DGRAPHIC_Fill(uint32_t color);

/**
 * @brief  Draws single pixel on currently active layer
 * @param  x: X coordinate on LCD
 * @param  y: Y coordinate on LCD
 * @param  color: Pixel color in RGB565 format
 * @retval None
 */
void TM_DMA2DGRAPHIC_DrawPixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief  Gets single pixel on currently active layer
 * @param  x: X coordinate on LCD
 * @param  y: Y coordinate on LCD
 * @retval Pixel color in RGB565 format
 */
uint32_t TM_DMA2DGRAPHIC_GetPixel(uint16_t x, uint16_t y);

/**
 * @brief  Draws vertical line on currently active layer
 * @param  x: X coordinate on LCD
 * @param  y: Y coordinate on LCD
 * @param  length: Vertical line length in pixels
 * @param  color: Color in RGB565 format
 * @retval None
 */
void TM_DMA2DGRAPHIC_DrawVerticalLine(int16_t x, int16_t y, uint16_t length, uint32_t color);

/**
 * @brief  Draws horizontal line on currently active layer
 * @param  x: X coordinate on LCD
 * @param  y: Y coordinate on LCD
 * @param  length: Horizontal line length in pixels
 * @param  color: Color in RGB565 format
 * @retval None
 */
void TM_DMA2DGRAPHIC_DrawHorizontalLine(int16_t x, int16_t y, uint16_t length, uint32_t color);

/**
 * @brief  Draws line on currently active layer
 * @param  x1: X1 coordinate on LCD
 * @param  y1: Y1 coordinate on LCD
 * @param  x2: X1 coordinate on LCD
 * @param  y2: Y1 coordinate on LCD
 * @param  color: Color in RGB565 format
 * @retval None
 */
void TM_DMA2DGRAPHIC_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color);

/**
 * @brief  Draws polygon line on currently active layer
 * @param  *Coordinates: Pointer to @ref TM_DMA2DRAPHIC_Poly_t array of coordinates
 * @param  count: Number of coordinates
 * @param  color: Color in RGB565 format
 * @retval None
 */
void TM_DMA2DGRAPHIC_DrawPolyLine(TM_DMA2DRAPHIC_Poly_t* Coordinates, uint16_t count, uint32_t color);

/**
 * @brief  Draws rectangle on currently active layer
 * @param  x: Top left X location for rectangle on LCD
 * @param  y: Top left Y location for rectangle on LCD
 * @param  width: Rectangle width in pixels
 * @param  height: Rectangle height in pixels
 * @param  color: Color in RGB565 format
 * @retval None
 */
void TM_DMA2DGRAPHIC_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief  Draws filled rectangle on currently active layer
 * @param  x: Top left X location for rectangle on LCD
 * @param  y: Top left Y location for rectangle on LCD
 * @param  width: Rectangle width in pixels
 * @param  height: Rectangle height in pixels
 * @param  color: Color in RGB565 format
 * @retval None
 */
void TM_DMA2DGRAPHIC_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief  Draws rounded rectangle on currently active layer
 * @param  x: Top left X location for rectangle on LCD
 * @param  y: Top left Y location for rectangle on LCD
 * @param  width: Rectangle width in pixels
 * @param  height: Rectangle height in pixels
 * @param  r: Corner radius in pixels
 * @param  color: Color in RGB565 format
 * @retval None
 */
void TM_DMA2DGRAPHIC_DrawRoundedRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t r, uint32_t color);

/**
 * @brief  Draws filled rounded rectangle on currently active layer
 * @param  x: Top left X location for rectangle on LCD
 * @param  y: Top left Y location for rectangle on LCD
 * @param  width: Rectangle width in pixels
 * @param  height: Rectangle height in pixels
 * @param  r: Corner radius in pixels
 * @param  color: Color in RGB565 format
 * @retval None
 */
void TM_DMA2DGRAPHIC_DrawFilledRoundedRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t r, uint32_t color);

/**
 * @brief  Draws circle on currently active layer
 * @param  x0: X coordinate of circle center on LCD
 * @param  y0: Y coordinate of circle center on LCD
 * @param  r: Circle radius in pixels
 * @param  color: Color in RGB565 format
 * @retval None
 */
void TM_DMA2DGRAPHIC_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color);

/**
 * @brief  Draws filed circle on currently active layer
 * @param  x0: X coordinate of circle center on LCD
 * @param  y0: Y coordinate of circle center on LCD
 * @param  r: Circle radius in pixels
 * @param  color: Color in RGB565 format
 * @retval None
 */
void TM_DMA2DGRAPHIC_DrawFilledCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color);

/**
 * @brief  Draws triangle on currently active layer
 * @param  x1: First point, X coordinate
 * @param  y1: First point, Y coordinate
 * @param  x2: Second point, X coordinate
 * @param  y2: Second point, Y coordinate
 * @param  x3: Third point, X coordinate
 * @param  y3: Third point, Y coordinate
 * @param  color: Color in RGB565 format
 * @retval None
 */
void TM_DMA2DGRAPHIC_DrawTriangle(uint16_t x1, uint16_t y1,  uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color);

/**
 * @brief  Draws filled triangle on currently active layer
 * @param  x1: First point, X coordinate
 * @param  y1: First point, Y coordinate
 * @param  x2: Second point, X coordinate
 * @param  y2: Second point, Y coordinate
 * @param  x3: Third point, X coordinate
 * @param  y3: Third point, Y coordinate
 * @param  color: Color in RGB565 format
 * @retval None
 */
void TM_DMA2DGRAPHIC_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color);

void TM_DMA2DGRAPHIC_CopyBuffer(void* pSrc, void* pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLineSrc, uint32_t OffLineDst);
void TM_DMA2DGRAPHIC_CopyBufferIT(void* pSrc, void* pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLineSrc, uint32_t OffLineDst);

/* Private functions */
void TM_INT_DMA2DGRAPHIC_SetConf(TM_DMA2DGRAPHIC_INT_Conf_t* Conf);

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
