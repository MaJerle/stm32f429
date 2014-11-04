/**
 *	Keil project for internal RTC Alarms
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
/* Struct for RTC tim */
TM_RTC_Time_t Time;
/* Struct for alarm time */
TM_RTC_AlarmTime_t AlarmTime;

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
	
	/* Set wakeup interrupt every 1 second */
	TM_RTC_Interrupts(TM_RTC_Int_1s);
	
	while (1) {
		/* If button pressed */
		if (TM_DISCO_ButtonPressed()) {
			
			/* Set new time */
			Time.hours = 21;
			Time.minutes = 11;
			Time.seconds = 00;
			Time.year = 14;
			Time.month = 10;
			Time.date = 20;
			Time.day = 1;
			
			/* Set new RTC time */
			TM_RTC_SetDateTime(&Time, TM_RTC_Format_BIN);
			
			/* Set alarm A each day 1 (Monday) in a week */
			/* Alarm will be first triggered 5 seconds later as time is configured for RTC */
			AlarmTime.hours = Time.hours;
			AlarmTime.minutes = Time.minutes;
			AlarmTime.seconds = Time.seconds + 5;
			AlarmTime.alarmtype = TM_RTC_AlarmType_DayInWeek;
			AlarmTime.day = 1;
			
			/* Set RTC alarm A, time in binary format */
			TM_RTC_SetAlarm(TM_RTC_Alarm_A, &AlarmTime, TM_RTC_Format_BIN);
			
			/* Set alarm B each 20th day in a month */
			/* Alarm will be first triggered 10 seconds later as time is configured for RTC */
			AlarmTime.hours = Time.hours;
			AlarmTime.minutes = Time.minutes;
			AlarmTime.seconds = Time.seconds + 10;
			AlarmTime.day = 20;
			AlarmTime.alarmtype = TM_RTC_AlarmType_DayInMonth;
			
			/* Set RTC alarm B, time in binary format */
			TM_RTC_SetAlarm(TM_RTC_Alarm_B, &AlarmTime, TM_RTC_Format_BIN);
		}
	}
}

/* Custom request handler function */
/* Called on wakeup interrupt */
void TM_RTC_RequestHandler() {
	/* Get time */
	TM_RTC_GetDateTime(&Time, TM_RTC_Format_BIN);
	
	/* Format time */
	sprintf(buf, "%02d.%02d.%04d %02d:%02d:%02d  Unix: %u\n",
				Time.date,
				Time.month,
				Time.year + 2000,
				Time.hours,
				Time.minutes,
				Time.seconds,
				Time.unix
	);
	
	/* Send to USART */
	TM_USART_Puts(USART3, buf);
	
	/* Toggle LED */
	TM_DISCO_LedToggle(LED_RED | LED_GREEN);
}

/* Custom request handler function */
/* Called on alarm A interrupt */
void TM_RTC_AlarmAHandler(void) {
	/* Show user to USART */
	TM_USART_Puts(USART3, "Alarm A triggered\n");
	
	/* Disable Alarm so it will not trigger next week at the same time */
	//TM_RTC_DisableAlarm(TM_RTC_Alarm_A);
}

/* Custom request handler function */
/* Called on alarm B interrupt */
void TM_RTC_AlarmBHandler(void) {
	/* Show user to USART */
	TM_USART_Puts(USART3, "Alarm B triggered\n");
	
	/* Disable Alarm so it will not trigger next month at the same date and time */
	//TM_RTC_DisableAlarm(TM_RTC_Alarm_A);
}
