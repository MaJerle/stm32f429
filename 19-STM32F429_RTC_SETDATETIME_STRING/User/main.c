/**
 *	Keil project for internal RTC
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
	char buff[100];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initiaize button */
	TM_DISCO_ButtonInit();
	
	/* Initialize Leds */
	TM_DISCO_LedInit();
	
	/* Initialize USART, TX: PB6, RX: PB7 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Initialize RTC with internal 32768Hz clock */
	/* It's not very accurate */
	if (!TM_RTC_Init(TM_RTC_ClockSource_Internal)) {
		/* RTC was first time initialized */
		/* Do your stuff here */
		/* eg. set default time */
	}

	while (1) {
		/* If string received from USART1 */
		if (TM_USART_Gets(USART1, buff, sizeof(buff))) {
			/* Try to set date and time */
			if (TM_RTC_SetDateTimeString(buff) == TM_RTC_Result_Ok) {
				/* Date and time written OK */
				
				/* Send OK back */
				TM_USART_Puts(USART1, "Written OK\n");
			} else {
				/* Send error sring */
				TM_USART_Puts(USART1, "Error\n");
			}
		}
	}
}
