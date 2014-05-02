/**
 *	Button library for ILI9341 LCD controller
 * 	and STMPE811 Resistive touch screen controller
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@link		
 *	@version 	v1.0
 */
#ifndef TM_ILI9341_BUTTON_H
#define TM_ILI9341_BUTTON_H
/**
 * Library dependencies
 *
 * TM_ILI931	LCD
 * TM_I2C		I2C
 * TM_STMPE811	Touch
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

#ifndef TM_ILI9341_BUTTON_MAX_BUTTONS
#define TM_ILI9341_BUTTON_MAX_BUTTONS	20	// Maximum number of buttons available
#endif

//Button flags
#define TM_BUTTON_FLAG_ENABLED			0x0001	
#define TM_BUTTON_FLAG_NOBORDER			0x0002
#define TM_BUTTON_FLAG_IMAGE			0x0004
#define TM_BUTTON_FLAG_NOLABEL			0x0008
#define TM_BUTTON_FLAG_USED				0x8000


typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	uint16_t background;
	uint16_t borderColor;
	uint16_t flags;
	char *label;
	TM_FontDef_t *font;
	uint16_t color;
	uint16_t *image;
} TM_ILI9341_Button_t;

extern TM_ILI9341_Button_t TM_ILI9341_Buttons[TM_ILI9341_BUTTON_MAX_BUTTONS];

extern int8_t TM_ILI9341_Button_Add(TM_ILI9341_Button_t *button);

extern void TM_ILI9341_Button_DrawAll(void);

extern ErrorStatus TM_ILI9341_Button_Draw(uint8_t id);

extern void TM_ILI9341_Button_Enable(uint8_t id);

extern void TM_ILI9341_Button_Disable(uint8_t id);

extern void TM_ILI9341_Button_DeleteAll(void);

extern void TM_ILI9341_Button_Delete(uint8_t id);

extern int8_t TM_ILI9341_Button_Touch(TM_STMPE811_TouchData *TouchData);

#endif

