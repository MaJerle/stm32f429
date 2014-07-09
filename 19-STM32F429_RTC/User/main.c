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

#include <stdio.h>

char buf[30];
TM_RTC_Time_t datatime;


int main(void) {	
	//Initialize system
	SystemInit();
	//Initialize delay
	TM_DELAY_Init();
	//Initiaize button
	TM_DISCO_ButtonInit();
	
	//Initialize USART, TX: PB10, RX: PB11
	TM_USART_Init(USART3, TM_USART_PinsPack_1, 115200);
	
	//Initialize RTC with internal 32768Hz clock
	//It's not very accurate
	TM_RTC_Init(TM_RTC_ClockSource_Internal);
	
	//Set wakeup interrupt every 1 second
	TM_RTC_Interrupts(TM_RTC_Int_1s);
	
	while (1) {
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
			
			datatime.year = 0;
			datatime.month = 0;
			datatime.date = 0;
			datatime.day = 0;
		}
	}
}

void TM_RTC_RequestHandler() {

	TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
	sprintf(buf, "%02d.%02d.%04d %02d:%02d:%02d\n", datatime.date, datatime.month, datatime.year + 2000, datatime.hours, datatime.minutes, datatime.seconds);
	TM_USART_Puts(USART3, buf);
}


