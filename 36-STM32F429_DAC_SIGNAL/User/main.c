/**
 *	Keil project for DAC with DMA signal feature
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
#include "tm_stm32f4_dac_signal.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"

int main(void) {	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize LEDs on board */
	TM_DISCO_LedInit();
	
	/* Initialize DAC1, use TIM4 for signal generation */
	TM_DAC_SIGNAL_Init(TM_DAC1, TIM4);
	/* Initialize DAC2, use TIM5 for signal generation */
	TM_DAC_SIGNAL_Init(TM_DAC2, TIM5);
	
	/* Output predefined triangle signal with frequency of 5kHz */
	TM_DAC_SIGNAL_SetSignal(TM_DAC1, TM_DAC_SIGNAL_Signal_Triangle, 5000);
	/* Output predefined square signal with frequency of 10kHz */
	TM_DAC_SIGNAL_SetSignal(TM_DAC2, TM_DAC_SIGNAL_Signal_Square, 10000);
	
	while (1) {
		
	}
}
