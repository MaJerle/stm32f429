/**
 *	Keil project for PWM SERVO example
 *
 *	Servo works with 50Hz (20ms) PWM frequency input.
 *	High pulse is between 1 and 2 ms, but some are working from 0.8 to 2.2ms
 *	My at home is going from 0.9 to 2.1 ms
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
#include "tm_stm32f4_pwm.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"

int main(void) {
	TM_PWM_TIM_t TIM2_Data;
	uint16_t position = 1000;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay functions */
	TM_DELAY_Init();

	/* Set PWM to 50Hz frequency on timer TIM2 */
	/* 50Hz = 20ms = 20000us */
	TM_PWM_InitTimer(TIM2, &TIM2_Data, 50);
	
	/* Initialize PWM on TIM2, Channel 1 and PinsPack 2 = PA5 */
	TM_PWM_InitChannel(&TIM2_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_2);
	
	/* Set channel 1 value, 1500us = servo at center position */
	TM_PWM_SetChannelMicros(&TIM2_Data, TM_PWM_Channel_1, 1500);
	
	/* Servo in position */
	Delayms(1000);

	while (1) {
		if (position == 1000) {
			/* Change direction */
			position = 2000;
		} else {
			/* Change direction */
			position = 1000;
		}

		/* Set servo position */
		TM_PWM_SetChannelMicros(&TIM2_Data, TM_PWM_Channel_1, position);

		/* Set some delay for servo to set to correct position */
		Delayms(1500);
	}
}
