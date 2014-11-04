/**
 *	Keil project for L3GD20 3x Gryo on STM32F429 Discovery board
 *
 *	Designed for STM32F429-Discovery board
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_l3gd20.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_ili9341_ltdc.h"

#include <stdio.h>

int main(void) {
	/* L3GD20 Struct */
	TM_L3GD20_t L3GD20_Data;
	char buffer[20];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Init ILI9341 with LTDC on STM32F429 Discovery board */
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	/* Fill layer 1 */
	TM_ILI9341_Fill(ILI9341_COLOR_RED);
	
	/* Put some text */
	TM_ILI9341_Puts(10, 10, "L3GD20 3-Axis\nGyroscope", &TM_Font_11x18, 0x0000, ILI9341_COLOR_RED);
	TM_ILI9341_Puts(90, 310, "stm32f4-discovery.com", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_RED);
	
	/* Init L3GD20 sensor */
	if (TM_L3GD20_Init(TM_L3GD20_Scale_2000) != TM_L3GD20_Result_Ok) {
		/* Sensor error */
		TM_ILI9341_Puts(10, 100, "Sensor ERROR!", &TM_Font_11x18, 0x0000, ILI9341_COLOR_RED);
		
		while (1);
	}

	while (1) {
		/* Read data */
		TM_L3GD20_Read(&L3GD20_Data);
		/* Display data on LCD */
		sprintf(buffer, "X rotation: %4d", L3GD20_Data.X);
		TM_ILI9341_Puts(10, 100, buffer, &TM_Font_11x18, 0x0000, ILI9341_COLOR_RED);
		sprintf(buffer, "Y rotation: %4d", L3GD20_Data.Y);
		TM_ILI9341_Puts(10, 122, buffer, &TM_Font_11x18, 0x0000, ILI9341_COLOR_RED);
		sprintf(buffer, "Z rotation: %4d", L3GD20_Data.Z);
		TM_ILI9341_Puts(10, 144, buffer, &TM_Font_11x18, 0x0000, ILI9341_COLOR_RED);
	}
}


