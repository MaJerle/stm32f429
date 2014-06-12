/**
 *	Keil project
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
#include "defines.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "tm_stm32f4_delay.h"


int main(void) {
	GPIO_InitTypeDef GPIO_InitDef;
	//Initialize system
	SystemInit();

	//TM_DELAY_Init();
	
	if (SysTick_Config(SystemCoreClock / 1000000)) {
		//Capture error
		while (1);
	}
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	
	GPIO_Init(GPIOA, &GPIO_InitDef);
	
	while (1) {
		GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
		Delayms(500);
	}
}
