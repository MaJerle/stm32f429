/**
 *	Keil project for PWM example
 *
 *	Before you start, select your target, on the right of the "Load" button
 *	
 *	STM32F429-Discovery doesn't have leds connected to PWM pins,
 *	so you can test this with STM32F4-Discovery or Nucleo-F4(0/1)1RE boards with leds.
 *	
 *	Below is code for both boards in main.c
 *	You have to set correct target above first, but code will be always compiled,
 *	if you select STM32F4-Discovery or Nucleo F401RE boards
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

int main(void) {
	TM_PWM_TIM_t TIM4_Data, TIM2_Data;
	
	/* Initialize system */
	SystemInit();

/* STM32F4-Discovery LEDS start */
/* Leds on PD12, PD13, PD14, PD15 */
	
	/* Set PWM to 1kHz frequency on timer TIM4 */
	/* 1 kHz = 1ms = 1000us */
	TM_PWM_InitTimer(TIM4, &TIM4_Data, 1000);
	
	/* Initialize PWM on TIM4, Channel 1 and PinsPack 2 = PD12 */
	TM_PWM_InitChannel(&TIM4_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_2);
	/* Initialize PWM on TIM4, Channel 2 and PinsPack 2 = PD13 */
	TM_PWM_InitChannel(&TIM4_Data, TM_PWM_Channel_2, TM_PWM_PinsPack_2);
	/* Initialize PWM on TIM4, Channel 3 and PinsPack 2 = PD14 */
	TM_PWM_InitChannel(&TIM4_Data, TM_PWM_Channel_3, TM_PWM_PinsPack_2);
	/* Initialize PWM on TIM4, Channel 4 and PinsPack 2 = PD15 */
	TM_PWM_InitChannel(&TIM4_Data, TM_PWM_Channel_4, TM_PWM_PinsPack_2);
	
	/* Set channel 1 value, 50% duty cycle */
	TM_PWM_SetChannel(&TIM4_Data, TM_PWM_Channel_1, TIM4_Data.Period / 2);
	/* Set channel 2 value, 33% duty cycle */
	TM_PWM_SetChannel(&TIM4_Data, TM_PWM_Channel_2, TIM4_Data.Period / 3);
	/* Set channel 3 value, 25% duty cycle */
	TM_PWM_SetChannel(&TIM4_Data, TM_PWM_Channel_3, TIM4_Data.Period / 4);
	/* Set channel 4 value, 5% duty cycle*/
	TM_PWM_SetChannelPercent(&TIM4_Data, TM_PWM_Channel_4, 5);
/* STM32F4-Discovery LEDS stop */

/* Nucleo F4(0/1)1-RE LED start */
/* Led connected to PA5 */

	/* Set PWM to 1kHz frequency on timer TIM2 */
	/* 1 kHz = 1ms = 1000us */
	TM_PWM_InitTimer(TIM2, &TIM2_Data, 1000);
	
	/* Initialize PWM on TIM2, Channel 1 and PinsPack 2 = PA5 */
	TM_PWM_InitChannel(&TIM2_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_2);
	
	/* Set channel 1 value, 500us pulse high = 500 / 1000 = 0.5 = 50% duty cycle */
	TM_PWM_SetChannelMicros(&TIM2_Data, TM_PWM_Channel_1, 500);
	
/* Nucleo F4(0/1)1-RE LED stop */

	while (1) {
		
	}
}
