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
#include "tm_stm32f4_timer_properties.h"

TM_TIMER_PROPERTIES_Result_t TM_TIMER_PROPERTIES_GetTimerProperties(TIM_TypeDef* TIMx, TM_TIMER_PROPERTIES_t* Timer_Data) {
	RCC_ClocksTypeDef RCC_ClocksStruct;

	/* Get clocks */
	RCC_GetClocksFreq(&RCC_ClocksStruct);
	
	/* All timers have 16-bit prescaler */
	Timer_Data->MaxPrescaler = 0xFFFF;
	
	if (0 /* 32bit timers with PCLK2 max frequency */
#ifdef TIM2
		|| TIMx == TIM2
#endif
#ifdef TIM5
		|| TIMx == TIM5
#endif
	) {
		Timer_Data->TimerFrequency = RCC_ClocksStruct.PCLK2_Frequency;	/* Clock */
		Timer_Data->MaxPeriod = 0xFFFFFFFF;								/* Max period */
		
		/* Timer valid */
		return TM_TIMER_PROPERTIES_Result_Ok;
	} else if (0	/* 16bit timers with HCLK clock frequency */
#ifdef TIM1
		|| TIMx == TIM1
#endif
#ifdef TIM8
		|| TIMx == TIM8
#endif
#ifdef TIM9
		|| TIMx == TIM9
#endif
#ifdef TIM10
		|| TIMx == TIM10
#endif
#ifdef TIM11
		|| TIMx == TIM11
#endif
	) {
		Timer_Data->TimerFrequency = RCC_ClocksStruct.HCLK_Frequency;	/* Clock */
		Timer_Data->MaxPeriod = 0xFFFF;									/* Max period */
		
		/* Timer valid */
		return TM_TIMER_PROPERTIES_Result_Ok;
	} else if (0	/* 16bit timers with PCLK2 clock frequency */
#ifdef TIM3
		|| TIMx == TIM3
#endif
#ifdef TIM4
		|| TIMx == TIM4
#endif
#ifdef TIM6
		|| TIMx == TIM6
#endif
#ifdef TIM7
		|| TIMx == TIM7
#endif
#ifdef TIM12
		|| TIMx == TIM12
#endif
#ifdef TIM13
		|| TIMx == TIM13
#endif
#ifdef TIM14
		|| TIMx == TIM14
#endif
	) {
		Timer_Data->TimerFrequency = RCC_ClocksStruct.PCLK2_Frequency;	/* Clock */
		Timer_Data->MaxPeriod = 0xFFFF;									/* Max period */
		
		/* Timer valid */
		return TM_TIMER_PROPERTIES_Result_Ok;
	}
	
	/* Timer is not valid */
	return TM_TIMER_PROPERTIES_Result_TimerNotValid;
}

TM_TIMER_PROPERTIES_Result_t TM_TIMER_PROPERTIES_GenerateDataForWorkingFrequency(TM_TIMER_PROPERTIES_t* Timer_Data, double frequency) {
	if (frequency > Timer_Data->TimerFrequency) {
		/* Reset values */
		Timer_Data->Prescaler = 0;
		Timer_Data->Period = 0;
		Timer_Data->Frequency = 0;
		
		/* Frequency too high */
		return TM_TIMER_PROPERTIES_Result_FrequencyTooHigh;
	} else if (frequency == 0) {
		/* Reset values */
		Timer_Data->Prescaler = 0;
		Timer_Data->Period = 0;
		Timer_Data->Frequency = 0;
		
		/* Not valid frequency */
		return TM_TIMER_PROPERTIES_Result_FrequencyTooLow;
	}
	
	/* Fix for 16/32bit timers */
	if (Timer_Data->MaxPeriod <= 0xFFFF) {
		Timer_Data->MaxPeriod++;
	}
	
	/* Get minimum prescaler and maximum resolution for timer */
	Timer_Data->Prescaler = 0;
	do {
		/* Get clock */
		Timer_Data->Period = (Timer_Data->TimerFrequency / (Timer_Data->Prescaler + 1));
		/* Get period */
		Timer_Data->Period = (Timer_Data->Period / frequency);
		/* Increase prescaler value */
		Timer_Data->Prescaler++;
	} while (Timer_Data->Period > (Timer_Data->MaxPeriod) && Timer_Data->Prescaler <= (Timer_Data->MaxPrescaler + 1));
	/* Check for too low frequency */ 
	if (Timer_Data->Prescaler > (Timer_Data->MaxPrescaler + 1)) {
		/* Reset values */
		Timer_Data->Prescaler = 0;
		Timer_Data->Period = 0;
		Timer_Data->Frequency = 0;
		
		/* Prescaler too high, frequency is too low for use */
		return TM_TIMER_PROPERTIES_Result_FrequencyTooLow;
	}
	
	/* Set frequency */
	Timer_Data->Frequency = frequency;
	
	/* Return OK */
	return TM_TIMER_PROPERTIES_Result_Ok;
}

TM_TIMER_PROPERTIES_Result_t TM_TIMER_PROPERTIES_EnableClock(TIM_TypeDef* TIMx) {
#ifdef TIM1
	if (TIMx == TIM1) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	}
#endif
#ifdef TIM2
	if (TIMx == TIM2) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
#endif
#ifdef TIM3
	if (TIMx == TIM3) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
#endif
#ifdef TIM4
	if (TIMx == TIM4) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
#endif
#ifdef TIM5
	if (TIMx == TIM5) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	}
#endif
#ifdef TIM6
	if (TIMx == TIM6) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	}
#endif
#ifdef TIM7
	if (TIMx == TIM7) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	}
#endif
#ifdef TIM8
	if (TIMx == TIM8) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
	}
#endif
#ifdef TIM9
	if (TIMx == TIM9) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
	}
#endif
#ifdef TIM10
	if (TIMx == TIM10) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
	}
#endif
#ifdef TIM11
	if (TIMx == TIM11) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
	}
#endif
#ifdef TIM12
	if (TIMx == TIM12) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;
	}
#endif
#ifdef TIM13
	if (TIMx == TIM13) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM13EN;
	}
#endif
#ifdef TIM14
	if (TIMx == TIM14) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
	}
#endif
	
	/* Return OK */
	return TM_TIMER_PROPERTIES_Result_Ok;
}

TM_TIMER_PROPERTIES_Result_t TM_TIMER_PROPERTIES_DisableClock(TIM_TypeDef* TIMx) {
#ifdef TIM1
	if (TIMx == TIM1) {
		RCC->APB2ENR &= ~RCC_APB2ENR_TIM1EN;
	}
#endif
#ifdef TIM2
	if (TIMx == TIM2) {
		RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;
	}
#endif
#ifdef TIM3
	if (TIMx == TIM3) {
		RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN;
	}
#endif
#ifdef TIM4
	if (TIMx == TIM4) {
		RCC->APB1ENR &= ~RCC_APB1ENR_TIM4EN;
	}
#endif
#ifdef TIM5
	if (TIMx == TIM5) {
		RCC->APB1ENR &= ~RCC_APB1ENR_TIM5EN;
	}
#endif
#ifdef TIM6
	if (TIMx == TIM6) {
		RCC->APB1ENR &= ~RCC_APB1ENR_TIM6EN;
	}
#endif
#ifdef TIM7
	if (TIMx == TIM7) {
		RCC->APB1ENR &= ~RCC_APB1ENR_TIM7EN;
	}
#endif
#ifdef TIM8
	if (TIMx == TIM8) {
		RCC->APB2ENR &= ~RCC_APB2ENR_TIM8EN;
	}
#endif
#ifdef TIM9
	if (TIMx == TIM9) {
		RCC->APB2ENR &= ~RCC_APB2ENR_TIM9EN;
	}
#endif
#ifdef TIM10
	if (TIMx == TIM10) {
		RCC->APB2ENR &= ~RCC_APB2ENR_TIM10EN;
	}
#endif
#ifdef TIM11
	if (TIMx == TIM11) {
		RCC->APB2ENR &= ~RCC_APB2ENR_TIM11EN;
	}
#endif
#ifdef TIM12
	if (TIMx == TIM12) {
		RCC->APB1ENR &= ~RCC_APB1ENR_TIM12EN;
	}
#endif
#ifdef TIM13
	if (TIMx == TIM13) {
		RCC->APB1ENR &= ~RCC_APB1ENR_TIM13EN;
	}
#endif
#ifdef TIM14
	if (TIMx == TIM14) {
		RCC->APB1ENR &= ~RCC_APB1ENR_TIM14EN;
	}
#endif
	
	/* Return OK */
	return TM_TIMER_PROPERTIES_Result_Ok;
}
