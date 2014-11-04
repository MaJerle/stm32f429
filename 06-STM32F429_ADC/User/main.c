/**
 *	Keil project for ADC peripheral
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
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_adc.h"
#include <stdio.h>

int main(void) {
	char str[15];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize Delay library */
	TM_DELAY_Init();
	
	/* Initialize USART1, 115200baud, TX: PB6 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	/* Initialize ADC1 on channel 0, this is pin PA0 */
	TM_ADC_Init(ADC1, ADC_Channel_0);
	
	/* Initialize ADC1 on channel 3, this is pin PA3 */
	TM_ADC_Init(ADC1, ADC_Channel_3);
	
	while (1) {
		/* 							Read ADC1 Channel0					Read ADC1 Channel3 */
		sprintf(str, "%4d: %4d\n\r", TM_ADC_Read(ADC1, ADC_Channel_0), TM_ADC_Read(ADC1, ADC_Channel_3));
		
		/* Put to USART */
		TM_USART_Puts(USART1, str);
		
		/* Little delay */
		Delayms(100);
	}
}
