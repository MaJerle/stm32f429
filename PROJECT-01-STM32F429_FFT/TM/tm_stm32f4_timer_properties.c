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
	
	if ( /* 32bit timers with PCLK2 max frequency */
		TIMx == TIM2 ||
		TIMx == TIM5
	) {
		Timer_Data->TimerFrequency = RCC_ClocksStruct.PCLK2_Frequency;	/* Clock */
		Timer_Data->MaxPeriod = 0xFFFFFFFF;							/* Max period */
		
		return TM_TIMER_PROPERTIES_Result_Ok;
	} else if (	/* 16bit timers with HCLK clock frequency */
		TIMx == TIM1 ||
		TIMx == TIM8 ||
		TIMx == TIM9 ||
		TIMx == TIM10 ||
		TIMx == TIM11
	) {
		Timer_Data->TimerFrequency = RCC_ClocksStruct.HCLK_Frequency;	/* Clock */
		Timer_Data->MaxPeriod = 0xFFFF;								/* Max period */
		
		return TM_TIMER_PROPERTIES_Result_Ok;
	} else if (	/* 16bit timers with PCLK2 clock frequency */
		TIMx == TIM3 ||
		TIMx == TIM4 ||
		TIMx == TIM6 ||
		TIMx == TIM7 ||
		TIMx == TIM12 ||
		TIMx == TIM13 ||
		TIMx == TIM14
	) {
		Timer_Data->TimerFrequency = RCC_ClocksStruct.PCLK2_Frequency;	/* Clock */
		Timer_Data->MaxPeriod = 0xFFFF;								/* Max period */
		
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
	if (TIMx == TIM1) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	} else if (TIMx == TIM2) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	} else if (TIMx == TIM3) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	} else if (TIMx == TIM4) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	} else if (TIMx == TIM5) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	} else if (TIMx == TIM6) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	} else if (TIMx == TIM7) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	} else if (TIMx == TIM8) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	} else if (TIMx == TIM9) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	} else if (TIMx == TIM10) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	} else if (TIMx == TIM11) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
	} else if (TIMx == TIM12) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	} else if (TIMx == TIM13) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	} else if (TIMx == TIM14) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	}
	/* Return OK */
	return TM_TIMER_PROPERTIES_Result_Ok;
}

TM_TIMER_PROPERTIES_Result_t TM_TIMER_PROPERTIES_DisableClock(TIM_TypeDef* TIMx) {
	if (TIMx == TIM1) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, DISABLE);
	} else if (TIMx == TIM2) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
	} else if (TIMx == TIM3) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);
	} else if (TIMx == TIM4) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);
	} else if (TIMx == TIM5) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, DISABLE);
	} else if (TIMx == TIM6) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE);
	} else if (TIMx == TIM7) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, DISABLE);
	} else if (TIMx == TIM8) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, DISABLE);
	} else if (TIMx == TIM9) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, DISABLE);
	} else if (TIMx == TIM10) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, DISABLE);
	} else if (TIMx == TIM11) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, DISABLE);
	} else if (TIMx == TIM12) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, DISABLE);
	} else if (TIMx == TIM13) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, DISABLE);
	} else if (TIMx == TIM14) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, DISABLE);
	}
	/* Return OK */
	return TM_TIMER_PROPERTIES_Result_Ok;
}
