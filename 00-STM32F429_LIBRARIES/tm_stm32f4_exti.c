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

uint32_t pinsused = 0;

TM_EXTI_Result_t TM_EXTI_Attach(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_EXTI_Trigger_t trigger) {
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	uint32_t gpio_clock;
	uint8_t pinsource, portsource, irqchannel;
	
	/* Check if line is already in use */
	if (pinsused & GPIO_Pin) {
		/* Return error */
		return TM_EXTI_Result_Error;
	}
	
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* Get proper settings */
	if (GPIOx == GPIOA) {
		gpio_clock = RCC_AHB1Periph_GPIOA;
		portsource = EXTI_PortSourceGPIOA;
	} else if (GPIOx == GPIOB) {
		gpio_clock = RCC_AHB1Periph_GPIOB;
		portsource = EXTI_PortSourceGPIOB;
	} else if (GPIOx == GPIOC) {
		gpio_clock = RCC_AHB1Periph_GPIOC;
		portsource = EXTI_PortSourceGPIOC;
	} else if (GPIOx == GPIOD) {
		gpio_clock = RCC_AHB1Periph_GPIOD;
		portsource = EXTI_PortSourceGPIOD;
	} else if (GPIOx == GPIOE) {
		gpio_clock = RCC_AHB1Periph_GPIOE;
		portsource = EXTI_PortSourceGPIOE;
	} else if (GPIOx == GPIOF) {
		gpio_clock = RCC_AHB1Periph_GPIOF;
		portsource = EXTI_PortSourceGPIOF;
	} else if (GPIOx == GPIOG) {
		gpio_clock = RCC_AHB1Periph_GPIOG;
		portsource = EXTI_PortSourceGPIOG;
	} else if (GPIOx == GPIOH) {
		gpio_clock = RCC_AHB1Periph_GPIOH;
		portsource = EXTI_PortSourceGPIOH;
	} else if (GPIOx == GPIOI) {
		gpio_clock = RCC_AHB1Periph_GPIOI;
		portsource = EXTI_PortSourceGPIOI;
	} else if (GPIOx == GPIOJ) {
		gpio_clock = RCC_AHB1Periph_GPIOJ;
		portsource = EXTI_PortSourceGPIOJ;
	} else if (GPIOx == GPIOK) {
		gpio_clock = RCC_AHB1Periph_GPIOK;
		portsource = EXTI_PortSourceGPIOK;
	} else {
		/* Return error */
		return TM_EXTI_Result_Error;
	}
	
	switch (GPIO_Pin) {
		case GPIO_Pin_0:
			pinsource = EXTI_PinSource0;
			irqchannel = EXTI0_IRQn;
			break;
		case GPIO_Pin_1:
			pinsource = EXTI_PinSource1;
			irqchannel = EXTI1_IRQn;
			break;
		case GPIO_Pin_2:
			pinsource = EXTI_PinSource2;
			irqchannel = EXTI2_IRQn;
			break;
		case GPIO_Pin_3:
			pinsource = EXTI_PinSource3;
			irqchannel = EXTI3_IRQn;
			break;
		case GPIO_Pin_4:
			pinsource = EXTI_PinSource4;
			irqchannel = EXTI4_IRQn;
			break;
		case GPIO_Pin_5:
			pinsource = EXTI_PinSource5;
			irqchannel = EXTI9_5_IRQn;
			break;
		case GPIO_Pin_6:
			pinsource = EXTI_PinSource6;
			irqchannel = EXTI9_5_IRQn;
			break;
		case GPIO_Pin_7:
			pinsource = EXTI_PinSource7;
			irqchannel = EXTI9_5_IRQn;
			break;
		case GPIO_Pin_8:
			pinsource = EXTI_PinSource8;
			irqchannel = EXTI9_5_IRQn;
			break;
		case GPIO_Pin_9:
			pinsource = EXTI_PinSource9;
			irqchannel = EXTI9_5_IRQn;
			break;
		case GPIO_Pin_10:
			pinsource = EXTI_PinSource10;
			irqchannel = EXTI15_10_IRQn;
			break;
		case GPIO_Pin_11:
			pinsource = EXTI_PinSource11;
			irqchannel = EXTI15_10_IRQn;
			break;
		case GPIO_Pin_12:
			pinsource = EXTI_PinSource12;
			irqchannel = EXTI15_10_IRQn;
			break;
		case GPIO_Pin_13:
			pinsource = EXTI_PinSource13;
			irqchannel = EXTI15_10_IRQn;
			break;
		case GPIO_Pin_14:
			pinsource = EXTI_PinSource14;
			irqchannel = EXTI15_10_IRQn;
			break;
		case GPIO_Pin_15:
			pinsource = EXTI_PinSource15;
			irqchannel = EXTI15_10_IRQn;
			break;
		default:
			return TM_EXTI_Result_Error;
	}
	
	/* Enable clock for GPIO */
	RCC_AHB1PeriphClockCmd(gpio_clock, ENABLE);
	
	/* Set pin as input */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	if (trigger == TM_EXTI_Trigger_Falling) {
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	} else if (trigger == TM_EXTI_Trigger_Rising) {
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	} else {
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	}
	
	/* Initialize pin */
	GPIO_Init(GPIOx, &GPIO_InitStruct);
	
	/* Connect proper GPIO */
	SYSCFG_EXTILineConfig(portsource, pinsource);
	
	/* Enable EXTI */
	EXTI_InitStruct.EXTI_Line = GPIO_Pin;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = (EXTITrigger_TypeDef)trigger;
	EXTI_Init(&EXTI_InitStruct);

	/* Add to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = irqchannel;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = TM_EXTI_PRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = pinsource;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	/* Add to used pins */
	pinsused |= GPIO_Pin;

	/* Return OK */
	return TM_EXTI_Result_Ok;
}

TM_EXTI_Result_t TM_EXTI_Detach(uint16_t GPIO_Pin) {
	EXTI_InitTypeDef EXTI_InitStruct;
	
	/* Check if pin is used */
	if (!(pinsused & GPIO_Pin)) {
		/* Return Error */
	}
	
	/* Disable EXTI */
	EXTI_InitStruct.EXTI_Line = GPIO_Pin;
	EXTI_InitStruct.EXTI_LineCmd = DISABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	/* Clear from used pins */
	pinsused &= ~GPIO_Pin;
	
	/* Return OK */
	return TM_EXTI_Result_Ok;
}

#ifndef TM_EXTI_DISABLE_DEFAULT_HANDLER_0
void EXTI0_IRQHandler(void) {
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line0);
		/* Call user function */
		TM_EXTI_Handler_0();
	}
}
#endif

#ifndef TM_EXTI_DISABLE_DEFAULT_HANDLER_1
void EXTI1_IRQHandler(void) {
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line1);
		/* Call user function */
		TM_EXTI_Handler_1();
	}
}
#endif

#ifndef TM_EXTI_DISABLE_DEFAULT_HANDLER_2
void EXTI2_IRQHandler(void) {
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line2);
		/* Call user function */
		TM_EXTI_Handler_2();
	}
}
#endif

#ifndef TM_EXTI_DISABLE_DEFAULT_HANDLER_3
void EXTI3_IRQHandler(void) {
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line3);
		/* Call user function */
		TM_EXTI_Handler_3();
	}
}
#endif

#ifndef TM_EXTI_DISABLE_DEFAULT_HANDLER_4
void EXTI4_IRQHandler(void) {
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line4);
		/* Call user function */
		TM_EXTI_Handler_4();
	}
}
#endif

#ifndef TM_EXTI_DISABLE_DEFAULT_HANDLER_9_5
void EXTI9_5_IRQHandler(void) {
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line5) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line5);
		/* Call user function */
		TM_EXTI_Handler_5();
	}
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line6) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line6);
		/* Call user function */
		TM_EXTI_Handler_6();
	}
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line7) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line7);
		/* Call user function */
		TM_EXTI_Handler_7();
	}
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line8) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line8);
		/* Call user function */
		TM_EXTI_Handler_8();
	}
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line9) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line9);
		/* Call user function */
		TM_EXTI_Handler_9();
	}
}
#endif

#ifndef TM_EXTI_DISABLE_DEFAULT_HANDLER_15_10
void EXTI15_10_IRQHandler(void) {
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line10) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line10);
		/* Call user function */
		TM_EXTI_Handler_10();
	}
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line11) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line11);
		/* Call user function */
		TM_EXTI_Handler_11();
	}
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line12) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line12);
		/* Call user function */
		TM_EXTI_Handler_12();
	}
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line13) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line13);
		/* Call user function */
		TM_EXTI_Handler_13();
	}
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line14) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line14);
		/* Call user function */
		TM_EXTI_Handler_14();
	}
	/* Check status */
	if (EXTI_GetITStatus(EXTI_Line15) != RESET) {
		/* Clear bit */
		EXTI_ClearITPendingBit(EXTI_Line15);
		/* Call user function */
		TM_EXTI_Handler_15();
	}
}
#endif
