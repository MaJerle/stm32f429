/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/06/library-18-ili9341-ltdc-stm32f429-discovery/
 * @version v1.4
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   ILI9341 library for LCD on STM32F429 Discovery using LTDC and external ram
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
#ifndef TM_ILI9341_LTDC_H
#define TM_ILI9341_LTDC_H 140

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_ILI9341_LTDC
 * @brief    ILI9341 library for LCD on STM32F429 Discovery, with LTDC hardware support - http://stm32f4-discovery.com/2014/06/library-18-ili9341-ltdc-stm32f429-discovery/
 * @{
 *
 * @note     Only works for STM32F4(2/3)9-Discovery because it has LTDC inside and external memory for data
 * 
 * \par Pinout on STM32F429-Discovery board
 *
@verbatim
PA3  <-> B5    | PB0  <-> R3 | PC6  <-> HSYNC | PD3  <-> G7 | PF10 <-> ENABLE | PG6  <-> R7     |
PA4  <-> VSYNC | PB1  <-> R6 | PC7  <-> G6    | PD6  <-> B2 |                 | PG7  <-> DOTCLK |
PA6  <-> G2    | PB8  <-> B6 | PC10 <-> R2    |             |                 | PB10 <-> G3     |
PA11 <-> R4    | PB9  <-> B7 |                |             |                 | PG11 <-> B3     |
PA12 <-> R5    | PB10 <-> G4 |                |             |                 | PG12 <-> B4     |
               | PB11 <-> G5 |                |             |                 |                 |
@endverbatim
 *
 * \par Changelog
 *
@verbatim
 Version 1.4
  - March 14, 2015
  - Added support for new GPIO system
  - Added functions TM_ILI9341_DisplayOff() and TM_ILI9341_DisplayOn()
 
 Version 1.3
  - January 21, 2015,
  - Added support for rounded rectangles
	
 Version 1.2
  - December 27, 2014
  - Added transparent option for strings and chars
	
 Version 1.1
  - August 07, 2014
  - Added scroll down and scroll up features

 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - STM32F4xx GPIO
 - STM32F4xx LTDC
 - defines.h
 - TM SPI
 - TM FONTS
 - TM SDRAM
 - TM GPIO
@endverbatim
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_ltdc.h"
#include "defines.h"
#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_fonts.h"
#include "tm_stm32f4_sdram.h"
#include "tm_stm32f4_gpio.h"

/**
 * @defgroup TM_ILI9341_LTDC_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  This SPI pins are used on STM32F429-Discovery board
 */
#ifndef ILI9341_SPI
#define ILI9341_SPI 				SPI5
#define ILI9341_SPI_PINS			TM_SPI_PinsPack_1
#endif

/**
 * @brief  CS PIN for SPI, used as on STM32F429-Discovery board
 */
#ifndef ILI9341_CS_PIN
#define ILI9341_CS_PORT				GPIOC
#define ILI9341_CS_PIN				GPIO_PIN_2
#endif

/**
 * @brief  WRX PIN for data/command, used as on STM32F429-Discovery board
 */
#ifndef ILI9341_WRX_PIN
#define ILI9341_WRX_PORT			GPIOD
#define ILI9341_WRX_PIN				GPIO_PIN_13
#endif

/**
 * @brief  Colors for LCD in RGB565 format
 */
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
#define ILI9341_COLOR_GRAY			0x7BEF
#define ILI9341_COLOR_BROWN			0xBBCA

/** 
 * @brief  Transparent background, only for chars and strings
 * @note   Use this as background when you use @ref TM_ILI9341_Putc or @ref TM_ILI9341_Puts
 *         if you want transparent background
 */
#define ILI9341_TRANSPARENT			0x80000000

/**
 * @}
 */
 
/**
 * @defgroup TM_ILI9341_LTDC_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Possible orientations for LCD
 */
typedef enum {
	TM_ILI9341_Orientation_Portrait_1,  /*!< Portrait orientation mode 1 */
	TM_ILI9341_Orientation_Portrait_2,  /*!< Portrait orientation mode 2 */
	TM_ILI9341_Orientation_Landscape_1, /*!< Landscape orientation mode 1 */
	TM_ILI9341_Orientation_Landscape_2  /*!< Landscape orientation mode 2 */
} TM_ILI9341_Orientation_t;

/**
 * @}
 */

/**
 * @defgroup TM_ILI9341_LTDC_Functions
 * @brief    Library Functions
 * @{
 */
/**
 * @brief  Initializes ILI9341 LCD with LTDC peripheral
 *         It also initializes external SDRAM
 * @param  None
 * @retval None
 */
void TM_ILI9341_Init(void);

/**
 * @brief  Draws single pixel to LCD
 * @param  x: X position for pixel
 * @param  y: Y position for pixel
 * @param  color: Color of pixel
 * @retval None
 */
void TM_ILI9341_DrawPixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief  Fills entire LCD with color
 * @param  color: Color to be used in fill
 * @retval None
 */
void TM_ILI9341_Fill(uint32_t color);

/**
 * @brief  Rotates LCD to specific orientation
 * @param  orientation: LCD orientation. This parameter can be a value of @ref TM_ILI9341_Orientation_t enumeration
 * @retval None
 */
void TM_ILI9341_Rotate(TM_ILI9341_Orientation_t orientation);

/**
 * @brief  Puts single character to LCD
 * @param  x: X position of top left corner
 * @param  y: Y position of top left corner
 * @param  c: Character to be displayed
 * @param  *font: Pointer to @ref TM_FontDef_t used font
 * @param  foreground: Color for char
 * @param  background: Color for char background
 * @retval None
 */
void TM_ILI9341_Putc(uint16_t x, uint16_t y, char c, TM_FontDef_t* font, uint32_t foreground, uint32_t background);

/**
 * @brief  Puts string to LCD
 * @param  x: X position of top left corner of first character in string
 * @param  y: Y position of top left corner of first character in string
 * @param  *str: Pointer to first character
 * @param  *font: Pointer to @ref TM_FontDef_t used font
 * @param  foreground: Color for string
 * @param  background: Color for string background
 * @retval None
 */
void TM_ILI9341_Puts(uint16_t x, uint16_t y, char* str, TM_FontDef_t *font, uint32_t foreground, uint32_t background);

/**
 * @brief  Gets width and height of box with text
 * @param  *str: Pointer to first character
 * @param  *font: Pointer to @ref TM_FontDef_t used font
 * @param  *width: Pointer to variable to store width
 * @param  *height: Pointer to variable to store height
 * @retval None
 */
void TM_ILI9341_GetStringSize(char* str, TM_FontDef_t* font, uint16_t* width, uint16_t* height);

/**
 * @brief  Draws line to LCD
 * @param  x0: X coordinate of starting point
 * @param  y0: Y coordinate of starting point
 * @param  x1: X coordinate of ending point
 * @param  y1: Y coordinate of ending point
 * @param  color: Line color
 * @retval None
 */
void TM_ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);

/**
 * @brief  Draws rectangle on LCD
 * @param  x0: X coordinate of top left point
 * @param  y0: Y coordinate of top left point
 * @param  x1: X coordinate of bottom right point
 * @param  y1: Y coordinate of bottom right point
 * @param  color: Rectangle color
 * @retval None
 */
void TM_ILI9341_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);

/**
 * @brief  Draws filled rectangle on LCD
 * @param  x0: X coordinate of top left point
 * @param  y0: Y coordinate of top left point
 * @param  x1: X coordinate of bottom right point
 * @param  y1: Y coordinate of bottom right point
 * @param  color: Rectangle color
 * @retval None
 */
void TM_ILI9341_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);

/**
 * @brief  Draws rounded rectangle on LCD
 * @param  x0: X coordinate of top left point
 * @param  y0: Y coordinate of top left point
 * @param  x1: X coordinate of bottom right point
 * @param  y1: Y coordinate of bottom right point
 * @param  r: Radius for corners
 * @param  color: Rectangle color
 * @retval None
 */
void TM_ILI9341_DrawRoundedRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t r, uint32_t color);

/**
 * @brief  Draws filled rounded rectangle on LCD
 * @param  x0: X coordinate of top left point
 * @param  y0: Y coordinate of top left point
 * @param  x1: X coordinate of bottom right point
 * @param  y1: Y coordinate of bottom right point
 * @param  r: Radius for corners
 * @param  color: Rectangle color
 * @retval None
 */
void TM_ILI9341_DrawFilledRoundedRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t r, uint32_t color);

/**
 * @brief  Draws circle on LCD
 * @param  x0: X coordinate of center circle point
 * @param  y0: Y coordinate of center circle point
 * @param  r: Circle radius
 * @param  color: Circle color
 * @retval None
 */
void TM_ILI9341_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);

/**
 * @brief  Draws filled circle on LCD
 * @param  x0: X coordinate of center circle point
 * @param  y0: Y coordinate of center circle point
 * @param  r: Circle radius
 * @param  color: Circle color
 * @retval None
 */
void TM_ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);

/**
 * @brief  Sets layer 2 to currently active layer
 * @param  None
 * @retval None
 */
void TM_ILI9341_SetLayer1(void);

/**
 * @brief  Sets layer 2 to currently active layer
 * @param  None
 * @retval None
 */
void TM_ILI9341_SetLayer2(void);

/**
 * @brief  Sets layer 1 opacity
 * @param  opacity: 0 to 255, 0 is no opacity, 255 is no transparency
 * @retval None
 */
void TM_ILI9341_SetLayer1Opacity(uint8_t opacity);

/**
 * @brief  Sets layer 2 opacity
 * @param  opacity: 0 to 255, 0 is no opacity, 255 is no transparency
 * @retval None
 */
void TM_ILI9341_SetLayer2Opacity(uint8_t opacity);

/**
 * @brief  This changes current active layer
 *         It sets transparency to 0 and 255 depends on which layer is selected
 
 * @note   If current layer is Layer 1, then now will be Layer 2 and vice versa
 * @retval None
 */
void TM_ILI9341_ChangeLayers(void);

/**
 * @brief  Copies content of layer 2 to layer 1
 * @note   It will do a memory copy from layer 2 to layer 1
 * @retval None
 */
void TM_ILI9341_Layer2To1(void);

/**
 * @brief  Copies content of layer 1 to layer 2
 * @note   It will do a memory copy from layer 1 to layer 2
 * @retval None
 */
void TM_ILI9341_Layer1To2(void);

/**
 * @brief   Enables display
 * @note    After initialization, LCD is enabled and you don't need to call this function
 * @param   None
 * @retval  None
 */
void TM_ILI9341_DisplayOn(void);

/**
 * @brief   Disables display
 * @param   None
 * @retval  None
 */
void TM_ILI9341_DisplayOff(void);

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
