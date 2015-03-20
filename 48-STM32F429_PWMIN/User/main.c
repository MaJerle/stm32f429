/**
 *	Keil project example for PWM input
 *
 *	In example, "fake" PWM signal of 3251Hz is generated
 *	and connected with wire to another for measuring.
 *
 * 	Results I got were:
 *	- Results on F407 168MHz device
 *	- Real input frequency was 3251Hz
 *	- 1000Hz expected frequency:
 *		- TIM2 (32-bit, 84MHz clock): frequency: 3251.4Hz, duty: 49.99
 *		- TIM3 (16-bit, 84MHz/prescaler clock): frequency: 3251.53; duty: 49.99
 *	- 1Hz expected frequency:
 *		- TIM2 (32-bit, 84MHz clock): frequency: 3251.4Hz, duty: 49.99
 *		- TIM3 (16-bit, 84MHz/prescaler clock): frequency: 3276.13; duty: 50.00
 *	- TIM3 has "only" 16bit and clock is quite faster for 1Hz, so it has prescaler,
 *		which gives you less accurate value.
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
#include "tm_stm32f4_pwmin.h"
#include "tm_stm32f4_pwm.h"
#include "tm_stm32f4_swo.h"

/* PWM IN structures */
TM_PWMIN_t PWMIN_TIM2;
TM_PWMIN_t PWMIN_TIM3;
/* PWM structures */
TM_PWM_TIM_t PWM_Data;

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	
/* PWM output for testing */
	/* Init TIM4 for PWM output, 10kHz PWM */
	TM_PWM_InitTimer(TIM4, &PWM_Data, 3251);
	
	/* TIM4, pin PD12 */
	TM_PWM_InitChannel(&PWM_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_2);
	
	/* 50 percent duty cycle on PD12 output */
	TM_PWM_SetChannelPercent(&PWM_Data, TM_PWM_Channel_1, 50);
/* PWM output for testing */
	
/* PWM input */	
	/* Initialize timer for PWM Input capture =  */
	/* We expect signal greater or equal to 1000Hz */
	/* Uncomment "1" and comment "1000" to change expected frequency */
	TM_PWMIN_InitTimer(TIM2, &PWMIN_TIM2, TM_PWMIN_Channel_1, TM_PWMIN_PinsPack_2, 1000 /* 1 */, TIM2_IRQn);
	
	/* We expect signal greater or equal to 1000Hz */
	/* Uncomment "1" and comment "1000" to change expected frequency */
	TM_PWMIN_InitTimer(TIM3, &PWMIN_TIM3, TM_PWMIN_Channel_2, TM_PWMIN_PinsPack_2, 1000 /* 1 */, TIM3_IRQn);
/* PWM input */

	/* Initialize SWO debug */
	TM_SWO_Init();
	
	while (1) {
		/* Every 100ms look for new signal */
		if (TM_DELAY_Time() >= 100) {
			TM_DELAY_SetTime(0);
			
			/* Get new data for both input signals */
			TM_PWMIN_Get(&PWMIN_TIM2);
			TM_PWMIN_Get(&PWMIN_TIM3);
			
			/* Valid PWM input signal */
			if (PWMIN_TIM2.Frequency > 0) {
				/* Print via SWo debug */
				TM_SWO_Printf("TIM2: F: %7.2f, D: %5.2f\n", PWMIN_TIM2.Frequency, PWMIN_TIM2.DutyCycle);
			}
			
			/* Valid PWM input signal */
			if (PWMIN_TIM3.Frequency > 0) {
				/* Print via SWo debug */
				TM_SWO_Printf("TIM3: F: %7.2f, D: %5.2f\n", PWMIN_TIM3.Frequency, PWMIN_TIM3.DutyCycle);
			}
			
			/* Print separator to debug window */
			TM_SWO_Printf("----\n");
		}
	}
}

/* TIM2 IRQ handler */
void TIM2_IRQHandler(void) {
	/* Interrupt request, don't forget! */
	TM_PWMIN_InterruptHandler(&PWMIN_TIM2);
}

/* TIM3 IRQ handler */
void TIM3_IRQHandler(void) {
	/* Interrupt request, don't forget! */
	TM_PWMIN_InterruptHandler(&PWMIN_TIM3);
}

