/**
 *	Keil project for RC Servo example
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
#include "tm_stm32f4_servo.h"

int main(void) {
	/* Servo structs */
	TM_SERVO_t Servo1, Servo2;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize servo 1, TIM2, Channel 1, Pinspack 2 = PA5 */
	TM_SERVO_Init(&Servo1, TIM2, TM_PWM_Channel_1, TM_PWM_PinsPack_2);
	
	/* Initialize servo 2, TIM2, Channel 2, Pinspack 2 = PB3 */
	TM_SERVO_Init(&Servo2, TIM2, TM_PWM_Channel_2, TM_PWM_PinsPack_2);

	while (1) {
		/* 0 degrees rotation on servo 1 */
		TM_SERVO_SetDegrees(&Servo1, 0);
		/* 180 degrees on servo 2 */
		TM_SERVO_SetDegrees(&Servo2, 180);
		/* 2s delay */
		Delayms(2000);
		
		/* 90 degrees rotation */
		TM_SERVO_SetDegrees(&Servo1, 90);
		/* 90 degrees on servo 2 */
		TM_SERVO_SetDegrees(&Servo2, 90);
		/* 2s delay */
		Delayms(2000);
		
		/* 180 degrees rotation */
		TM_SERVO_SetDegrees(&Servo1, 180);
		/* 0 degrees on servo 2 */
		TM_SERVO_SetDegrees(&Servo2, 0);
		/* 2s delay */
		Delayms(2000);
	}
}
