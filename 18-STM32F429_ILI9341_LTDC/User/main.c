/**
 *	Keil project for ILI9341 LCD example with LTDC support
 *	
 *	Works only on STM32F429-Discovery!
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
#include "tm_stm32f4_ili9341_ltdc.h"
#include "tm_stm32f4_fonts.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include <stdio.h>

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize ILI9341 with LTDC */
	/* By default layer 1 is used */
	TM_ILI9341_Init();
	
	/* Rotate LCD for 90 degrees */
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_2);
	
	/* Go to layer 1 */
	TM_ILI9341_SetLayer1();
	
	/* Fill screen on layer 1 */
	TM_ILI9341_Fill(ILI9341_COLOR_BLUE);
	
	/* Show text */
	TM_ILI9341_Puts(65, 30, "Layer 1", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	TM_ILI9341_Puts(20, 130, "STM32F429 Discovery ONLY!", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	TM_ILI9341_Puts(60, 150, "ILI9341 LCD Module", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	TM_ILI9341_Puts(70, 170, "with LTDC support", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	TM_ILI9341_Puts(170, 230, "stm32f4-discovery.com", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);
	
	/* Go to layer 2 */
	TM_ILI9341_SetLayer2();
	
	/* Fill screen on layer 2 */
	TM_ILI9341_Fill(ILI9341_COLOR_GREEN);
	
	/* Show text */
	TM_ILI9341_Puts(65, 30, "Layer 2", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE);
	TM_ILI9341_Puts(20, 130, "STM32F429 Discovery ONLY!", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_RED);
	TM_ILI9341_Puts(60, 150, "ILI9341 LCD Module", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	TM_ILI9341_Puts(70, 170, "with LTDC support", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	TM_ILI9341_Puts(170, 230, "stm32f4-discovery.com", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);
	
	/* Draw circle on layer 2 */
	TM_ILI9341_DrawCircle(150, 150, 140, ILI9341_COLOR_BLACK);

	while (1) {		
		/* This will set opacity of one layer to 0, other to max (255) each time */
		/* This is like toggle function */
		TM_ILI9341_ChangeLayers();
		/* Every 1 second */
		Delayms(1000);
		/*
		//Bottom code works the same as one before inside while loop
		
		//Turn on Layer1 and turn off Layer2
		TM_ILI9341_SetLayer1Opacity(255);
		TM_ILI9341_SetLayer2Opacity(0);
		Delayms(500);
		
		//Turn on Layer2 and turn off Layer1
		TM_ILI9341_SetLayer1Opacity(0);
		TM_ILI9341_SetLayer2Opacity(255);
		Delayms(500);
		*/
	}
}
