/**
 *	Keil project for rotary encoder
 *
 *	Before you start, select your target, on the right of the "Load" button
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
#include "tm_stm32f4_rotary_encoder.h"

#include <stdio.h>

int main(void) {
	char buf[15];
	/* Rotary encoder data */
	TM_RE_t data;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize LCD */
	TM_ILI9341_Init();
	/* Rotate LCD */
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	/* Initialize Rotary encoder */
	TM_RE_Init(&data);

	while (1) {
		/* Get new rotation */
		TM_RE_Get(&data);
		
		/* Format % display on LCD */
		sprintf(buf, "Absolute:\n%8d\n\n\nDifference from\nlast check:\n%8d", data.Absolute, data.Diff);
		TM_ILI9341_Puts(10, 10, buf, &TM_Font_11x18, 0x0000, 0xFFFF);
	}
}

