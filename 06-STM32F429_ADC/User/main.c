/**
 *	ADC Example
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_adc.h"
#include <stdio.h>

int main(void) {
	char str[15];
	//Initialize system
	SystemInit();
	//Initialize Delay library
	TM_DELAY_Init();
	
	//Initialize USART1-> TX: PA9, RX: PA10
	TM_USART_Init(USART1, TM_USART_PinsPack_1, 9600);
	
	TM_USART_Puts(USART1, "ADC example");
	
	//Initialize ADC1 on channel 0, this is pin PA0
	TM_ADC_Init(ADC1, ADC_Channel_0);
	//Initialize ADC1 on channel 3, this is pin PA3
	TM_ADC_Init(ADC1, ADC_Channel_3);
	
	while (1) {
		//							Read ADC1 Channel0					Read ADC1 Channel3
		sprintf(str, "%d: %d\n\r", TM_ADC_Read(ADC1, ADC_Channel_0), TM_ADC_Read(ADC1, ADC_Channel_3));
		TM_USART_Puts(USART1, str);
		
		Delayms(100);
	}
}
