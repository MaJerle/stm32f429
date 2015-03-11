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
#include "tm_stm32f4_exti.h"

/* Used pins variable */
static volatile uint32_t pinsused = 0;

TM_EXTI_Result_t TM_EXTI_Attach(GPIO_TypeDef* GPIOx, uint16_t GPIO_Line, TM_EXTI_Trigger_t trigger) {
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	TM_GPIO_PuPd_t PuPd;
	uint8_t pinsource, portsource, irqchannel;
	
	/* Check if line is already in use */
	if (pinsused & GPIO_Line) {
		/* Return error */
		return TM_EXTI_Result_Error;
	}
	
	/* Get IRQ channel */
	switch (GPIO_Line) {
		case GPIO_PIN_0:
			irqchannel = EXTI0_IRQn;
			break;
		case GPIO_PIN_1:
			irqchannel = EXTI1_IRQn;
			break;
		case GPIO_PIN_2:
			irqchannel = EXTI2_IRQn;
			break;
		case GPIO_PIN_3:
			irqchannel = EXTI3_IRQn;
			break;
		case GPIO_Pin_4:
			irqchannel = EXTI4_IRQn;
			break;
		case GPIO_PIN_5:
		case GPIO_PIN_6:
		case GPIO_PIN_7:
		case GPIO_PIN_8:
		case GPIO_PIN_9:
			irqchannel = EXTI9_5_IRQn;
			break;
		case GPIO_PIN_10:
		case GPIO_PIN_11:
		case GPIO_PIN_12:
		case GPIO_PIN_13:
		case GPIO_PIN_14:
		case GPIO_PIN_15:
			irqchannel = EXTI15_10_IRQn;
			break;
		default:
			return TM_EXTI_Result_Error;
	}

	/* Check pull settings */
	if (trigger == TM_EXTI_Trigger_Falling) {
		PuPd = TM_GPIO_PuPd_UP;
	} else if (trigger == TM_EXTI_Trigger_Rising) {
		PuPd = TM_GPIO_PuPd_DOWN;
	} else {
		PuPd = TM_GPIO_PuPd_NOPULL;
	}
	
	/* Init GPIO pin */
	TM_GPIO_Init(GPIOx, GPIO_Line, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, PuPd, TM_GPIO_Speed_Low);
	
	/* Calculate pinsource */
	pinsource = TM_GPIO_GetPinSource(GPIO_Line);
	
	/* Calculate portsource */
	portsource = TM_GPIO_GetPortSource(GPIOx);
	
	/* Enable SYSCFG clock */
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	/* Connect proper GPIO */
	SYSCFG_EXTILineConfig(portsource, pinsource);
	
	/* Enable EXTI */
	EXTI_InitStruct.EXTI_Line = GPIO_Line;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = (EXTITrigger_TypeDef) trigger;
	EXTI_Init(&EXTI_InitStruct);

	/* Add to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = irqchannel;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = TM_EXTI_PRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = pinsource;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	/* Add to used pins */
	pinsused |= GPIO_Line;

	/* Return OK */
	return TM_EXTI_Result_Ok;
}

TM_EXTI_Result_t TM_EXTI_Detach(uint16_t GPIO_Line) {
	EXTI_InitTypeDef EXTI_InitStruct;
	
	/* Check if pin is used */
	if (!(pinsused & GPIO_Line)) {
		/* Return Error */
	}
	
	/* Disable EXTI */
	EXTI_InitStruct.EXTI_Line = GPIO_Line;
	EXTI_InitStruct.EXTI_LineCmd = DISABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	/* Clear from used pins */
	pinsused &= ~GPIO_Line;
	
	/* Return OK */
	return TM_EXTI_Result_Ok;
}

#ifndef TM_EXTI_DISABLE_DEFAULT_HANDLER_0
void EXTI0_IRQHandler(void) {
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR0)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR0;
		/* Call user function */
		TM_EXTI_Handler_0();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_0);
	}
}
#endif

#ifndef TM_EXTI_DISABLE_DEFAULT_HANDLER_1
void EXTI1_IRQHandler(void) {
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR1)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR1;
		/* Call user function */
		TM_EXTI_Handler_1();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_1);
	}
}
#endif

#ifndef TM_EXTI_DISABLE_DEFAULT_HANDLER_2
void EXTI2_IRQHandler(void) {
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR2)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR2;
		/* Call user function */
		TM_EXTI_Handler_2();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_2);
	}
}
#endif

#ifndef TM_EXTI_DISABLE_DEFAULT_HANDLER_3
void EXTI3_IRQHandler(void) {
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR3)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR3;
		/* Call user function */
		TM_EXTI_Handler_3();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_3);
	}
}
#endif

#ifndef TM_EXTI_DISABLE_DEFAULT_HANDLER_4
void EXTI4_IRQHandler(void) {
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR4)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR4;
		/* Call user function */
		TM_EXTI_Handler_4();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_4);
	}
}
#endif

#ifndef TM_EXTI_DISABLE_DEFAULT_HANDLER_9_5
void EXTI9_5_IRQHandler(void) {
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR5)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR5;
		/* Call user function */
		TM_EXTI_Handler_5();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_5);
	}
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR6)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR6;
		/* Call user function */
		TM_EXTI_Handler_6();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_6);
	}
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR7)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR7;
		/* Call user function */
		TM_EXTI_Handler_7();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_7);
	}
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR8)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR8;
		/* Call user function */
		TM_EXTI_Handler_8();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_8);
	}
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR9)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR9;
		/* Call user function */
		TM_EXTI_Handler_9();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_9);
	}
}
#endif

#ifndef TM_EXTI_DISABLE_DEFAULT_HANDLER_15_10
void EXTI15_10_IRQHandler(void) {
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR10)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR10;
		/* Call user function */
		TM_EXTI_Handler_10();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_10);
	}
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR11)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR11;
		/* Call user function */
		TM_EXTI_Handler_11();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_11);
	}
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR12)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR12;
		/* Call user function */
		TM_EXTI_Handler_12();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_12);
	}
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR13)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR13;
		/* Call user function */
		TM_EXTI_Handler_13();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_13);
	}
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR14)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR14;
		/* Call user function */
		TM_EXTI_Handler_14();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_14);
	}
	/* Check status */
	if (EXTI->PR & (EXTI_PR_PR15)) {
		/* Clear bit */
		EXTI->PR = EXTI_PR_PR15;
		/* Call user function */
		TM_EXTI_Handler_15();
		/* Call global function */
		TM_EXTI_Handler(GPIO_PIN_15);
	}
}
#endif
