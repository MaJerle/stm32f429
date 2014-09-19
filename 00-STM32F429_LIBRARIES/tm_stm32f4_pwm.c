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
	if (TIMx == TIM6 || TIMx == TIM7) {
		return TM_PWM_Result_TimerNotValid;
	}
	
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
	/* Start timer */
	TIM_Cmd(TIMx, ENABLE);
	
	/* Return OK */
	return TM_PWM_Result_Ok;
}


TM_PWM_Result_t TM_PWM_InitChannel(TIM_TypeDef* TIMx, TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
	if (TIMx == TIM1) {
		return TM_PWM_INT_InitTIM1Pins(Channel, PinsPack);
	} else if (TIMx == TIM2) {
		return TM_PWM_INT_InitTIM2Pins(Channel, PinsPack);
	} else if (TIMx == TIM3) {
		return TM_PWM_INT_InitTIM3Pins(Channel, PinsPack);
	} else if (TIMx == TIM4) {
		return TM_PWM_INT_InitTIM4Pins(Channel, PinsPack);
	} else if (TIMx == TIM5) {
		return TM_PWM_INT_InitTIM5Pins(Channel, PinsPack);
	} else if (TIMx == TIM8) {
		return TM_PWM_INT_InitTIM8Pins(Channel, PinsPack);
	} else if (TIMx == TIM9) {
		return TM_PWM_INT_InitTIM9Pins(Channel, PinsPack);
	} else if (TIMx == TIM10) {
		return TM_PWM_INT_InitTIM10Pins(Channel, PinsPack);
	} else if (TIMx == TIM11) {
		return TM_PWM_INT_InitTIM11Pins(Channel, PinsPack);
	} else if (TIMx == TIM12) {
		return TM_PWM_INT_InitTIM12Pins(Channel, PinsPack);
	} else if (TIMx == TIM13) {
		return TM_PWM_INT_InitTIM13Pins(Channel, PinsPack);
	} else if (TIMx == TIM14) {
		return TM_PWM_INT_InitTIM14Pins(Channel, PinsPack);
	}
	/* Timer is not valid */
	return TM_PWM_Result_TimerNotValid;
}

TM_PWM_Result_t TM_PWM_SetChannel(TIM_TypeDef* TIMx, TM_PWM_TIM_t* TIM_Data, TM_PWM_Channel_t Channel, uint32_t Pulse) {
	TIM_OCInitTypeDef TIM_OCStruct;
	
	/* Check pulse length */
	if (Pulse > (TIM_Data->Period - 1)) {
		return TM_PWM_Result_PulseTooHigh;
	}

	/* Common settings */
	TIM_OCStruct.TIM_Pulse = Pulse;
	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	
	switch (Channel) {
		case TM_PWM_Channel_1:
			TIM_OC1Init(TIMx, &TIM_OCStruct);
			TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
			break;
		case TM_PWM_Channel_2:
			TIM_OC2Init(TIMx, &TIM_OCStruct);
			TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
			break;
		case TM_PWM_Channel_3:
			TIM_OC3Init(TIMx, &TIM_OCStruct);
			TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
			break;
		case TM_PWM_Channel_4:
			TIM_OC4Init(TIMx, &TIM_OCStruct);
			TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
			break;
		default:
			break;
	}
	
	return TM_PWM_Result_Ok;
}

TM_PWM_Result_t TM_PWM_SetChannelPercent(TIM_TypeDef* TIMx, TM_PWM_TIM_t* TIM_Data, TM_PWM_Channel_t Channel, float percent) {
	if (percent > 100) {
		return TM_PWM_SetChannel(TIMx, TIM_Data, Channel, TIM_Data->Period);
	} else if (percent <= 0) {
		return TM_PWM_SetChannel(TIMx, TIM_Data, Channel, 0);
	}
	return TM_PWM_SetChannel(TIMx, TIM_Data, Channel, (uint32_t)((float)(TIM_Data->Period - 1) * percent) / 100);
}

TM_PWM_Result_t TM_PWM_SetChannelMicros(TIM_TypeDef* TIMx, TM_PWM_TIM_t* TIM_Data, TM_PWM_Channel_t Channel, uint32_t micros) {
	if (micros > TIM_Data->Micros) {
		return TM_PWM_Result_PulseTooHigh;
	}
	return TM_PWM_SetChannel(TIMx, TIM_Data, Channel, (uint32_t)((TIM_Data->Period - 1) * micros) / TIM_Data->Micros);
}

TM_PWM_Result_t TM_PWM_INT_InitTIM1Pins(TM_PWM_Channel_t Channel, TM_PWM_PinsPack_t PinsPack) {
	TM_PWM_Result_t result;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;					/* Set pin */
					GPIO_Init(GPIOE, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_TIM1);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;					/* Set pin */
					GPIO_Init(GPIOE, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_3:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;					/* Set pin */
					GPIO_Init(GPIOE, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_4:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;					/* Set pin */
					GPIO_Init(GPIOE, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
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
	TM_PWM_Result_t result;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_3:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_3:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_4:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_TIM2);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
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
	TM_PWM_Result_t result;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_3:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOC, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_3:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;					/* Set pin */
					GPIO_Init(GPIOC, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_3:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;					/* Set pin */
					GPIO_Init(GPIOC, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_4:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;					/* Set pin */
					GPIO_Init(GPIOC, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
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
	TM_PWM_Result_t result;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;					/* Set pin */
					GPIO_Init(GPIOD, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;					/* Set pin */
					GPIO_Init(GPIOD, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_3:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;					/* Set pin */
					GPIO_Init(GPIOD, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_4:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;					/* Set pin */
					GPIO_Init(GPIOD, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
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
	TM_PWM_Result_t result;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOH, GPIO_PinSource10, GPIO_AF_TIM5);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;					/* Set pin */
					GPIO_Init(GPIOH, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOH, GPIO_PinSource11, GPIO_AF_TIM5);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;					/* Set pin */
					GPIO_Init(GPIOH, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_3:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOH, GPIO_PinSource12, GPIO_AF_TIM5);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;					/* Set pin */
					GPIO_Init(GPIOH, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_4:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOI, GPIO_PinSource0, GPIO_AF_TIM5);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;					/* Set pin */
					GPIO_Init(GPIOI, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
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
	TM_PWM_Result_t result;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOC, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOI, GPIO_PinSource5, GPIO_AF_TIM8);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;					/* Set pin */
					GPIO_Init(GPIOI, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;					/* Set pin */
					GPIO_Init(GPIOC, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_TIM8);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOI, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_3:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM8);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;					/* Set pin */
					GPIO_Init(GPIOC, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_TIM8);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;					/* Set pin */
					GPIO_Init(GPIOI, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_4:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;					/* Set pin */
					GPIO_Init(GPIOC, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOI, GPIO_PinSource2, GPIO_AF_TIM8);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;					/* Set pin */
					GPIO_Init(GPIOI, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
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
	TM_PWM_Result_t result;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;					/* Set pin */
					GPIO_Init(GPIOE, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM9);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_TIM9);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOE, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
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
	TM_PWM_Result_t result;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM10);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_TIM10);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOF, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
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
	TM_PWM_Result_t result;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM11);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_TIM11);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;					/* Set pin */
					GPIO_Init(GPIOF, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
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
	TM_PWM_Result_t result;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM12);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOH, GPIO_PinSource6, GPIO_AF_TIM12);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOH, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWM_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWM_Channel_2:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;					/* Set pin */
					GPIO_Init(GPIOB, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOH, GPIO_PinSource9, GPIO_AF_TIM12);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;					/* Set pin */
					GPIO_Init(GPIOH, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
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
	TM_PWM_Result_t result;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM13);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;					/* Set pin */
					GPIO_Init(GPIOF, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
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
	TM_PWM_Result_t result;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	switch (Channel) {
		case TM_PWM_Channel_1:
			switch (PinsPack) {
				case TM_PWM_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM14);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;					/* Set pin */
					GPIO_Init(GPIOA, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
					break;
				case TM_PWM_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);	/* Alternate function */
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;					/* Set pin */
					GPIO_Init(GPIOF, &GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWM_Result_Ok;								/* Result OK */
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

