#include "tm_stm32f4_adc.h"

void TM_ADC_Init(ADC_TypeDef* ADCx, uint8_t channel) {
	ADC_InitTypeDef ADC_InitDef;
	ADC_CommonInitTypeDef ADC_CommonInitDef;

	if (channel == ADC_Channel_0) {
		TM_ADC_Channel_0_Init(ADCx);
	} else if (channel == ADC_Channel_1) {
		TM_ADC_Channel_1_Init(ADCx);
	} else if (channel == ADC_Channel_2) {
		TM_ADC_Channel_2_Init(ADCx);
	} else if (channel == ADC_Channel_3) {
		TM_ADC_Channel_3_Init(ADCx);
	} else if (channel == ADC_Channel_4) {
		TM_ADC_Channel_4_Init(ADCx);
	} else if (channel == ADC_Channel_5) {
		TM_ADC_Channel_5_Init(ADCx);
	} else if (channel == ADC_Channel_6) {
		TM_ADC_Channel_6_Init(ADCx);
	} else if (channel == ADC_Channel_7) {
		TM_ADC_Channel_7_Init(ADCx);
	} else if (channel == ADC_Channel_8) {
		TM_ADC_Channel_8_Init(ADCx);
	} else if (channel == ADC_Channel_9) {
		TM_ADC_Channel_9_Init(ADCx);
	} else if (channel == ADC_Channel_10) {
		TM_ADC_Channel_10_Init(ADCx);
	} else if (channel == ADC_Channel_11) {
		TM_ADC_Channel_11_Init(ADCx);
	} else if (channel == ADC_Channel_12) {
		TM_ADC_Channel_12_Init(ADCx);
	} else if (channel == ADC_Channel_13) {
		TM_ADC_Channel_13_Init(ADCx);
	} else if (channel == ADC_Channel_14) {
		TM_ADC_Channel_14_Init(ADCx);
	} else if (channel == ADC_Channel_15) {
		TM_ADC_Channel_15_Init(ADCx);
	}

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
	
	ADC_Init(ADCx, &ADC_InitDef);

	ADC_Cmd(ADCx, ENABLE);
}

uint16_t TM_ADC_Read(ADC_TypeDef* ADCx, uint8_t channel) {
	ADC_RegularChannelConfig(ADCx, channel, 1, ADC_SampleTime_15Cycles);
	ADC_SoftwareStartConv(ADCx);
	while (ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);

	return ADC_GetConversionValue(ADCx);
}

void TM_ADC_Channel_0_Init(ADC_TypeDef* ADCx) {
	TM_ADC_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_0);
}
void TM_ADC_Channel_1_Init(ADC_TypeDef* ADCx) {
	TM_ADC_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_1);
}
void TM_ADC_Channel_2_Init(ADC_TypeDef* ADCx) {
	TM_ADC_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_2);
}
void TM_ADC_Channel_3_Init(ADC_TypeDef* ADCx) {
	TM_ADC_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_3);
}
void TM_ADC_Channel_4_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_4);
	} else if (ADCx == ADC3) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_6);
	}
}
void TM_ADC_Channel_5_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_5);
	} else if (ADCx == ADC3) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_7);
	}
}
void TM_ADC_Channel_6_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_6);
	} else if (ADCx == ADC3) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_8);
	}
}
void TM_ADC_Channel_7_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_7);
	} else if (ADCx == ADC3) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_9);
	}
}
void TM_ADC_Channel_8_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_0);
	} else if (ADCx == ADC3) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_10);
	}
}
void TM_ADC_Channel_9_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_1);
	} else if (ADCx == ADC3) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_3);
	}
}
void TM_ADC_Channel_10_Init(ADC_TypeDef* ADCx) {
	TM_ADC_InitPin(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_0);
}
void TM_ADC_Channel_11_Init(ADC_TypeDef* ADCx) {
	TM_ADC_InitPin(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_1);
}
void TM_ADC_Channel_12_Init(ADC_TypeDef* ADCx) {
	TM_ADC_InitPin(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_2);
}
void TM_ADC_Channel_13_Init(ADC_TypeDef* ADCx) {
	TM_ADC_InitPin(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_3);
}
void TM_ADC_Channel_14_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_3);
	} else if (ADCx == ADC3) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_4);
	}
}
void TM_ADC_Channel_15_Init(ADC_TypeDef* ADCx) {
	if (ADCx == ADC1 || ADCx == ADC2) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_5);
	} else if (ADCx == ADC3) {
		TM_ADC_InitPin(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_5);
	}
}

void TM_ADC_InitPin(uint16_t RCCx, GPIO_TypeDef* GPIOx, uint16_t PinX) {
	GPIO_InitTypeDef GPIO_InitDef;

	RCC_AHB1PeriphClockCmd(RCCx, ENABLE);

	GPIO_InitDef.GPIO_Pin = PinX;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AN;
	//GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOx, &GPIO_InitDef);
}
