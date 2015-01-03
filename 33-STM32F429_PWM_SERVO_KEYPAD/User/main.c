/**
 *	Keil project for PWM SERVO example controlled with keypad
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
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_keypad.h"

/* Set this value for your servo */
/* Pulse length in microseconds */
#define SERVO_MAX		2000
#define SERVO_MIN		1000
#define SERVO_MIDDLE	(SERVO_MIN + (SERVO_MAX - SERVO_MIN) / 2)

int main(void) {
	TM_PWM_TIM_t TIM2_Data;
	TM_KEYPAD_Button_t Keypad_Button;
	uint16_t duration = SERVO_MIDDLE;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay functions */
	TM_DELAY_Init();
	
	/* Initialize matrix keypad, 4x4 */
	TM_KEYPAD_Init(TM_KEYPAD_Type_Large);

	/* Set PWM to 50Hz frequency on timer TIM2 */
	/* 50Hz = 20ms = 20000us */
	TM_PWM_InitTimer(TIM2, &TIM2_Data, 50);
	
	/* Initialize PWM on TIM2, Channel 1 and PinsPack 2 = PA5 */
	TM_PWM_InitChannel(&TIM2_Data, TM_PWM_Channel_1, TM_PWM_PinsPack_2);
	
	/* Set channel 1 value, 1500us = servo at center position */
	TM_PWM_SetChannelMicros(&TIM2_Data, TM_PWM_Channel_1, SERVO_MIDDLE);

	while (1) {
		/* Read keypad */
		Keypad_Button = TM_KEYPAD_Read();
		
		/* If any button pressed */
		if (Keypad_Button != TM_KEYPAD_Button_NOPRESSED) {
			switch (Keypad_Button) {
				case TM_KEYPAD_Button_0:	/* Go to the middle position */
				case TM_KEYPAD_Button_2:
				case TM_KEYPAD_Button_5:
				case TM_KEYPAD_Button_8:
					/* Go to the middle */
					duration = SERVO_MIDDLE;
					break;
				case TM_KEYPAD_Button_1:	/* 1us decrease pulse length */
					duration--;
					break;
				case TM_KEYPAD_Button_3:	/* 1us increase pulse length */
					duration++;
					break;
				case TM_KEYPAD_Button_4:	/* 10us decrease pulse length */
					duration -= 10;
					break;
				case TM_KEYPAD_Button_6:	/* 10us increase pulse length */
					duration += 10;
					break;
				case TM_KEYPAD_Button_7:	/* 100us decrease pulse length */
					duration -= 100;
					break;
				case TM_KEYPAD_Button_9:	/* 100us increase pulse length */
					duration += 100;
					break;
				case TM_KEYPAD_Button_STAR:	/* Turn totally left */
					duration = SERVO_MIN;
					break;
				case TM_KEYPAD_Button_HASH:	/* Turn totally right */
					duration = SERVO_MAX;
					break;
				default: /* Do nothing */
					break;
			}
			
			/* Filter */
			if (duration > SERVO_MAX) {
				duration = SERVO_MAX;
			} else if (duration < SERVO_MIN) {
				duration = SERVO_MIN;
			}
			
			/* Set PWM length for servo */
			TM_PWM_SetChannelMicros(&TIM2_Data, TM_PWM_Channel_1, duration);
		}
	}
}
