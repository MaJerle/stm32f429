/**
 *	DAC example
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://majerle.eu
 *	@ide		Keil uVision 5
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_dac.h"

int main(void) {
	//Initialize system
	SystemInit();
	
	//Initialize DAC channel 1, pin PA4
	TM_DAC_Init(TM_DAC2);
	//Set 12bit analog value of 1500/4096 * 3.3V
	TM_DAC_SetValue(TM_DAC2, 1500);
	
	while (1) {
	
	}
}
