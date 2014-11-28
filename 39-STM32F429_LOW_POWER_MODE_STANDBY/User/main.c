/**
 *	Keil project for low power mode - STANDBY mode
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
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_low_power.h"
#include "tm_stm32f4_rtc.h"

int main(void) {
	uint8_t i;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize LEDS */
	TM_DISCO_LedInit();
	
	/* Checks if reset was because of wakeup from standby */
	if (TM_LOWPOWER_StandbyReset()) {
		for (i = 0; i < 10; i++) {
			/* Toggle LED red to indicate this */
			TM_DISCO_LedToggle(LED_RED);
			/* Delay */
			Delayms(100);
		}
	}
	
	/* Initialize RTC with internal clock */
	TM_RTC_Init(TM_RTC_ClockSource_Internal);
	
	/* Set RTC to generate wakeup interrupt every 10 seconds */
	TM_RTC_Interrupts(TM_RTC_Int_10s);
	
	/* Enable wakeup pin, PA0 */
	TM_LOWPOWER_EnableWakeUpPin();
	
	/* Set time to 0 */
	TM_DELAY_SetTime(0);
	
	while (1) {
		/* Toggle LEDs every 200ms */
		if (TM_DELAY_Time() >= 200) {
			/* Reset time */
			TM_DELAY_SetTime(0);
			
			/* Toggle leds */
			TM_DISCO_LedToggle(LED_GREEN);
			
			/* Increase counter */
			i++;
			
			/* After 20 toggles, put STM32F4 into STANDBY mode */
			if (i == 20) {
				/* Reset counter */
				i = 0;
				
				
				/* Put STM32F4 into standby mode */
				/* You can wake up MCU with rising edge on PA0 pin */
				/* Or with some special interrupts, like RTC, etc */
				TM_LOWPOWER_Standby();
				
				/* Toggle RED LED to indicate wakeup from STANDBY mode */
				/* This should never happen, because STM32F4 will reset after wakeup from STANDBY */
				TM_DISCO_LedToggle(LED_RED);
			}
		}
	}
}
