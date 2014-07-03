#include "tm_stm32f4_delay.h"

static __IO uint32_t TM_TimingDelay = 0;
static __IO uint32_t TM_Time = 0;
uint8_t TM_DELAY_Initialized = 0;

void Delay(__IO uint32_t nTime) {
	TM_TimingDelay = nTime;

	while (TM_TimingDelay != 0);
}

void Delayms(__IO uint32_t nTime) {
	TM_TimingDelay = 1000 * nTime;

	while (TM_TimingDelay != 0);
}

#ifdef KEIL_IDE
void TimingDelay_Decrement(void) {
	TM_Time++;
	if (TM_TimingDelay != 0x00) {
		TM_TimingDelay--;
	}
}
#else
void SysTick_Handler(void) {
	TM_Time++;
	if (TM_TimingDelay != 0x00) {
		TM_TimingDelay--;
	}
}
#endif

void TM_DELAY_Init(void) {
	if (TM_DELAY_Initialized) {
		return;
	}
	RCC_HSEConfig(RCC_HSE_ON);
	while (!RCC_WaitForHSEStartUp());
	//Set Systick interrupt every 1us
	if (SysTick_Config(SystemCoreClock / 1000000)) {
		//Capture error
		while (1);
	}
	TM_DELAY_Initialized = 1;
}

uint32_t TM_DELAY_Time(void) {
	return TM_Time;
}

void TM_DELAY_SetTime(uint32_t time) {
	TM_Time = time;
}

