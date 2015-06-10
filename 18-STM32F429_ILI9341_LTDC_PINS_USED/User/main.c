/**
 *	Keil project for ILI9341 LTDC showing how to get all used pins for your project
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

/* Create array of GPIOx pointers */
GPIO_TypeDef* GPIOs[] = {
	GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI, GPIOJ, GPIOK
};

int main(void) {
	uint16_t pins_used;
	uint8_t i, j;
	char buff[50];
	
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
	
	/* Go through all pins */
	for (i = 0; i < 11; i++) {
		/* Print for which GPIO we are checking */
		pins_used = TM_GPIO_GetUsedPins(GPIOs[i]);
		
		/* Format start string */
		sprintf(buff, "GPIO%c: ", ('A' + i));
		
		/* Go through all */
		if (pins_used) {
			for (j = 0; j < 0x10; j++) {
				if (pins_used & (1 << j)) {
					sprintf(buff, "%s%u,", buff, j);
				}
			}
			
			/* Delete last comma */
			buff[strlen(buff) - 1] = 0;
		} else {
			strcat(buff, "NO used pins");
		}
		
		/* Print to LCD */
		TM_ILI9341_Puts(10, i * 12 + 10, buff, &TM_Font_7x10, ILI9341_COLOR_GREEN2, 0x0000);
	}

	while (1) {		

	}
}
