/**
 *	Keil project for ADC peripheral how to measure Vbat pin
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
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_usart.h"
#include <stdio.h>

int main(void) {
	char str[150];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize Delay library */
	TM_DELAY_Init();
	
	/* Initialize USART1, 115200 baud, TX: PB6 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Initialize ADC1 */
	TM_ADC_InitADC(ADC1);
	
	/* Enable vbat channel */
	TM_ADC_EnableVbat();
	
	while (1) {
		/* Read & format data */
		sprintf(str, "----------------------------\nVbat voltage: %d mV\n", TM_ADC_ReadVbat(ADC1));
		
		/* Put to USART */
		TM_USART_Puts(USART1, str);
		
		/* Little delay */
		Delayms(1000);
	}
}
