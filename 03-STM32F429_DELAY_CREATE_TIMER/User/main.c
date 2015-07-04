/**
 *	Keil project for delay
 *
 *  Before you start, select your target, on the right of the "Load" button
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

/* Create 2 callback functions for custom timers */
void CustomTIMER1_Task(void* UserParameters);
void CustomTIMER2_Task(void* UserParameters);

/* Pointers to custom timers */
TM_DELAY_Timer_t* CustomTimer1;
TM_DELAY_Timer_t* CustomTimer2;

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize onboard leds */
	TM_DISCO_LedInit();

	/* Init 2 custom timers */
	/* Timer1 has reload value each 500ms, enabled auto reload feature and timer is enabled */
	CustomTimer1 = TM_DELAY_TimerCreate(100, 1, 1, CustomTIMER1_Task, NULL);
	
	/* Timer1 has reload value each 70ms, enabled auto reload feature and timer is enabled */
	CustomTimer2 = TM_DELAY_TimerCreate(200, 1, 1, CustomTIMER2_Task, NULL);
	
	while (1) {
		/* After 2 seconds */
		if (TM_DELAY_Time() > 2000) {
			/* Stop timer 2 */
			TM_DELAY_TimerStop(CustomTimer2);
		}
	}
}

/* Called when Custom TIMER1 reaches zero */
void CustomTIMER1_Task(void* UserParameters) {
	/* Toggle GREEN led */
	TM_DISCO_LedToggle(LED_GREEN);
}

/* Called when Custom TIMER2 reaches zero */
void CustomTIMER2_Task(void* UserParameters) {
	/* Toggle RED led */
	TM_DISCO_LedToggle(LED_RED);
}
