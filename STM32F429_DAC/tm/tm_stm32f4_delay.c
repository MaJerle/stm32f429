#include "tm_stm32f4_delay.h"

static __IO uint32_t TimingDelay;

void Delay(__IO uint32_t nTime) {
	TimingDelay = nTime;

	while (TimingDelay != 0);
}

void Delayms(__IO uint32_t nTime) {
	TimingDelay = 1000 * nTime;

	while (TimingDelay != 0);
}

void SysTick_Handler(void) {
	if (TimingDelay != 0x00) {
		TimingDelay--;
	}
}

void TM_DELAY_Init(void) {
	RCC_HSEConfig(RCC_HSE_ON);
	while (!RCC_WaitForHSEStartUp());

	//Every 1us
	if (SysTick_Config(SystemCoreClock / 1000000)) {
		/* Capture error */
		while (1);
	}
}
