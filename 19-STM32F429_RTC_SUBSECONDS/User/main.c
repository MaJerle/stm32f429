/**
 *	Keil project for internal RTC subseconds
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
#include "tm_stm32f4_rtc.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"

#include <stdio.h>

char buf[50];
TM_RTC_Time_t datatime;

int main(void) {	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initiaize button */
	TM_DISCO_ButtonInit();
	
	/* Initialize Leds */
	TM_DISCO_LedInit();
	
	/* Initialize USART, TX: PB10, RX: PB11 */
	TM_USART_Init(USART3, TM_USART_PinsPack_1, 115200);
	
	/* Initialize RTC with internal 32768Hz clock */
	/* It's not very accurate */
	if (!TM_RTC_Init(TM_RTC_ClockSource_Internal)) {
		/* RTC was first time initialized */
		/* Do your stuff here */
		/* eg. set default time */
	}
	
	/* Set wakeup interrupt every 125 ms */
	TM_RTC_Interrupts(TM_RTC_Int_125ms);
	
	while (1) {
		/* If button pressed */
		if (TM_DISCO_ButtonPressed()) {
			
			/* Subseconds are ignored when writing new time */
			
			datatime.hours = 0;
			datatime.minutes = 59;
			datatime.seconds = 55;
			datatime.year = 14;
			datatime.month = 6;
			datatime.date = 30;
			datatime.day = 6;
			
			/* Set new time */
			TM_RTC_SetDateTime(&datatime, TM_RTC_Format_BIN);
		}
	}
}

/* Custom request handler function */
/* Called on wakeup interrupt */
void TM_RTC_RequestHandler() {
	/* Get time */
	TM_RTC_GetDateTime(&datatime, TM_RTC_Format_BIN);
	
	/* Format time */
	sprintf(buf, "%02d.%02d.%04d %02d:%02d:%02d:%04d  Unix: %u\n",
				datatime.date,
				datatime.month,
				datatime.year + 2000,
				datatime.hours,
				datatime.minutes,
				datatime.seconds,
				datatime.subseconds,	/* Subseconds */
				datatime.unix
	);
	/* Send to USART */
	TM_USART_Puts(USART3, buf);
	/* Toggle LED */
	TM_DISCO_LedToggle(LED_RED | LED_GREEN);
}
