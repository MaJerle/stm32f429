/**
 *	Keil project for ADC peripheral
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_adc.h"
#include <stdio.h>

int main(void) {
	char str[15];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize Delay library */
	TM_DELAY_Init();
	
	/* Init ILI9341 on board */
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	/* Initialize ADC1 on channel 0, this is pin PA0 */
	TM_ADC_Init(ADC1, ADC_Channel_0);
	
	/* Initialize ADC1 on channel 3, this is pin PA3 */
	TM_ADC_Init(ADC1, ADC_Channel_3);
	
	while (1) {
		/* 							Read ADC1 Channel0					Read ADC1 Channel3 */
		sprintf(str, "%4d: %4d\n\r", TM_ADC_Read(ADC1, ADC_Channel_0), TM_ADC_Read(ADC1, ADC_Channel_3));
		
		TM_ILI9341_Puts(10, 10, str, &TM_Font_11x18, 0x0000, 0xFFFF);
		
		Delayms(100);
	}
}
