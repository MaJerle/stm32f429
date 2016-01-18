/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Library template 
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
#ifndef TM_LIBRARY_H
#define TM_LIBRARY_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_LCD
 * @brief    Library description here
 * @{
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
 - defines.h
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_ltdc.h"
#include "defines.h"
#include "tm_stm32f4_sdram.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_fonts.h"
#include "tm_stm32f4_dma2d_graphic.h"

/**
 * @defgroup TM_LCD_Macros
 * @brief    Library defines
 * @{
 */

#if defined(USE_LCD_STM324x9_EVAL) || defined(TM_DISCO_STM324x9_EVAL)

/* Check define */
#ifndef USE_LCD_STM324x9_EVAL
#define USE_LCD_STM324x9_EVAL
#endif

/* Set pixel settings */
#define LCD_PIXEL_WIDTH        640
#define LCD_PIXEL_HEIGHT       480

#else

/* STM32F429-Discovery */
#define LCD_PIXEL_WIDTH        240
#define LCD_PIXEL_HEIGHT       320

#endif

/* Frame settings */
#define LCD_FRAME_BUFFER       (SDRAM_START_ADR)
#define LCD_BUFFER_OFFSET      ((uint32_t)(LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT * 2))

/**
 * @}
 */
 
/**
 * @defgroup TM_LCD_Typedefs
 * @brief    Library Typedefs
 * @{
 */

typedef enum _TM_LCD_Result_t {
	TM_LCD_Result_Ok = 0x00,
	TM_LCD_Result_Error,
	TM_LCD_Result_SDRAM_Error
} TM_LCD_Result_t;

/**
 * @}
 */

/**
 * @defgroup TM_LCD_Functions
 * @brief    Library Functions
 * @{
 */

TM_LCD_Result_t TM_LCD_Init(void);
TM_LCD_Result_t TM_LCD_Fill(uint32_t color);
TM_LCD_Result_t TM_LCD_DisplayOn(void);
TM_LCD_Result_t TM_LCD_DisplayOff(void);
uint16_t TM_LCD_GetWidth(void);
uint16_t TM_LCD_GetHeight(void);
uint32_t TM_LCD_GetFrameBuffer(void);

TM_LCD_Result_t TM_LCD_SetXY(uint16_t X, uint16_t Y);
TM_LCD_Result_t TM_LCD_SetFont(TM_FontDef_t* Font);
TM_LCD_Result_t TM_LCD_SetColors(uint32_t Foreground, uint32_t Background);
TM_LCD_Result_t TM_LCD_Putc(char c);
TM_LCD_Result_t TM_LCD_Puts(char* str);

/**
 * @defgroup TM_LCD_GRAPHIC_Functions
 * @brief    Library Functions
 * @{
 */

TM_LCD_Result_t TM_LCD_DrawPixel(uint16_t X, uint16_t Y, uint32_t color);
uint32_t TM_LCD_GetPixel(uint16_t X, uint16_t Y);
TM_LCD_Result_t TM_LCD_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);
TM_LCD_Result_t TM_LCD_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t Width, uint16_t Height, uint32_t color);
TM_LCD_Result_t TM_LCD_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t Width, uint16_t Height, uint32_t color);
TM_LCD_Result_t TM_LCD_DrawRoundedRectangle(uint16_t x0, uint16_t y0, uint16_t Width, uint16_t Height, uint16_t r, uint32_t color);
TM_LCD_Result_t TM_LCD_DrawFilledRoundedRectangle(uint16_t x0, uint16_t y0, uint16_t Width, uint16_t Height, uint16_t r, uint32_t color);
TM_LCD_Result_t TM_LCD_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);
TM_LCD_Result_t TM_LCD_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);

/**
 * @}
 */

/**
 * @defgroup TM_LCD_LAYER_Functions
 * @brief    Library Functions
 * @{
 */
 
TM_LCD_Result_t TM_LCD_SetLayer1(void);
TM_LCD_Result_t TM_LCD_SetLayer2(void);
TM_LCD_Result_t TM_LCD_SetLayer1Opacity(uint8_t opacity);
TM_LCD_Result_t TM_LCD_SetLayer2Opacity(uint8_t opacity);
TM_LCD_Result_t TM_LCD_ChangeLayers(void);
TM_LCD_Result_t TM_LCD_Layer2To1(void);
TM_LCD_Result_t TM_LCD_Layer1To2(void);

/**
 * @}
 */
 
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
