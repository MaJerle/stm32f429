/**
 *	Keil project for low power mode - STOP mode
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
	
	/* Initialize RTC with internal clock */
	TM_RTC_Init(TM_RTC_ClockSource_Internal);
	
	/* Set RTC to generate wakeup interrupt every 10 seconds */
	TM_RTC_Interrupts(TM_RTC_Int_10s);
	
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
			
			/* After 20 toggles, put STM32F4 into STOP mode */
			if (i == 20) {
				/* Reset counter */
				i = 0;
				
				/* Stop STM32F4 */
				/* If you stop device, then you can wake him up with interrupt/event on EXTI line */
				
				/* Put it into STOP mode, allowing EXTI interrupts to wake him up */
				/* RTC will wake him up after 10 seconds */
				TM_LOWPOWER_StopUntilInterrupt();
				
				/* Toggle RED LED to indicate wakeup from stop mode */
				TM_DISCO_LedToggle(LED_RED);
			}
		}
	}
}
