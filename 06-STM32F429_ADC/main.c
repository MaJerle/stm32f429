#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"

int main(void) {
	SystemInit();

	TM_DELAY_Init();

	TM_DISCO_LedInit();

	TM_DELAY_SetTime(0);
	while (1) {
		if (TM_DELAY_Time() > 500000) {
			TM_DELAY_SetTime(0);
			GPIO_ToggleBits(GPIOG, LED_RED | LED_GREEN);
		}
	}
}
