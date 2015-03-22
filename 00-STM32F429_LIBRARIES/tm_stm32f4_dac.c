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
#include "tm_stm32f4_dac.h"

void TM_DAC_Init(TM_DAC_Channel_t DACx) {
	DAC_InitTypeDef DAC_InitStruct;
	uint16_t GPIO_Pin;
	
	/* Select proper GPIO pin */
	if (DACx == TM_DAC1) {
		GPIO_Pin = GPIO_PIN_4;
	} else {
		GPIO_Pin = GPIO_PIN_5;
	}
	
	/* Initialize proper GPIO pin */
	TM_GPIO_Init(GPIOA, GPIO_Pin, TM_GPIO_Mode_AN, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Fast);

	/* Enable DAC clock */
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	
	/* Set DAC options */
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	
	/* Init and enable proper DAC */
	if (DACx == TM_DAC1) {
		DAC_Init(DAC_Channel_1, &DAC_InitStruct);
		/* Enable DAC channel 1 */
		DAC->CR |= DAC_CR_EN1;
	} else {
		DAC_Init(DAC_Channel_2, &DAC_InitStruct);
		/* Enable DAC channel 2 */
		DAC->CR |= DAC_CR_EN2;
	}
}

void TM_DAC_SetValue(TM_DAC_Channel_t DACx, uint16_t value) {
	/* Check value */
	if (value > 4095) {
		value = 4095;
	}
	
	/* Set 12-bit value, right aligned */
	if (DACx == TM_DAC1) {
		DAC->DHR12R1 = value;
	} else {
		DAC->DHR12R2 = value;
	}
}
