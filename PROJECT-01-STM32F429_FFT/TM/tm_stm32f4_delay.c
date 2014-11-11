/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
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
	if (TM_Time2 != 0x00) {
		TM_Time2--;
	}
	if (TM_TimingDelay != 0x00) {
		TM_TimingDelay--;
	}
}
#else
void SysTick_Handler(void) {
	TM_Time++;
	if (TM_Time2 != 0x00) {
		TM_Time2--;
	}
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
	/* Set Systick interrupt every 1us */
	if (SysTick_Config(SystemCoreClock / 1000000)) {
		/* Capture error */
		while (1);
	}
	TM_DELAY_Initialized = 1;
}

uint32_t TM_DELAY_Time(void) {
	return TM_Time;
}

uint32_t TM_DELAY_Time2(void) {
	return TM_Time2;
}

void TM_DELAY_SetTime(uint32_t time) {
	TM_Time = time;
}

void TM_DELAY_SetTime2(uint32_t time) {
	TM_Time2 = time;
}

void TM_DELAY_EnableSystick(void) {
	if (!TM_DELAY_Initialized) {
		return;
	}
	
	/* Enable systick interrupts, useful when you wakeup from sleep mode */  
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

void TM_DELAY_DisableSystick(void) {
	/* Disable systick, useful when you go to sleep mode */
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

