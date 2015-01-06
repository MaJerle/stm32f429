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
#include "tm_stm32f4_pwmin.h"

/* Private functions */
TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM1Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack);
TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM2Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack);
TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM3Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack);
TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM4Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack);
TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM5Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack);
TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM8Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack);
TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM9Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack);
TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM10Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack);
TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM11Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack);
TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM12Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack);
TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM13Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack);
TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM14Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack);
TM_PWMIN_Result_t TM_PWMIN_INT_InitChannel(TM_PWMIN_t* TIM_Data, TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack);

/* Private variables */
__IO float IC2Value = 0;
__IO float DutyCycle = 0;
__IO float Frequency = 0;
GPIO_InitTypeDef PWMIN_GPIO_InitStruct;
static uint16_t PWMIN_SubPriorities = 0;

TM_PWMIN_Result_t TM_PWMIN_InitTimer(
	TIM_TypeDef* TIMx,
	TM_PWMIN_t* PWMIN_Data,
	TM_PWMIN_Channel_t PWMIN_Channel,
	TM_PWMIN_PinsPack_t PinsPack,
	float MinExpectedFrequency,
	IRQn_Type TIMx_IRQn
) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	TM_PWMIN_Result_t res;
	
	/* Check timer */
	if (TIMx == TIM6 || TIMx == TIM7) {
		/* Invalid timer */
		return TM_PWMIN_Result_TimerNotValid;
	}
	
	/* Save timer and settings */
	PWMIN_Data->__TIM = TIMx;
	
	/* Get properties about timer */
	TM_TIMER_PROPERTIES_GetTimerProperties(PWMIN_Data->__TIM, &PWMIN_Data->__TIM_Data);
	
	/* Generate settings */
	TM_TIMER_PROPERTIES_GenerateDataForWorkingFrequency(&PWMIN_Data->__TIM_Data, MinExpectedFrequency);
	
	/* Enable TIM clock */
	TM_TIMER_PROPERTIES_EnableClock(PWMIN_Data->__TIM);
	
	/* Set timer settings */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);
	if (PWMIN_Data->__TIM_Data.MaxPeriod == 0x00010000) {
		TIM_TimeBaseStruct.TIM_Period = 0x0000FFFF; /* 16-bit */
	} else {
		TIM_TimeBaseStruct.TIM_Period = PWMIN_Data->__TIM_Data.MaxPeriod;
	}
	TIM_TimeBaseStruct.TIM_Prescaler = PWMIN_Data->__TIM_Data.Prescaler - 1;
	
	/* Initialize TIM */
	TIM_TimeBaseInit(PWMIN_Data->__TIM, &TIM_TimeBaseStruct);
	
	/* Default GPIO settings */
	PWMIN_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	PWMIN_GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	PWMIN_GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	PWMIN_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	/* Enable TIMER pin */
	if ((res = TM_PWMIN_INT_InitChannel(PWMIN_Data, PWMIN_Channel, PinsPack)) != TM_PWMIN_Result_Ok) {
		/* Return result */
		return res;
	}

	/* Calculate subpriority */
	for (PWMIN_Data->__SubPriority = 0; PWMIN_Data->__SubPriority < 16; PWMIN_Data->__SubPriority++) {
		/* Check if it is available */
		if (!(PWMIN_SubPriorities & (1 << PWMIN_Data->__SubPriority))) {
			/* Available */
			PWMIN_SubPriorities |= 1 << PWMIN_Data->__SubPriority;
			
			/* Stop the loop */
			break;
		}
	}
	
	/* Enable the TIM global Interrupt */
	NVIC_InitStruct.NVIC_IRQChannel = TIMx_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = PWMIN_PRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = PWMIN_Data->__SubPriority;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/* Set timer settings */
	if (PWMIN_Channel == TM_PWMIN_Channel_1) {
		TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	} else {
		TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	}
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICFilter = 0x0;

	/* Initialize PWM Input config */
	TIM_PWMIConfig(PWMIN_Data->__TIM, &TIM_ICInitStruct);

	/* Select input trigger */
	if (PWMIN_Channel == TM_PWMIN_Channel_1) {
		TIM_SelectInputTrigger(PWMIN_Data->__TIM, TIM_TS_TI1FP1);
	} else {
		TIM_SelectInputTrigger(PWMIN_Data->__TIM, TIM_TS_TI2FP2);
	}

	/* Select the slave Mode: Reset Mode */
	TIM_SelectSlaveMode(PWMIN_Data->__TIM, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(PWMIN_Data->__TIM, TIM_MasterSlaveMode_Enable);

	/* TIM enable counter */
	TIM_Cmd(PWMIN_Data->__TIM, ENABLE);

	/* Enable compare interrupt request */
	if (PWMIN_Channel == TM_PWMIN_Channel_1) {
		TIM_ITConfig(PWMIN_Data->__TIM, TIM_IT_CC1, ENABLE);
	} else {
		TIM_ITConfig(PWMIN_Data->__TIM, TIM_IT_CC2, ENABLE);
	}
	
	/* Return OK */
	return TM_PWMIN_Result_Ok;
}

TM_PWMIN_Result_t TM_PWMIN_Get(TM_PWMIN_t* PWMIN_Data) {
	/* Set new data */
	PWMIN_Data->Frequency = PWMIN_Data->__Freq;
	PWMIN_Data->DutyCycle = PWMIN_Data->__Duty;
	
	/* Reset data for future calls */
	PWMIN_Data->__Freq = 0;
	PWMIN_Data->__Duty = 0;
	
	/* Return Ok */
	return TM_PWMIN_Result_Ok;
}

TM_PWMIN_Result_t TM_PWMIN_InterruptHandler(TM_PWMIN_t* PWMIN_Data) {
	/* Interrupt handler for PWMIN */
	
	if (TIM_GetITStatus(PWMIN_Data->__TIM, TIM_IT_CC1)) {
		/* Read data */		

		/* Clear pending bit */
		TIM_ClearITPendingBit(PWMIN_Data->__TIM, TIM_IT_CC1);
		
		/* Get the Input Capture value */
		IC2Value = (float)TIM_GetCapture1(PWMIN_Data->__TIM);

		if (IC2Value != 0) {
			/* Duty cycle and frequency */
			DutyCycle = ((float)TIM_GetCapture2(PWMIN_Data->__TIM) * 100) / IC2Value;
			Frequency = (float)PWMIN_Data->__TIM_Data.TimerFrequency / (float)(IC2Value * PWMIN_Data->__TIM_Data.Prescaler);
			
			/* Save data */
			PWMIN_Data->__Duty = DutyCycle;
			PWMIN_Data->__Freq = Frequency;
		} else {
			/* Reset data */
			DutyCycle = 0;
			Frequency = 0;
		}
	}
	if (TIM_GetITStatus(PWMIN_Data->__TIM, TIM_IT_CC2)) {
		/* Read data */
		
		/* Clear pending bit */
		TIM_ClearITPendingBit(PWMIN_Data->__TIM, TIM_IT_CC2);
		
		/* Get the Input Capture value */
		IC2Value = (float)TIM_GetCapture2(PWMIN_Data->__TIM);

		if (IC2Value != 0) {
			/* Duty cycle and frequency */
			DutyCycle = ((float)TIM_GetCapture1(PWMIN_Data->__TIM) * 100) / IC2Value;
			Frequency = (float)PWMIN_Data->__TIM_Data.TimerFrequency / (IC2Value * PWMIN_Data->__TIM_Data.Prescaler);
			
			/* Save data */
			PWMIN_Data->__Duty = DutyCycle;
			PWMIN_Data->__Freq = Frequency;
		} else {
			/* Reset data */
			DutyCycle = 0;
			Frequency = 0;
		}
	}
	
	/* Return OK */
	return TM_PWMIN_Result_Ok;
}

TM_PWMIN_Result_t TM_PWMIN_INT_InitChannel(TM_PWMIN_t* TIM_Data, TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack) {
	if (TIM_Data->__TIM == TIM1) {
		return TM_PWMIN_INT_InitTIM1Pins(Channel, PinsPack);
	} else if (TIM_Data->__TIM == TIM2) {
		return TM_PWMIN_INT_InitTIM2Pins(Channel, PinsPack);
	} else if (TIM_Data->__TIM == TIM3) {
		return TM_PWMIN_INT_InitTIM3Pins(Channel, PinsPack);
	} else if (TIM_Data->__TIM == TIM4) {
		return TM_PWMIN_INT_InitTIM4Pins(Channel, PinsPack);
	} else if (TIM_Data->__TIM == TIM5) {
		return TM_PWMIN_INT_InitTIM5Pins(Channel, PinsPack);
	} else if (TIM_Data->__TIM == TIM8) {
		return TM_PWMIN_INT_InitTIM8Pins(Channel, PinsPack);
	} else if (TIM_Data->__TIM == TIM9) {
		return TM_PWMIN_INT_InitTIM9Pins(Channel, PinsPack);
	} else if (TIM_Data->__TIM == TIM10) {
		return TM_PWMIN_INT_InitTIM10Pins(Channel, PinsPack);
	} else if (TIM_Data->__TIM == TIM11) {
		return TM_PWMIN_INT_InitTIM11Pins(Channel, PinsPack);
	} else if (TIM_Data->__TIM == TIM12) {
		return TM_PWMIN_INT_InitTIM12Pins(Channel, PinsPack);
	} else if (TIM_Data->__TIM == TIM13) {
		return TM_PWMIN_INT_InitTIM13Pins(Channel, PinsPack);
	} else if (TIM_Data->__TIM == TIM14) {
		return TM_PWMIN_INT_InitTIM14Pins(Channel, PinsPack);
	}
	
	/* Timer is not valid */
	return TM_PWMIN_Result_TimerNotValid;
}

TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM1Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack) {
	TM_PWMIN_Result_t result;
	
	switch (Channel) {
		case TM_PWMIN_Channel_1:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;					/* Set pin */
					GPIO_Init(GPIOA, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;					/* Set pin */
					GPIO_Init(GPIOE, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWMIN_Channel_2:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;					/* Set pin */
					GPIO_Init(GPIOA, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_TIM1);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;					/* Set pin */
					GPIO_Init(GPIOE, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWMIN_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM2Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack) {
	TM_PWMIN_Result_t result;
	
	switch (Channel) {
		case TM_PWMIN_Channel_1:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;					/* Set pin */
					GPIO_Init(GPIOA, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;					/* Set pin */
					GPIO_Init(GPIOA, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_3:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;					/* Set pin */
					GPIO_Init(GPIOA, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWMIN_Channel_2:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;					/* Set pin */
					GPIO_Init(GPIOA, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;					/* Set pin */
					GPIO_Init(GPIOB, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWMIN_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM3Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack) {
	TM_PWMIN_Result_t result;
	
	switch (Channel) {
		case TM_PWMIN_Channel_1:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOA, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;					/* Set pin */
					GPIO_Init(GPIOB, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_3:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOC, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWMIN_Channel_2:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;					/* Set pin */
					GPIO_Init(GPIOA, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;					/* Set pin */
					GPIO_Init(GPIOB, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_3:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;					/* Set pin */
					GPIO_Init(GPIOC, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWMIN_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM4Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack) {
	TM_PWMIN_Result_t result;

	switch (Channel) {
		case TM_PWMIN_Channel_1:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOB, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;					/* Set pin */
					GPIO_Init(GPIOD, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWMIN_Channel_2:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;					/* Set pin */
					GPIO_Init(GPIOB, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;					/* Set pin */
					GPIO_Init(GPIOD, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWMIN_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM5Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack) {
	TM_PWMIN_Result_t result;

	switch (Channel) {
		case TM_PWMIN_Channel_1:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;					/* Set pin */
					GPIO_Init(GPIOA, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOH, GPIO_PinSource10, GPIO_AF_TIM5);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;					/* Set pin */
					GPIO_Init(GPIOH, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWMIN_Channel_2:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;					/* Set pin */
					GPIO_Init(GPIOA, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOH, GPIO_PinSource11, GPIO_AF_TIM5);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;					/* Set pin */
					GPIO_Init(GPIOH, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWMIN_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM8Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack) {
	TM_PWMIN_Result_t result;

	switch (Channel) {
		case TM_PWMIN_Channel_1:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOC, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOI, GPIO_PinSource5, GPIO_AF_TIM8);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;					/* Set pin */
					GPIO_Init(GPIOI, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWMIN_Channel_2:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;					/* Set pin */
					GPIO_Init(GPIOC, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_TIM8);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOI, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWMIN_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM9Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack) {
	TM_PWMIN_Result_t result;

	switch (Channel) {
		case TM_PWMIN_Channel_1:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;					/* Set pin */
					GPIO_Init(GPIOA, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;					/* Set pin */
					GPIO_Init(GPIOE, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWMIN_Channel_2:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM9);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;					/* Set pin */
					GPIO_Init(GPIOA, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_TIM9);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOE, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWMIN_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM10Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack) {
	TM_PWMIN_Result_t result;

	switch (Channel) {
		case TM_PWMIN_Channel_1:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM10);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;					/* Set pin */
					GPIO_Init(GPIOB, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_TIM10);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOF, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWMIN_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM11Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack) {
	TM_PWMIN_Result_t result;

	switch (Channel) {
		case TM_PWMIN_Channel_1:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM11);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;					/* Set pin */
					GPIO_Init(GPIOB, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_TIM11);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;					/* Set pin */
					GPIO_Init(GPIOF, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWMIN_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM12Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack) {
	TM_PWMIN_Result_t result;

	switch (Channel) {
		case TM_PWMIN_Channel_1:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM12);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;					/* Set pin */
					GPIO_Init(GPIOB, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOH, GPIO_PinSource6, GPIO_AF_TIM12);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOH, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		case TM_PWMIN_Channel_2:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;					/* Set pin */
					GPIO_Init(GPIOB, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOH, GPIO_PinSource9, GPIO_AF_TIM12);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;					/* Set pin */
					GPIO_Init(GPIOH, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWMIN_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM13Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack) {
	TM_PWMIN_Result_t result;

	switch (Channel) {
		case TM_PWMIN_Channel_1:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM13);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;					/* Set pin */
					GPIO_Init(GPIOA, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;					/* Set pin */
					GPIO_Init(GPIOF, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWMIN_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

TM_PWMIN_Result_t TM_PWMIN_INT_InitTIM14Pins(TM_PWMIN_Channel_t Channel, TM_PWMIN_PinsPack_t PinsPack) {
	TM_PWMIN_Result_t result;

	switch (Channel) {
		case TM_PWMIN_Channel_1:
			switch (PinsPack) {
				case TM_PWMIN_PinsPack_1:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM14);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;					/* Set pin */
					GPIO_Init(GPIOA, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				case TM_PWMIN_PinsPack_2:
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	/* Enable clock */
					GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);	/* Alternate function */
					PWMIN_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;					/* Set pin */
					GPIO_Init(GPIOF, &PWMIN_GPIO_InitStruct);						/* Initialize pin */
					result = TM_PWMIN_Result_Ok;								/* Result OK */
					break;
				default:
					result = TM_PWMIN_Result_PinNotValid;
					break;
			}
			break;
		default:
			result = TM_PWMIN_Result_ChannelNotValid;
			break;
	}
	
	return result;
}

