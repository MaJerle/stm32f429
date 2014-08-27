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
#include "tm_stm32f4_rotary_encoder.h"

int32_t TM_INT_RE_Count;

void TM_RE_Init(TM_RE_t* data) {
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	/* Pins as inputs */
	RCC_AHB1PeriphClockCmd(TM_RE_A_RCC | TM_RE_B_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* Pin common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	/* A pin */
	GPIO_InitStruct.GPIO_Pin = TM_RE_A_PIN;
	GPIO_Init(TM_RE_A_PORT, &GPIO_InitStruct);
	/* B pin */
	GPIO_InitStruct.GPIO_Pin = TM_RE_B_PIN;
	GPIO_Init(TM_RE_B_PORT, &GPIO_InitStruct);
	
	/* Connect pin to interrupt */
	SYSCFG_EXTILineConfig(TM_RE_A_EXTIPORTSOURCE, TM_RE_A_EXTIPINSOURCE);
	
	/* Configure external interrupt */
	EXTI_InitStruct.EXTI_Line = TM_RE_A_EXTI_LINE;	
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);

	/* Add interrupt to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = TM_RE_A_NVIC;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = RE_NVIC_PRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = RE_NVIC_SUBPRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/* Set default */
	TM_INT_RE_Count = 0;
	data->Diff = 0;
	data->Absolute = 0;
}

TM_RE_Rotate_t TM_RE_Get(TM_RE_t* data) {
	data->Diff = TM_INT_RE_Count - data->Absolute;
	data->Absolute = TM_INT_RE_Count;
	
	if (TM_INT_RE_Count < 0) {
		data->Rotation = TM_RE_Rotate_Decrement;
		return TM_RE_Rotate_Decrement;
	} else if (TM_INT_RE_Count > 0) {
		data->Rotation = TM_RE_Rotate_Increment;
		return TM_RE_Rotate_Increment;
	}
	data->Rotation = TM_RE_Rotate_Nothing;
	return TM_RE_Rotate_Nothing;
}

void TM_RE_Process(void) {
	static uint8_t last_a = 1;
	uint8_t now_a;
	uint8_t now_b;
	
	now_a = GPIO_ReadInputDataBit(TM_RE_A_PORT, TM_RE_A_PIN);
	now_b = GPIO_ReadInputDataBit(TM_RE_B_PORT, TM_RE_B_PIN);
	if (now_a != last_a) {
		last_a = now_a;
		
		if (last_a == 0) {
#ifdef TM_RE_CHANGE_ROTATION
			if (now_b == 1) {
				TM_INT_RE_Count--;
			} else {
				TM_INT_RE_Count++;
			}
#else
			if (now_b == 1) {
				TM_INT_RE_Count++;
			} else {
				TM_INT_RE_Count--;
			}
#endif
		}
	}
}

#ifdef TM_RE_A_DEFAULT
/* Default configuration is in use */
void EXTI0_IRQHandler(void) {
	if (EXTI_GetITStatus(TM_RE_A_EXTI_LINE) != RESET) {
		/* Process rotary encoder */
		TM_RE_Process();
		
		/* Clear interrupt bit */
		EXTI_ClearITPendingBit(TM_RE_A_EXTI_LINE);
	}
}
#endif

