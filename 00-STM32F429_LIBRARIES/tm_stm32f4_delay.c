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

__IO uint32_t TM_TimingDelay = 0;
__IO uint32_t TM_Time = 0;
__IO uint32_t TM_Time2 = 0;
uint8_t TM_DELAY_Initialized = 0;
volatile uint32_t mult;

#if defined(TM_DELAY_TIM)
void TM_DELAY_INT_InitTIM(void);
#endif

#if defined(TM_DELAY_TIM)
void TM_DELAY_TIM_IRQ_HANDLER(void) {
	TIM_ClearITPendingBit(TM_DELAY_TIM, TIM_IT_Update);
#elif defined(KEIL_IDE)
void TimingDelay_Decrement(void) {
#else
void SysTick_Handler(void) {
#endif
	TM_Time++;
	if (TM_Time2 != 0x00) {
		TM_Time2--;
	}
	
	/* Call user function */
	TM_DELAY_1msHandler();
}

void TM_DELAY_Init(void) {
	/* If already initialized */
	if (TM_DELAY_Initialized) {
		return;
	}
	
	/* Enable External HSE clock */
	RCC_HSEConfig(RCC_HSE_ON);
	
	/* Wait for stable clock */
	while (!RCC_WaitForHSEStartUp());
	
#if defined(TM_DELAY_TIM)
	TM_DELAY_INT_InitTIM();
#else
	/* Set Systick interrupt every 1ms */
	if (SysTick_Config(SystemCoreClock / 1000)) {
		/* Capture error */
		while (1);
	}
	
	#ifdef __GNUC__
		/* Set multiplier for delay under 1us with pooling mode = not so accurate */
		mult = SystemCoreClock / 7000000;
	#else
		/* Set multiplier for delay under 1us with pooling mode = not so accurate */
		mult = SystemCoreClock / 3000000;
	#endif
#endif
	
	/* Set initialized flag */
	TM_DELAY_Initialized = 1;
}

void TM_DELAY_EnableDelayTimer(void) {
	/* Check if library is even initialized */
	if (!TM_DELAY_Initialized) {
		return;
	}
	
#if defined(TM_DELAY_TIM)
	/* Enable TIMER for delay, useful when you wakeup from sleep mode */
	TIM_Cmd(TM_DELAY_TIM, ENABLE);
#else
	/* Enable systick interrupts, useful when you wakeup from sleep mode */  
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
#endif
}

void TM_DELAY_DisableDelayTimer(void) {
#if defined(TM_DELAY_TIM)
	/* Disable TIMER for delay, useful when you go to sleep mode */
	TIM_Cmd(TM_DELAY_TIM, DISABLE);
#else
	/* Disable systick, useful when you go to sleep mode */
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
#endif
}

/* Internal functions */
#if defined(TM_DELAY_TIM)
void TM_DELAY_INT_InitTIM(void) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	TM_TIMER_PROPERTIES_t TIM_Data;
	
	/* Get timer properties */
	TM_TIMER_PROPERTIES_GetTimerProperties(TM_DELAY_TIM, &TIM_Data);
	
	/* Generate timer properties, 1us ticks */
	TM_TIMER_PROPERTIES_GenerateDataForWorkingFrequency(&TIM_Data, 1000000);
	
	/* Enable clock for TIMx */
	TM_TIMER_PROPERTIES_EnableClock(TM_DELAY_TIM);
	
	/* Set timer settings */
	TIM_TimeBaseStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStruct.TIM_Period = 999; /* 1 millisecond */
	TIM_TimeBaseStruct.TIM_Prescaler = SystemCoreClock / (1000000 * (SystemCoreClock / TIM_Data.TimerFrequency)) - 1; /* With prescaler for 1 microsecond tick */
	TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
	
	/* Initialize timer */
	TIM_TimeBaseInit(TM_DELAY_TIM, &TIM_TimeBaseStruct);
	
	/* Enable interrupt */
	TIM_ITConfig(TM_DELAY_TIM, TIM_IT_Update, ENABLE);
	
	/* Set NVIC parameters */
	NVIC_InitStruct.NVIC_IRQChannel = TM_DELAY_TIM_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);
	
	/* Start timer */
	TIM_Cmd(TM_DELAY_TIM, ENABLE);
}
#endif

