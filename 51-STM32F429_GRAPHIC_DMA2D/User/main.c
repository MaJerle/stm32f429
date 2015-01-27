/**
 *	Keil project example for ST's Chrom-ART Accelerator (DMA2D)
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
#include "tm_stm32f4_dma2d_graphic.h"
#include "tm_stm32f4_ili9341_ltdc.h"
#include "tm_stm32f4_fonts.h"
#include "tm_stm32f4_delay.h"
#include <stdio.h>

int main(void) {
	/* Set coordinates for poly line */
	TM_DMA2DRAPHIC_Poly_t Coordinates[] = {
		{10, 10},
		{15, 65},
		{200, 200},
		{150, 30},
		{10, 10}
	};
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay functions */
	TM_DELAY_Init();
	
	/* Initialize ILI9341 with LTDC */
	/* By default layer 1 is used */
	TM_ILI9341_Init();
	/* Initialize DMA2D GRAPHIC library */
	TM_DMA2DGRAPHIC_Init();
	
	/* Set ILI9341 Orientation */
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	/* Set DMA2D GRAPHIC library Orientation */
	TM_DMA2DGRAPHIC_SetOrientation((uint8_t)TM_ILI9341_Orientation_Portrait_2);

	/* Layer 1 */
	/* Draw red rectangle at X = 10, Y = 10 position, width = 100 and height = 50px */
	TM_DMA2DGRAPHIC_DrawRectangle(10, 10, 100, 50, GRAPHIC_COLOR_RED);
	
	/* Draw filled blue rectangle at X = 100, Y = 100 position, width = 100 and height = 50px */
	TM_DMA2DGRAPHIC_DrawFilledRectangle(100, 100, 100, 50, GRAPHIC_COLOR_BLUE);
	
	/* Draw rounded rectangle */
	TM_DMA2DGRAPHIC_DrawRoundedRectangle(110, 110, 80, 30, 10, GRAPHIC_COLOR_GREEN);
	
	/* Go to layer 2, set address offset in memory for LCD */
	TM_DMA2DGRAPHIC_SetLayer(2);
	
	/* Fill layer 2 */
	TM_DMA2DGRAPHIC_Fill(GRAPHIC_COLOR_YELLOW);
	
	/* Draw poly line */
	TM_DMA2DGRAPHIC_DrawPolyLine(Coordinates, 5, GRAPHIC_COLOR_BLACK);
	
	/* Draw filled circle */
	TM_DMA2DGRAPHIC_DrawFilledCircle(100, 170, 40, GRAPHIC_COLOR_CYAN);
	
	while (1) {
		/* Change display layer on LCD using LTDC transfer */
		TM_ILI9341_ChangeLayers();
		
		/* Delay */
		Delayms(1000);
	}
}
