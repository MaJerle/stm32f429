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
#include "tm_stm32f4_adc.h"

/* Private functions */
void TM_ADC_INT_Channel_0_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_1_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_2_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_3_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_4_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_5_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_6_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_7_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_8_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_9_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_10_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_11_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_12_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_13_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_14_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_Channel_15_Init(ADC_TypeDef* ADCx);
void TM_ADC_INT_InitPin(GPIO_TypeDef* GPIOx, uint16_t PinX);

void TM_ADC_Init(ADC_TypeDef* ADCx, uint8_t channel) {
	TM_ADC_Channel_t ch = (TM_ADC_Channel_t) channel;
	if (ch == TM_ADC_Channel_0) {
		TM_ADC_INT_Channel_0_Init(ADCx);
	} else if (ch == TM_ADC_Channel_1) {
		TM_ADC_INT_Channel_1_Init(ADCx);
	} else if (ch == TM_ADC_Channel_2) {
		TM_ADC_INT_Channel_2_Init(ADCx);
	} else if (ch == TM_ADC_Channel_3) {
		TM_ADC_INT_Channel_3_Init(ADCx);
	} else if (ch == TM_ADC_Channel_4) {
		TM_ADC_INT_Channel_4_Init(ADCx);
	} else if (ch == TM_ADC_Channel_5) {
		TM_ADC_INT_Channel_5_Init(ADCx);
	} else if (ch == TM_ADC_Channel_6) {
		TM_ADC_INT_Channel_6_Init(ADCx);
	} else if (ch == TM_ADC_Channel_7) {
		TM_ADC_INT_Channel_7_Init(ADCx);
	} else if (ch == TM_ADC_Channel_8) {
		TM_ADC_INT_Channel_8_Init(ADCx);
	} else if (ch == TM_ADC_Channel_9) {
		TM_ADC_INT_Channel_9_Init(ADCx);
	} else if (ch == TM_ADC_Channel_10) {
		TM_ADC_INT_Channel_10_Init(ADCx);
	} else if (ch == TM_ADC_Channel_11) {
		TM_ADC_INT_Channel_11_Init(ADCx);
	} else if (ch == TM_ADC_Channel_12) {
		TM_ADC_INT_Channel_12_Init(ADCx);
	} else if (ch == TM_ADC_Channel_13) {
		TM_ADC_INT_Channel_13_Init(ADCx);
	} else if (ch == TM_ADC_Channel_14) {
		TM_ADC_INT_Channel_14_Init(ADCx);
	} else if (ch == TM_ADC_Channel_15) {
		TM_ADC_INT_Channel_15_Init(ADCx);
	}
	
	/* Init ADC */
	TM_ADC_InitADC(ADCx);
}

void TM_ADC_InitADC(ADC_TypeDef* ADCx) {
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;

	/* Init ADC settings */
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	
	/* Enable clock and fill resolution settings */
	if (ADCx == ADC1) {
		/* Enable ADC clock */
		RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
		
		/* Set resolution */
		ADC_InitStruct.ADC_Resolution = TM_ADC1_RESOLUTION;
	} else if (ADCx == ADC2) {
		/* Enable ADC clock */
		RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
		
		/* Set resolution */
		ADC_InitStruct.ADC_Resolution = TM_ADC2_RESOLUTION;
	} else if (ADCx == ADC3) {
		/* Enable ADC clock */
		RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
		
		/* Set resolution */
		ADC_InitStruct.ADC_Resolution = TM_ADC3_RESOLUTION;
	}

	/* Set common ADC settings */
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_8Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	/* Initialize ADC */
	ADC_Init(ADCx, &ADC_InitStruct);
	
	/* Enable ADC */
	ADCx->CR2 |= ADC_CR2_ADON;
}

uint16_t TM_ADC_Read(ADC_TypeDef* ADCx, uint8_t channel) {
	uint32_t timeout = 0xFFF;
	
	ADC_RegularChannelConfig(ADCx, channel, 1, ADC_SampleTime_15Cycles);

	/* Start software conversion */
	ADCx->CR2 |= (uint32_t)ADC_CR2_SWSTART;
	
	/* Wait till done */
	while (!(ADCx->SR & ADC_SR_EOC)) {
		if (timeout-- == 0x00) {
			return 0;
		}
	}
	
	/* Return result */
	return ADCx->DR;
}

void TM_ADC_EnableVbat(void) {
	/* Enable VBAT */
	ADC->CCR |= ADC_CCR_VBATE;
}

void TM_ADC_DisableVbat(void) {
	/* Disable VBAT */
	ADC->CCR &= ~ADC_CCR_VBATE;
}

uint16_t TM_ADC_ReadVbat(ADC_TypeDef* ADCx) {
	uint32_t result;
	/* Read battery voltage data */
	/* Start conversion */
	ADC_RegularChannelConfig(ADCx, ADC_Channel_Vbat, 1, ADC_SampleTime_112Cycles);
	
	/* Start software conversion */
	ADCx->CR2 |= (uint32_t)ADC_CR2_SWSTART;
	
	/* Wait till done */
	while (!(ADCx->SR & ADC_SR_EOC));
	
	/* Get result */
	result = ADCx->DR;
	
	/* Convert to voltage */
	result = result * ADC_VBAT_MULTI * ADC_SUPPLY_VOLTAGE / 0xFFF;
	
	/* Return value in mV */
	return (uint16_t) result;
}

/* Private functions */
void TM_ADC_INT_Channel_0_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(GPIOA, GPIO_PIN_0);
}
void TM_ADC_INT_Channel_1_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(GPIOA, GPIO_PIN_1);
}
void TM_ADC_INT_Channel_2_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(GPIOA, GPIO_PIN_2);
}
void TM_ADC_INT_Channel_3_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(GPIOA, GPIO_PIN_3);
}
void TM_ADC_INT_Channel_4_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(GPIOA, GPIO_PIN_4);
	} 
	if (ADCx == ADC3) {
#if defined(GPIOF)
		TM_ADC_INT_InitPin(GPIOF, GPIO_PIN_6);
#endif
	}
}
void TM_ADC_INT_Channel_5_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(GPIOA, GPIO_PIN_5);
	} 
	if (ADCx == ADC3) {
#if defined(GPIOF)
		TM_ADC_INT_InitPin(GPIOF, GPIO_PIN_7);
#endif
	}
}
void TM_ADC_INT_Channel_6_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(GPIOA, GPIO_PIN_6);
	} 
	if (ADCx == ADC3) {
#if defined(GPIOF)
		TM_ADC_INT_InitPin(GPIOF, GPIO_PIN_8);
#endif
	}
}
void TM_ADC_INT_Channel_7_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(GPIOA, GPIO_PIN_7);
	} 
	if (ADCx == ADC3) {
#if defined(GPIOF)
		TM_ADC_INT_InitPin(GPIOF, GPIO_PIN_9);
#endif
	}
}
void TM_ADC_INT_Channel_8_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(GPIOB, GPIO_PIN_0);
	} 
	if (ADCx == ADC3) {
#if defined(GPIOF)
		TM_ADC_INT_InitPin(GPIOF, GPIO_PIN_10);
#endif
	}
}
void TM_ADC_INT_Channel_9_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(GPIOB, GPIO_PIN_1);
	} 
	if (ADCx == ADC3) {
#if defined(GPIOF)
		TM_ADC_INT_InitPin(GPIOF, GPIO_PIN_3);
#endif
	}
}
void TM_ADC_INT_Channel_10_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(GPIOC, GPIO_PIN_0);
}
void TM_ADC_INT_Channel_11_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(GPIOC, GPIO_PIN_1);
}
void TM_ADC_INT_Channel_12_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(GPIOC, GPIO_PIN_2);
}
void TM_ADC_INT_Channel_13_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(GPIOC, GPIO_PIN_3);
}
void TM_ADC_INT_Channel_14_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(GPIOC, GPIO_PIN_4);
	} 
	if (ADCx == ADC3) {
#if defined(GPIOF)
		TM_ADC_INT_InitPin(GPIOF, GPIO_PIN_4);
#endif
	}
}
void TM_ADC_INT_Channel_15_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(GPIOC, GPIO_PIN_5);
	} else if (ADCx == ADC3) {
#if defined(GPIOF)
		TM_ADC_INT_InitPin(GPIOF, GPIO_PIN_5);
#endif
	}
}

void TM_ADC_INT_InitPin(GPIO_TypeDef* GPIOx, uint16_t PinX) {
	/* Enable GPIO */
	TM_GPIO_Init(GPIOx, PinX, TM_GPIO_Mode_AN, TM_GPIO_OType_PP, TM_GPIO_PuPd_DOWN, TM_GPIO_Speed_Medium);
}
