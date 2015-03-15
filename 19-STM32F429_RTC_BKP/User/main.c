/**
 *	Keil project for internal RTC backup registers
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
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"

#include <stdio.h>

int main(void) {	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initiaize button */
	TM_DISCO_ButtonInit();
	
	/* Initialize Leds */
	TM_DISCO_LedInit();
	
	/* Initialize RTC with internal 32768Hz clock */
	/* It's not very accurate */
	if (!TM_RTC_Init(TM_RTC_ClockSource_Internal)) {
		/* RTC was first time initialized */
		/* Do your stuff here */
		/* eg. set default time */
		
		/* Write data to backup register 4 */
		TM_RTC_WriteBackupRegister(4, 0x1244);
		
		/* Turn on RED led = Write operation */
		TM_DISCO_LedOn(LED_RED);
	} else {
		/* Try to read data back and check if it is OK */
		if (TM_RTC_ReadBackupRegister(4) == 0x1244) {
			/* Read OK after reset, turn on GREEN led */
			TM_DISCO_LedOn(LED_GREEN);
		}
	}
	
	/* If not leds ON, try to remove power from device first so RTC will need to initialize again */
	/* If still no leds after that, then you have problems with your RTC clock */
	
	while (1) {
		/* If button pressed */
		if (TM_DISCO_ButtonPressed()) {
			/* After button press, system will reset */
			NVIC_SystemReset();
		}
	}
}

