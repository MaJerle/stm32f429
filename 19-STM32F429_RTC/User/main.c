/**
 *	Keil project example for internal RTC
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
#include "defines.h"
#include "stm32f4xx.h"

#include "tm_stm32f4_rtc.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_ili9341.h"

#include <stdio.h>

char buf[30];
TM_RTC_Time_t datatime;    
    
char buffer[30];

int main(void) {
	uint8_t old_sek;
	//Initialize system
	SystemInit();
	//Initialize delay
	TM_DELAY_Init();
	//Initiaize button
	TM_DISCO_ButtonInit();
	
	TM_DISCO_LedInit();
	//Init LCD
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	//Initialize USART, TX: PB10, RX: PB11
	TM_USART_Init(USART3, TM_USART_PinsPack_1, 115200);
	
	//Initialize RTC with internal 32768Hz clock
	//It's not very accurate
	TM_RTC_Init(TM_RTC_ClockSource_External);
	
	//Set wakeup interrupt every 1 second
	TM_RTC_Interrupts(TM_RTC_Int_1s);
	
	datatime.hours = 23;
	datatime.minutes = 59;
	datatime.seconds = 58;
	datatime.year = 14;
	datatime.month = 5;
	datatime.date = 31;
	datatime.day = 6;
	//Set new time
	TM_RTC_SetDateTime(&datatime, TM_RTC_Format_BIN);
	
	while (1) {
		TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
		if (datatime.seconds != old_sek) {
			old_sek = datatime.seconds;
			
			TM_DISCO_LedToggle(LED_RED);
			if (datatime.date == 1) {
				TM_DISCO_LedOn(LED_GREEN);
			} else {
				TM_DISCO_LedOff(LED_GREEN);
			}
			
			TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
			sprintf(buffer, "%02d.%02d.  %02d:%02d:%02d\n", datatime.date, datatime.month, datatime.hours, datatime.minutes, datatime.seconds);
			TM_ILI9341_Puts(10, 10, buffer, &TM_Font_11x18, 0x00, 0xFF);
		}
		
		if (TM_DISCO_ButtonPressed()) {
			datatime.hours = 23;
			datatime.minutes = 59;
			datatime.seconds = 55;
			datatime.year = 14;
			datatime.month = 5;
			datatime.date = 31;
			datatime.day = 6;
			//Set new time
			TM_RTC_SetDateTime(&datatime, TM_RTC_Format_BIN);
		}
	}
}

void TM_RTC_RequestHandler() {

}


