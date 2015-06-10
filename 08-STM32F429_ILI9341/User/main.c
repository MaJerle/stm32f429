/**
 *	Keil project for ILI9341 LCD connected as SPI
 *
 *  Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"
#include "tm_stm32f4_delay.h"
#include <stdio.h>

int main(void) {	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
		
	/* Initialize ILI9341 */
	TM_ILI9341_Init();
	
	/* Rotate LCD for 90 degrees */
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_2);
	
	/* Fill lcd with color */
	TM_ILI9341_Fill(ILI9341_COLOR_MAGENTA);
	/* Draw white circle */
	TM_ILI9341_DrawCircle(60, 60, 40, ILI9341_COLOR_GREEN);
	/* Draw red filled circle */
	TM_ILI9341_DrawFilledCircle(60, 60, 35, ILI9341_COLOR_RED);
	/* Draw blue rectangle */
	TM_ILI9341_DrawRectangle(120, 20, 220, 100, ILI9341_COLOR_BLUE);
	/* Draw black filled circle */
	TM_ILI9341_DrawFilledRectangle(130, 30, 210, 90, ILI9341_COLOR_BLACK);
	/* Draw line with custom color 0x0005 */
	TM_ILI9341_DrawLine(10, 120, 310, 120, 0x0005);
	
	/* Put string with black foreground color and blue background with 11x18px font */
	TM_ILI9341_Puts(65, 130, "STM32F4 Discovery", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	/* Put string with black foreground color and blue background with 11x18px font */
	TM_ILI9341_Puts(60, 150, "ILI9341 LCD Module", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	/* Put string with black foreground color and red background with 11x18px font */
	TM_ILI9341_Puts(45, 229, "stm32f4-discovery.com", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);

	/* While loop */
	while (1) {
		
	}
}
