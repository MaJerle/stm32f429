/**
 *	Simple snake game for STM32F429-Discovery board
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
#include "tm_stm32f4_snake.h"

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Start snake game */
	TM_SNAKE_Start();
	
	while (1) {
		
	}
}
