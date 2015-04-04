/**
 *	Keil project for AM2301 sensor
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
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_am2301.h"
#include "tm_stm32f4_ili9341.h"

#include <stdio.h>

int main(void) {
	TM_AM2301_Data_t data;
	char str[50];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize LCD on F429-Discovery board */
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	TM_ILI9341_Fill(ILI9341_COLOR_ORANGE);
	TM_ILI9341_Puts(10, 10, "AM2301 (DHT21)\nsensor", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);
	TM_ILI9341_Puts(90, 310, "stm32f4-discovery.com", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);
	
	/* Initialize sensor */
	TM_AM2301_Init();
	
	/* Reset time */
	TM_DELAY_SetTime(0);
	while (1) {
		/* Every 1 second */
		if (TM_DELAY_Time() > 1000) {
			/* Reset time */
			TM_DELAY_SetTime(0);
			
			/* Data valid */
			if (TM_AM2301_Read(&data) == TM_AM2301_OK) {
				/* Show on LCD */
				sprintf(str, "Humidity: %2.1f %%\nTemperature: %2.1f C", (float)data.Hum / 10, (float)data.Temp / 10);
				/* Put to LCD */
				TM_ILI9341_Puts(10, 100, str, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);
			}
		}
		
		/* Do other stuff here constantly */
	}
}
