/**
 *	Keil project for HC-SR04 Ultrasonic distance sensor
 *	
 * 	Tested on STM32F4-, STM32F429- Discovery and Nucleo F401/411RE boards
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
#include "tm_stm32f4_hcsr04.h"
#include <stdio.h>

int main(void) {	
	/* HCSR04 Instance */
	TM_HCSR04_t HCSR04;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay functions */
	TM_DELAY_Init();
	
	/* Initialize LEDs on board */
	TM_DISCO_LedInit();
	
	/* Turn on LED red */
	TM_DISCO_LedOn(LED_RED);
	
	/* Initialize distance sensor1 on pins; ECHO: PD0, TRIGGER: PC1 */
	if (!TM_HCSR04_Init(&HCSR04, GPIOD, GPIO_PIN_0, GPIOC, GPIO_PIN_1)) {
		/* Sensor is not ready to use */
		/* Maybe wiring is incorrect */
		while (1) {
			TM_DISCO_LedToggle(LED_RED | LED_GREEN);
			Delayms(100);
		}
	}

	while (1) {
		/* Read distance from sensor 1 */
		/* Distance is returned in cm and also stored in structure */
		/* You can use both ways */
		TM_HCSR04_Read(&HCSR04);
		
		/* Something is going wrong, maybe incorrect pinout */
		if (HCSR04.Distance < 0) {
			TM_DISCO_LedOn(LED_RED);
			TM_DISCO_LedOff(LED_GREEN);
		} else if (HCSR04.Distance > 50) {
			/* Distance more than 50cm */
			TM_DISCO_LedOn(LED_GREEN);
			TM_DISCO_LedOff(LED_RED);
		} else {
			/* Distance between 0 and 50cm */
			TM_DISCO_LedOff(LED_GREEN | LED_RED);
		}
		
		/* Give some time to sensor */
		Delayms(100);
	}
}
