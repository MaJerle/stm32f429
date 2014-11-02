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

/* Internal functions */
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
void TM_ADC_INT_InitPin(uint16_t RCCx, GPIO_TypeDef* GPIOx, uint16_t PinX);

uint8_t vbatEnabled = 0;
uint8_t mult_vref = 0;
uint8_t mult_vbat = 0;

void TM_ADC_Init(ADC_TypeDef* ADCx, uint8_t channel) {
	if (channel == ADC_Channel_0) {
		TM_ADC_INT_Channel_0_Init(ADCx);
	} else if (channel == ADC_Channel_1) {
		TM_ADC_INT_Channel_1_Init(ADCx);
	} else if (channel == ADC_Channel_2) {
		TM_ADC_INT_Channel_2_Init(ADCx);
	} else if (channel == ADC_Channel_3) {
		TM_ADC_INT_Channel_3_Init(ADCx);
	} else if (channel == ADC_Channel_4) {
		TM_ADC_INT_Channel_4_Init(ADCx);
	} else if (channel == ADC_Channel_5) {
		TM_ADC_INT_Channel_5_Init(ADCx);
	} else if (channel == ADC_Channel_6) {
		TM_ADC_INT_Channel_6_Init(ADCx);
	} else if (channel == ADC_Channel_7) {
		TM_ADC_INT_Channel_7_Init(ADCx);
	} else if (channel == ADC_Channel_8) {
		TM_ADC_INT_Channel_8_Init(ADCx);
	} else if (channel == ADC_Channel_9) {
		TM_ADC_INT_Channel_9_Init(ADCx);
	} else if (channel == ADC_Channel_10) {
		TM_ADC_INT_Channel_10_Init(ADCx);
	} else if (channel == ADC_Channel_11) {
		TM_ADC_INT_Channel_11_Init(ADCx);
	} else if (channel == ADC_Channel_12) {
		TM_ADC_INT_Channel_12_Init(ADCx);
	} else if (channel == ADC_Channel_13) {
		TM_ADC_INT_Channel_13_Init(ADCx);
	} else if (channel == ADC_Channel_14) {
		TM_ADC_INT_Channel_14_Init(ADCx);
	} else if (channel == ADC_Channel_15) {
		TM_ADC_INT_Channel_15_Init(ADCx);
	}
	
	/* Init ADC */
	TM_ADC_InitADC(ADCx);
}

void TM_ADC_InitADC(ADC_TypeDef* ADCx) {
	ADC_InitTypeDef ADC_InitDef;
	ADC_CommonInitTypeDef ADC_CommonInitDef;

	ADC_InitDef.ADC_ContinuousConvMode = DISABLE;
	ADC_InitDef.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitDef.ADC_ExternalTrigConv = DISABLE;
	ADC_InitDef.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitDef.ADC_NbrOfConversion = 1;
	ADC_InitDef.ADC_ScanConvMode = DISABLE;
	
	if (ADCx == ADC1) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		ADC_InitDef.ADC_Resolution = TM_ADC1_RESOLUTION;
	} else if (ADCx == ADC2) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
		ADC_InitDef.ADC_Resolution = TM_ADC2_RESOLUTION;
	} else if (ADCx == ADC3) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
		ADC_InitDef.ADC_Resolution = TM_ADC3_RESOLUTION;
	}

	ADC_CommonInitDef.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitDef.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitDef.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitDef.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_8Cycles;
	ADC_CommonInit(&ADC_CommonInitDef);
	
	/* Initialize ADC */
	ADC_Init(ADCx, &ADC_InitDef);
	/* Enable ADC */
	ADC_Cmd(ADCx, ENABLE);
}

uint16_t TM_ADC_Read(ADC_TypeDef* ADCx, uint8_t channel) {
	ADC_RegularChannelConfig(ADCx, channel, 1, ADC_SampleTime_15Cycles);
	ADC_SoftwareStartConv(ADCx);
	
	/* Wait till done */
	while (ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
	
	/* Return result */
	return ADC_GetConversionValue(ADCx);
}

void TM_ADC_EnableVbat(void) {
	/* Enable VBAT */
	ADC_VBATCmd(ENABLE);
	
	/* Store vbat enabled */
	vbatEnabled = 1;
}

void TM_ADC_DisableVbat(void) {
	/* Disable VBAT */
	ADC_VBATCmd(DISABLE);
	
	/* Store vbat enabled */
	vbatEnabled = 0;
}

uint16_t TM_ADC_ReadVbat(ADC_TypeDef* ADCx) {
	uint32_t result;
	/* Read battery voltage data */
	/* Start conversion */
	ADC_RegularChannelConfig(ADCx, ADC_Channel_Vbat, 1, ADC_SampleTime_112Cycles);
	ADC_SoftwareStartConv(ADCx);
	
	/* Wait till done */
	while (ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
	
	/* Get result */
	result = ADC_GetConversionValue(ADCx);
	
	/* Convert to voltage */
	result = result * ADC_VBAT_MULTI * ADC_SUPPLY_VOLTAGE / 0xFFF;
	
	/* Return value in mV */
	return (uint16_t) result;
}

/* Private functions */
void TM_ADC_INT_Channel_0_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_0);
}
void TM_ADC_INT_Channel_1_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_1);
}
void TM_ADC_INT_Channel_2_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_2);
}
void TM_ADC_INT_Channel_3_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_3);
}
void TM_ADC_INT_Channel_4_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_4);
	} else if (ADCx == ADC3) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_6);
	}
}
void TM_ADC_INT_Channel_5_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_5);
	} else if (ADCx == ADC3) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_7);
	}
}
void TM_ADC_INT_Channel_6_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_6);
	} else if (ADCx == ADC3) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_8);
	}
}
void TM_ADC_INT_Channel_7_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_7);
	} else if (ADCx == ADC3) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_9);
	}
}
void TM_ADC_INT_Channel_8_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_0);
	} else if (ADCx == ADC3) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_10);
	}
}
void TM_ADC_INT_Channel_9_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_1);
	} else if (ADCx == ADC3) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_3);
	}
}
void TM_ADC_INT_Channel_10_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_0);
}
void TM_ADC_INT_Channel_11_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_1);
}
void TM_ADC_INT_Channel_12_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_2);
}
void TM_ADC_INT_Channel_13_Init(ADC_TypeDef* ADCx) {
	TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_3);
}
void TM_ADC_INT_Channel_14_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_3);
	} else if (ADCx == ADC3) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_4);
	}
}
void TM_ADC_INT_Channel_15_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_5);
	} else if (ADCx == ADC3) {
		TM_ADC_INT_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_5);
	}
}

void TM_ADC_INT_InitPin(uint16_t RCCx, GPIO_TypeDef* GPIOx, uint16_t PinX) {
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCCx, ENABLE);

	/* Set GPIO settings */
	GPIO_InitStruct.GPIO_Pin = PinX;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	/* Initialize GPIO pin */
	GPIO_Init(GPIOx, &GPIO_InitStruct);
}
