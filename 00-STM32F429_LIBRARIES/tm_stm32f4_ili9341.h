/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/04/library-08-ili9341-lcd-on-stm32f429-discovery-board/
 * @version v1.3
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   ILI9341 library for STM32F4xx with SPI communication, without LTDC hardware
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
#ifndef TM_ILI9341_H
#define TM_ILI9341_H 130

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_ILI9341
 * @brief    ILI9341 library for STM32F4xx with SPI communication, without LTDC hardware - http://stm32f4-discovery.com/2014/04/library-08-ili9341-lcd-on-stm32f429-discovery-board/
 * @{
 *
 * This driver works for all STM32F4xx series with built in SPI peripheral.
 *	
 * \par Default pinout
 *
@verbatim
ILI9341      STM32F4xx    DESCRIPTION
		
SDO (MISO    PF8          Output from LCD for SPI.	Not used, can be left
LED          3.3V         Backlight
SCK          PF7          SPI clock
SDI (MOSI)   PF9          SPI master output
WRX or D/C   PD13         Data/Command register
RESET        PD12         Reset LCD
CS           PC2          Chip select for SPI
GND          GND          Ground
VCC          3.3V         Positive power supply
@endverbatim
 *		
 * All pins can be changed in your defines.h file
 *		
@verbatim
//Default SPI used is SPI5. Check my SPI library for other pinouts
#define ILI9341_SPI           SPI5
#define ILI9341_SPI_PINS      TM_SPI_PinsPack_1
		
//Default CS pin. Edit this in your defines.h file
#define ILI9341_CS_PORT       GPIOC
#define ILI9341_CS_PIN        GPIO_PIN_2
		
//Default D/C (or WRX) pin. Edit this in your defines.h file
#define ILI9341_WRX_PORT      GPIOD
#define ILI9341_WRX_PIN       GPIO_PIN_13
@endverbatim
 *
 * Reset pin can be disabled, if you need GPIOs for other purpose.
 * To disable RESET pin, add line below to defines.h file
 * If you disable pin, then set LCD's RESET pin to VCC.
 *	
@verbatim
//Disable RESET pin
#define ILI9341_USE_RST_PIN			0
@endverbatim
 *		
 * But if you want to use RESET pin, you can change its settings in defines.h file
 *	
@verbatim
//Default RESET pin. Edit this in your defines.h file
#define ILI9341_RST_PORT			GPIOD
#define ILI9341_RST_PIN				GPIO_PIN_12
@endverbatim
 *
 * \par Changelog
 *
@verbatim
 Version 1.3
  - June 06, 2015
  - Added support for SPI DMA for faster refreshing
 
 Version 1.2
  - March 14, 2015
  - Added support for new GPIO system
  - Added functions TM_ILI9341_DisplayOff() and TM_ILI9341_DisplayOn()
 
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
 - STM32F4xx SPI
 - defines.h
 - TM SPI
 - TM DMA
 - TM SPI DMA
 - TM FONTS
 - TM GPIO
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "tm_stm32f4_fonts.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_dma.h"
#include "tm_stm32f4_spi_dma.h"

/**
 * @defgroup TM_ILI9341_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  This SPI pins are used on STM32F429-Discovery board
 */
#ifndef ILI9341_SPI
#define ILI9341_SPI           SPI5
#define ILI9341_SPI_PINS      TM_SPI_PinsPack_1
#endif

/**
 * @brief  CS PIN for SPI, used as on STM32F429-Discovery board
 */
#ifndef ILI9341_CS_PIN
#define ILI9341_CS_PORT       GPIOC
#define ILI9341_CS_PIN        GPIO_PIN_2
#endif

/**
 * @brief  WRX PIN for data/command, used as on STM32F429-Discovery board
 */
#ifndef ILI9341_WRX_PIN
#define ILI9341_WRX_PORT      GPIOD
#define ILI9341_WRX_PIN       GPIO_PIN_13
#endif

/**
 * @brief  RESET for LCD
 */
#ifndef ILI9341_RST_PIN
#define ILI9341_RST_PORT      GPIOD
#define ILI9341_RST_PIN       GPIO_PIN_12
#endif

/* LCD settings */
#define ILI9341_WIDTH        240
#define ILI9341_HEIGHT       320
#define ILI9341_PIXEL        76800

/* Colors */
#define ILI9341_COLOR_WHITE			0xFFFF
#define ILI9341_COLOR_BLACK			0x0000
#define ILI9341_COLOR_RED       0xF800
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

/* Transparent background, only for strings and chars */
#define ILI9341_TRANSPARENT			0x80000000

/**
 * @}
 */
 
/**
 * @defgroup TM_ILI9341_Typedefs
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
 * @defgroup TM_ILI9341_Functions
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

