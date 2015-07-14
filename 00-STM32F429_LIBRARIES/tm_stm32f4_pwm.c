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
#include "tm_stm32f4_pwm.h"

/* Private functions */
void TM_PWM_INT_EnableClock(TIM_TypeDef* TIMx);
TM_PWM_Result_t TM_PWM_INT_GetTimerProperties(TIM_TypeDef* TIMx, uint32_t* frequency, uint32_t* period);

TM_PWM_Result_t TM_PWM_INT_InitTIM1Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack);
TM_PWM_Result_t TM_PWM_INT_InitTIM2Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack);
TM_PWM_Result_t TM_PWM_INT_InitTIM3Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack);
TM_PWM_Result_t TM_PWM_INT_InitTIM4Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack);
TM_PWM_Result_t TM_PWM_INT_InitTIM5Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack);
TM_PWM_Result_t TM_PWM_INT_InitTIM8Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack);
TM_PWM_Result_t TM_PWM_INT_InitTIM9Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack);
TM_PWM_Result_t TM_PWM_INT_InitTIM10Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack);
TM_PWM_Result_t TM_PWM_INT_InitTIM11Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack);
TM_PWM_Result_t TM_PWM_INT_InitTIM12Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack);
TM_PWM_Result_t TM_PWM_INT_InitTIM13Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack);
TM_PWM_Result_t TM_PWM_INT_InitTIM14Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack);

TM_PWM_Result_t TM_PWM_InitTimer(TIM_TypeDef* TIMx, TM_PWM_TIM_t* TIM_Data, double PWMFrequency) {
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	TM_TIMER_PROPERTIES_t Timer_Data;

	/* Check valid timer */
	if (0 
#ifdef TIM6
		|| TIMx == TIM6
#endif	
#ifdef TIM7
		|| TIMx == TIM7
#endif
	) {
		/* Timers TIM6 and TIM7 can not provide PWM feature */
		return TM_PWM_Result_TimerNotValid;
	}
	
	/* Save timer */
	TIM_Data->TIM = TIMx;
	
	/* Get timer properties */
	TM_TIMER_PROPERTIES_GetTimerProperties(TIMx, &Timer_Data);
	
	/* Check for maximum timer frequency */
	if (PWMFrequency > Timer_Data.TimerFrequency) {
		/* Frequency too high */
		return TM_PWM_Result_FrequencyTooHigh;
	} else if (PWMFrequency == 0) {
		/* Not valid frequency */
		return TM_PWM_Result_FrequencyTooLow;
	}

	/* Generate settings */
	TM_TIMER_PROPERTIES_GenerateDataForWorkingFrequency(&Timer_Data, PWMFrequency);
	
	/* Check valid data */
	if (Timer_Data.Period == 0) {
		/* Too high frequency */
		return TM_PWM_Result_FrequencyTooHigh;
	}
	
	/* Tests are OK */
	TIM_Data->Frequency = PWMFrequency;
	TIM_Data->Micros = 1000000 / PWMFrequency;
	TIM_Data->Period = Timer_Data.Period;
	TIM_Data->Prescaler = Timer_Data.Prescaler;
	
	/* Enable clock for Timer */	
	TM_TIMER_PROPERTIES_EnableClock(TIMx);

	/* Set timer options */
	TIM_BaseStruct.TIM_Prescaler = Timer_Data.Prescaler - 1;
	TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseStruct.TIM_Period = Timer_Data.Period - 1; 
	TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseStruct.TIM_RepetitionCounter = 0;
	
	/* Initialize timer */
	TIM_TimeBaseInit(TIMx, &TIM_BaseStruct);
	
	/* Preload enable */
	TIM_ARRPreloadConfig(TIMx, ENABLE);
	
	if (0
#ifdef TIM1
		|| TIMx == TIM1
#endif	
#ifdef TIM8
		|| TIMx == TIM8
#endif
	) {
		/* Enable PWM outputs */
		TIM_CtrlPWMOutputs(TIMx, ENABLE);
	}
	
	/* Start timer */
	TIMx->CR1 |= TIM_CR1_CEN;
	
	/* Set default values */
	TIM_Data->CH_Init = 0;
	
	/* Return OK */
	return TM_PWM_Result_Ok;
}


TM_PWM_Result_t TM_PWM_InitChannel(TM_PWM_TIM_t* TIM_Data, TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
#ifdef TIM1
	if (TIM_Data->TIM == TIM1) {
		return TM_PWM_INT_InitTIM1Pins(Channel, PinsPack);
	}  
#endif
#ifdef TIM2
	if (TIM_Data->TIM == TIM2) {
		return TM_PWM_INT_InitTIM2Pins(Channel, PinsPack);
	}  
#endif
#ifdef TIM3
	if (TIM_Data->TIM == TIM3) {
		return TM_PWM_INT_InitTIM3Pins(Channel, PinsPack);
	}  
#endif
#ifdef TIM4
	if (TIM_Data->TIM == TIM4) {
		return TM_PWM_INT_InitTIM4Pins(Channel, PinsPack);
	}  
#endif
#ifdef TIM5
	if (TIM_Data->TIM == TIM5) {
		return TM_PWM_INT_InitTIM5Pins(Channel, PinsPack);
	}  
#endif
#ifdef TIM8
	if (TIM_Data->TIM == TIM8) {
		return TM_PWM_INT_InitTIM8Pins(Channel, PinsPack);
	}  
#endif
#ifdef TIM9
	if (TIM_Data->TIM == TIM9) {
		return TM_PWM_INT_InitTIM9Pins(Channel, PinsPack);
	}  
#endif
#ifdef TIM10
	if (TIM_Data->TIM == TIM10) {
		return TM_PWM_INT_InitTIM10Pins(Channel, PinsPack);
	}  
#endif
#ifdef TIM11
	if (TIM_Data->TIM == TIM11) {
		return TM_PWM_INT_InitTIM11Pins(Channel, PinsPack);
	}  
#endif
#ifdef TIM12
	if (TIM_Data->TIM == TIM12) {
		return TM_PWM_INT_InitTIM12Pins(Channel, PinsPack);
	}
#endif
#ifdef TIM13
	if (TIM_Data->TIM == TIM13) {
		return TM_PWM_INT_InitTIM13Pins(Channel, PinsPack);
	} 
#endif
#ifdef TIM14
	if (TIM_Data->TIM == TIM14) {
		return TM_PWM_INT_InitTIM14Pins(Channel, PinsPack);
	}
#endif
	
	/* Timer is not valid */
	return TM_PWM_Result_TimerNotValid;
}

TM_PWM_Result_t TM_PWM_SetChannel(TM_PWM_TIM_t* TIM_Data, TM_PWM_Channel_t Channel, uint32_t Pulse) {
	TIM_OCInitTypeDef TIM_OCStruct;
	uint8_t ch = (uint8_t)Channel;
	
	/* Check pulse length */
	if (Pulse >= TIM_Data->Period) {
		/* Pulse too high */
		return TM_PWM_Result_PulseTooHigh;
	}

	/* Common settings */
	TIM_OCStruct.TIM_Pulse = Pulse;
	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	
	/* Save pulse value */
	TIM_Data->CH_Periods[ch] = Pulse;
	
	/* Go to bits */
	ch = 1 << ch;
	
	/* Select proper channel */
	switch (Channel) {
		case TM_PWM_Channel_1:
			/* Check if initialized */
			if (!(TIM_Data->CH_Init & ch)) {
				TIM_Data->CH_Init |= ch;
				
				/* Init channel */
				TIM_OC1Init(TIM_Data->TIM, &TIM_OCStruct);
				TIM_OC1PreloadConfig(TIM_Data->TIM, TIM_OCPreload_Enable);
			}
			
			/* Set pulse */
			TIM_Data->TIM->CCR1 = Pulse;
			break;
		case TM_PWM_Channel_2:
			/* Check if initialized */
			if (!(TIM_Data->CH_Init & ch)) {
				TIM_Data->CH_Init |= ch;
				
				/* Init channel */
				TIM_OC2Init(TIM_Data->TIM, &TIM_OCStruct);
				TIM_OC2PreloadConfig(TIM_Data->TIM, TIM_OCPreload_Enable);
			}
			
			/* Set pulse */
			TIM_Data->TIM->CCR2 = Pulse;
			break;
		case TM_PWM_Channel_3:
			/* Check if initialized */
			if (!(TIM_Data->CH_Init & ch)) {
				TIM_Data->CH_Init |= ch;
				
				/* Init channel */
				TIM_OC3Init(TIM_Data->TIM, &TIM_OCStruct);
				TIM_OC3PreloadConfig(TIM_Data->TIM, TIM_OCPreload_Enable);
			}
			
			/* Set pulse */
			TIM_Data->TIM->CCR3 = Pulse;
			break;
		case TM_PWM_Channel_4:
			/* Check if initialized */
			if (!(TIM_Data->CH_Init & ch)) {
				TIM_Data->CH_Init |= ch;
				
				/* Init channel */
				TIM_OC4Init(TIM_Data->TIM, &TIM_OCStruct);
				TIM_OC4PreloadConfig(TIM_Data->TIM, TIM_OCPreload_Enable);
			}
			
			/* Set pulse */
			TIM_Data->TIM->CCR4 = Pulse;
			break;
		default:
			break;
	}
	
	/* Return everything OK */
	return TM_PWM_Result_Ok;
}

TM_PWM_Result_t TM_PWM_SetChannelPercent(TM_PWM_TIM_t* TIM_Data, TM_PWM_Channel_t Channel, float percent) {
	/* Check input value */
	if (percent > 100) {
		return TM_PWM_SetChannel(TIM_Data, Channel, TIM_Data->Period);
	} else if (percent <= 0) {
		return TM_PWM_SetChannel(TIM_Data, Channel, 0);
	}
	
	/* Set channel value */
	return TM_PWM_SetChannel(TIM_Data, Channel, (uint32_t)((float)(TIM_Data->Period - 1) * percent) / 100);
}

TM_PWM_Result_t TM_PWM_SetChannelMicros(TM_PWM_TIM_t* TIM_Data, TM_PWM_Channel_t Channel, uint32_t micros) {
	/* If we choose too much micro seconds that we have valid */
	if (micros > TIM_Data->Micros) {
		/* Too high pulse */
		return TM_PWM_Result_PulseTooHigh;
	}
	
	/* Set PWM channel */
	return TM_PWM_SetChannel(TIM_Data, Channel, (uint32_t)((TIM_Data->Period - 1) * micros) / TIM_Data->Micros);
}

/* Private functions */
TM_PWM_Result_t TM_PWM_INT_InitTIM1Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
	TM_PWM_Result_t result = TM_PWM_Result_PinNotValid;
	
	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_8, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM1);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOE
					TM_GPIO_InitAlternate(GPIOE, GPIO_PIN_9, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM1);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_9, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM1);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOE
					TM_GPIO_InitAlternate(GPIOE, GPIO_PIN_10, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM1);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_3:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_10, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM1);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOE
					TM_GPIO_InitAlternate(GPIOE, GPIO_PIN_13, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM1);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_4:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_11, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM1);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOE
					TM_GPIO_InitAlternate(GPIOE, GPIO_PIN_14, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM1);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWM_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWM_Result_t TM_PWM_INT_InitTIM2Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
	TM_PWM_Result_t result = TM_PWM_Result_PinNotValid;
	
	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_0, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM2);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_5, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM2);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_3:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM2);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_1, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM2);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_3, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM2);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_3:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_2, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM2);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_10, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM2);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_4:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_3, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM2);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_11, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM2);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWM_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWM_Result_t TM_PWM_INT_InitTIM3Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
	TM_PWM_Result_t result = TM_PWM_Result_PinNotValid;
	
	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_6, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM3);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_4, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM3);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_3:
#ifdef GPIOC
					TM_GPIO_InitAlternate(GPIOC, GPIO_PIN_10, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM3);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM3);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_5, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM3);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_3:
#ifdef GPIOC
					TM_GPIO_InitAlternate(GPIOC, GPIO_PIN_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM3);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_3:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_0, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM3);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOC
					TM_GPIO_InitAlternate(GPIOC, GPIO_PIN_8, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM3);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_4:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_1, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM3);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOC
					TM_GPIO_InitAlternate(GPIOC, GPIO_PIN_9, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM3);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWM_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWM_Result_t TM_PWM_INT_InitTIM4Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
	TM_PWM_Result_t result = TM_PWM_Result_PinNotValid;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_6, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM4);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOD
					TM_GPIO_InitAlternate(GPIOD, GPIO_PIN_12, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM4);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM4);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOD
					TM_GPIO_InitAlternate(GPIOD, GPIO_PIN_13, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM4);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_3:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_8, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM4);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOD
					TM_GPIO_InitAlternate(GPIOD, GPIO_PIN_14, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM4);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_4:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_9, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM4);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOD
					TM_GPIO_InitAlternate(GPIOD, GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM4);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWM_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWM_Result_t TM_PWM_INT_InitTIM5Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
	TM_PWM_Result_t result = TM_PWM_Result_PinNotValid;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_0, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM5);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOH
					TM_GPIO_InitAlternate(GPIOH, GPIO_PIN_10, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM5);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_1, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM5);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOH
					TM_GPIO_InitAlternate(GPIOH, GPIO_PIN_11, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM5);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_3:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_2, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM5);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOH
					TM_GPIO_InitAlternate(GPIOH, GPIO_PIN_12, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM5);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_4:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_3, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM5);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOI
					TM_GPIO_InitAlternate(GPIOI, GPIO_PIN_0, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM5);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWM_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWM_Result_t TM_PWM_INT_InitTIM8Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
	TM_PWM_Result_t result = TM_PWM_Result_PinNotValid;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOC
					TM_GPIO_InitAlternate(GPIOC, GPIO_PIN_6, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM8);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOI
					TM_GPIO_InitAlternate(GPIOI, GPIO_PIN_5, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM8);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOC
					TM_GPIO_InitAlternate(GPIOC, GPIO_PIN_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM8);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOI
					TM_GPIO_InitAlternate(GPIOI, GPIO_PIN_6, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM8);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_3:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOC
					TM_GPIO_InitAlternate(GPIOC, GPIO_PIN_8, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM8);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOI
					TM_GPIO_InitAlternate(GPIOI, GPIO_PIN_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM8);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_4:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOC
					TM_GPIO_InitAlternate(GPIOC, GPIO_PIN_9, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM8);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOI
					TM_GPIO_InitAlternate(GPIOI, GPIO_PIN_8, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM8);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWM_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWM_Result_t TM_PWM_INT_InitTIM9Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
	TM_PWM_Result_t result = TM_PWM_Result_PinNotValid;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_2, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM9);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOE
					TM_GPIO_InitAlternate(GPIOE, GPIO_PIN_5, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM9);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_3, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM9);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOE
					TM_GPIO_InitAlternate(GPIOE, GPIO_PIN_6, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM9);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWM_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWM_Result_t TM_PWM_INT_InitTIM10Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
	TM_PWM_Result_t result = TM_PWM_Result_PinNotValid;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_8, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM10);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOF
					TM_GPIO_InitAlternate(GPIOF, GPIO_PIN_6, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM10);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWM_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWM_Result_t TM_PWM_INT_InitTIM11Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
	TM_PWM_Result_t result = TM_PWM_Result_PinNotValid;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_9, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM11);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOF
					TM_GPIO_InitAlternate(GPIOF, GPIO_PIN_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM11);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWM_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWM_Result_t TM_PWM_INT_InitTIM12Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
	TM_PWM_Result_t result = TM_PWM_Result_PinNotValid;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_14, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM12);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOH
					TM_GPIO_InitAlternate(GPIOH, GPIO_PIN_6, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM12);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOB
					TM_GPIO_InitAlternate(GPIOB, GPIO_PIN_15, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM12);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOH
					TM_GPIO_InitAlternate(GPIOH, GPIO_PIN_9, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM12);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWM_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWM_Result_t TM_PWM_INT_InitTIM13Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
	TM_PWM_Result_t result = TM_PWM_Result_PinNotValid;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_1, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM13);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOF
					TM_GPIO_InitAlternate(GPIOF, GPIO_PIN_8, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM13);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWM_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWM_Result_t TM_PWM_INT_InitTIM14Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
	TM_PWM_Result_t result = TM_PWM_Result_PinNotValid;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
#ifdef GPIOA
					TM_GPIO_InitAlternate(GPIOA, GPIO_PIN_7, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM14);
					result = TM_PWM_Result_Ok;
#endif
					break;
				case TM_PWM_PinsPack_2:
#ifdef GPIOF
					TM_GPIO_InitAlternate(GPIOF, GPIO_PIN_9, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_TIM14);
					result = TM_PWM_Result_Ok;
#endif
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWM_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

