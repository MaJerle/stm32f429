#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_adc.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

caddr_t _sbrk(int incr) {
	extern char _ebss; // Defined by the linker
	static char *heap_end;
	char *prev_heap_end;
	if (heap_end == 0) {
		heap_end = &_ebss;
	}
	prev_heap_end = heap_end;
	char * stack = (char*) __get_MSP();
	if (heap_end + incr > stack) {
		errno = ENOMEM;
		return (caddr_t) -1;
	}
	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

int main(void) {
	SystemInit();
	TM_DELAY_Init();
	TM_USART_Init(115200);


	GPIO_InitTypeDef  GPIO_InitStructure;

	// Clock Enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// Config des DAC-Pins als Analog-Ausgang
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	DAC_InitTypeDef DAC_Initialize;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	DAC_Initialize.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_Initialize.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Initialize.DAC_Trigger = DAC_Trigger_None;
	DAC_Initialize.DAC_WaveGeneration = DAC_WaveGeneration_None;

	DAC_Init(DAC_Channel_1, &DAC_Initialize);
	DAC_Cmd(DAC_Channel_1, ENABLE);




	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_CommonInitTypeDef ADC_CommonInitialize;
	ADC_CommonInitialize.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitialize.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitialize.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitialize.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;
	ADC_CommonInit(&ADC_CommonInitialize);

	ADC_InitTypeDef ADC_Initialize;
	ADC_StructInit(&ADC_Initialize);
	ADC_Init(ADC1, &ADC_Initialize);
	ADC_Cmd(ADC1, ENABLE);

	int i;

	//DAC_SetChannel1Data(DAC_Align_12b_R, 3500);
	//Delayms(1000);
	char str[15];
    while(1) {
    	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_3Cycles);
    	ADC_SoftwareStartConv(ADC1);
    	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    	i = ADC_GetConversionValue(ADC1);
    	DAC_SetChannel1Data(DAC_Align_12b_R, i);

    	sprintf(str, "%d\n\r", i);
    	TM_USART_Puts(str);

    	Delayms(100);
    }
}
