/**
 *	Keil project for MCO output
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
#include "tm_stm32f4_mco_output.h"

int main(void) {
	/* Initialize system */
	SystemInit();

	/* Initialize MCO1 output, pin PA8 */
	TM_MCOOUTPUT_InitMCO1();
	
	/* Initialize MCO2 output, pin PC9 */
	TM_MCOOUTPUT_InitMCO2();
	
	/* Set MCO1 output = HSI with prescaler 2 = 16MHz / 2 = 8MHz*/
	TM_MCOOUTPUT_SetOutput1(TM_MCOOUTPUT1_Source_HSI, TM_MCOOUTPUT_Prescaler_2);
	
	/* Set MCO2 output = SYSCLK / 4 */
	TM_MCOOUTPUT_SetOutput2(TM_MCOOUTPUT2_Source_SYSCLK, TM_MCOOUTPUT_Prescaler_4);

	while (1) {
		
	}
}
