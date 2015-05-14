/**
 *	Keil project for DS1307 RTC library
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
#include "tm_stm32f4_ds1307.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_disco.h"
#include <stdio.h>

int main(void) {
	char str[100];
	TM_DS1307_Time_t time;
	uint8_t last;
	
	/* Initialize system */
	SystemInit();
		
	/* Initialize LEDS */
	TM_DISCO_LedInit();
	
	/* Initialize ILI9341 LCD on board */
	TM_ILI9341_Init();
	TM_ILI9341_Fill(ILI9341_COLOR_ORANGE);
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	TM_ILI9341_Puts(90, 310, "stm32f4-discovery.com", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);
	
	/* Initialize DS1307 */
	if (TM_DS1307_Init() != TM_DS1307_Result_Ok) {
		/* Red LED on */
		TM_DISCO_LedOn(LED_RED);
		
		/* Show on LCD */
		TM_ILI9341_Puts(10, 10, "DS1307 Error", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_ORANGE);
		
		while (1);
	}
	
	/* Set date and time */
	/* Day 7, 26th May 2014, 02:05:00 */
	time.hours = 21;
	time.minutes = 18;
	time.seconds = 0;
	time.date = 26;
	time.day = 1;
	time.month = 5;
	time.year = 14;
	TM_DS1307_SetDateTime(&time);
	
	/* Disable output first */
	TM_DS1307_DisableOutputPin();
	
	/* Set output pin to 4096 Hz */
	TM_DS1307_EnableOutputPin(TM_DS1307_OutputFrequency_4096Hz);
	
	while (1) {
		/* Get date and time */
		TM_DS1307_GetDateTime(&time);
		
		/* Display on LCD */
		sprintf(str, "Day: %d\nDate: %02d\nMonth: %02d\nYear: %04d\nHours: %02d\nMinutes: %02d\nSeconds: %02d", time.day, time.date, time.month, time.year + 2000, time.hours, time.minutes, time.seconds);
		TM_ILI9341_Puts(10, 15, str, &TM_Font_11x18, ILI9341_COLOR_ORANGE, 0x0000);
		
		/* Toggle GREEN led if needed */
		if (last != time.seconds) {
			last = time.seconds;
			
			/* Toggle GREEN LED */
			TM_DISCO_LedToggle(LED_GREEN);
		}
	}
}
