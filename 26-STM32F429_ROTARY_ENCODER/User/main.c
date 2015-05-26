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

/* Rotary encoder data */
TM_RE_t RE1_Data;
TM_RE_t RE2_Data;

int main(void) {
	char buf[100];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize LCD */
	TM_ILI9341_Init();
	
	/* Rotate LCD */
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	/* Initialize Rotary encoder 1, pin A = PD0, pin B = PD1 */
	TM_RE_Init(&RE1_Data, GPIOD, GPIO_PIN_0, GPIOD, GPIO_PIN_1);
	
	/* Initialize Rotary encoder 2, pin A = PD3, pin B = PD5 */
	TM_RE_Init(&RE2_Data, GPIOD, GPIO_PIN_3, GPIOD, GPIO_PIN_5);
	
	/* Set rotation mode for rotary 1 */
	TM_RE_SetMode(&RE1_Data, TM_RE_Mode_One);

	while (1) {
		/* Get new rotation */
		TM_RE_Get(&RE1_Data);
		
		/* Format & display on LCD */
		sprintf(buf, "Absolute:\n%8d\n\n\nDifference from\nlast check:\n%8d", RE1_Data.Absolute, RE1_Data.Diff);
		
		/* Show on LCD */
		TM_ILI9341_Puts(10, 10, buf, &TM_Font_11x18, 0x0000, 0xFFFF);
		
		/* Get new rotation */
		TM_RE_Get(&RE2_Data);
		
		/* Format & display on LCD */
		sprintf(buf, "Absolute:\n%8d\n\n\nDifference from\nlast check:\n%8d", RE2_Data.Absolute, RE2_Data.Diff);
		
		/* Show on LCD */
		TM_ILI9341_Puts(10, 160, buf, &TM_Font_11x18, 0x0000, 0xFFFF);
	}
}

/* TM EXTI Handler for all EXTI lines */
void TM_EXTI_Handler(uint16_t GPIO_Pin) {
	/* Check RE pin 1 */
	if (GPIO_Pin == RE1_Data.GPIO_PIN_A) {
		/* Process data */
		TM_RE_Process(&RE1_Data);
	}
	if (GPIO_Pin == RE2_Data.GPIO_PIN_A) {
		/* Process data */
		TM_RE_Process(&RE2_Data);
	}
}
