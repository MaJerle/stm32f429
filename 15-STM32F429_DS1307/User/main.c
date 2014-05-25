/**
 *	Keil project example for DS1307 RTC library
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_ds1307.h"
#include "tm_stm32f4_ili9341.h"
#include <stdio.h>

int main(void) {
	char str[20];
	TM_DS1307_Time_t time;
	//Initialize system
	SystemInit();
	
	//Initialize DS1307
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	//Initialize DS1307
	TM_DS1307_Init();
	
	//Set date and time
	time.hours = 23;
	time.minutes = 59;
	time.seconds = 56;
	time.date = 28;
	time.day = 3;
	time.month = 2;
	time.year = 13;
	TM_DS1307_SetDateTime(&time);
	
	while (1) {
		//Get date and time
		TM_DS1307_GetDateTime(&time);
		//Display on LCD
		sprintf(str, "%d.  %02d.%02d.%04d\n%02d:%02d:%02d", time.day, time.date, time.month, time.year + 2000, time.hours, time.minutes, time.seconds);
		TM_ILI9341_Puts(10, 15, str, &TM_Font_11x18, 0x0000, 0xFFFF);
	}
}
