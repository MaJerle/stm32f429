/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2015
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
#include "tm_stm32f4_gpio.h"

/* Private */
static GPIO_InitTypeDef GPIO_InitStruct;

/* Private functions */
void TM_GPIO_INT_EnableClock(GPIO_TypeDef* GPIOx);
void TM_GPIO_INT_DisableClock(GPIO_TypeDef* GPIOx);

void TM_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_GPIO_Mode_t GPIO_Mode, TM_GPIO_OType_t GPIO_OType, TM_GPIO_PuPd_t GPIO_PuPd, TM_GPIO_Speed_t GPIO_Speed) {	
	/* Check input */
	if (GPIO_Pin == 0x00) {
		return;
	}
	
	/* Enable clock for GPIO */
	TM_GPIO_INT_EnableClock(GPIOx);
	
	/* Fill settings */
	GPIO_InitStruct.GPIO_Mode = (GPIOMode_TypeDef) GPIO_Mode;
	GPIO_InitStruct.GPIO_OType = (GPIOOType_TypeDef) GPIO_OType;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin;
	GPIO_InitStruct.GPIO_PuPd = (GPIOPuPd_TypeDef) GPIO_PuPd;
	GPIO_InitStruct.GPIO_Speed = (GPIOSpeed_TypeDef) GPIO_Speed;
	
	/* Init */
	GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void TM_GPIO_InitAlternate(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_GPIO_OType_t GPIO_OType, TM_GPIO_PuPd_t GPIO_PuPd, TM_GPIO_Speed_t GPIO_Speed, uint8_t Alternate) {
	uint32_t pinsource = 0, pinpos, pin;

	/* Check input */
	if (GPIO_Pin == 0x00) {
		return;
	}
	
	/* Init pin as alternate function */
	TM_GPIO_Init(GPIOx, GPIO_Pin, TM_GPIO_Mode_AF, GPIO_OType, GPIO_PuPd, GPIO_Speed);
	
	/* Set alternate functions for all pins */
	for (pinpos = 0; pinpos < 0x10; pinpos++) {
		/* Check pin */
		if ((GPIO_Pin & (1 << pinpos)) == 0) {
			continue;
		}
		
		/* Calculate pinsource from GPIO pin */
		pinsource = 0;
		for (pin = 1 << pinpos; pin > 1; pin >>= 1) {
			pinsource++;
		}
		
		/* Set alternate function */
		GPIO_PinAFConfig(GPIOx, pinsource, Alternate);
	}
}

/* Private functions */
void TM_GPIO_INT_EnableClock(GPIO_TypeDef* GPIOx) {
	if (GPIOx == GPIOA) {
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOA;
	} else if (GPIOx == GPIOB) {
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOB;
	} else if (GPIOx == GPIOC) {
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOC;
	} else if (GPIOx == GPIOD) {
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOD;
	} else if (GPIOx == GPIOE) {
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOE;
	} else if (GPIOx == GPIOF) {
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOF;
	} else if (GPIOx == GPIOG) {
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOG;
	} else if (GPIOx == GPIOH) {
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOH;
	} else if (GPIOx == GPIOI) {
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOI;
	} else if (GPIOx == GPIOJ) {
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOJ;
	} else if (GPIOx == GPIOK) {
		RCC->AHB1ENR |= RCC_AHB1Periph_GPIOK;
	}
}

void TM_GPIO_INT_DisableClock(GPIO_TypeDef* GPIOx) {
	if (GPIOx == GPIOA) {
		RCC->AHB1ENR &= ~RCC_AHB1Periph_GPIOA;
	} else if (GPIOx == GPIOB) {
		RCC->AHB1ENR &= ~RCC_AHB1Periph_GPIOB;
	} else if (GPIOx == GPIOC) {
		RCC->AHB1ENR &= ~RCC_AHB1Periph_GPIOC;
	} else if (GPIOx == GPIOD) {
		RCC->AHB1ENR &= ~RCC_AHB1Periph_GPIOD;
	} else if (GPIOx == GPIOE) {
		RCC->AHB1ENR &= ~RCC_AHB1Periph_GPIOE;
	} else if (GPIOx == GPIOF) {
		RCC->AHB1ENR &= ~RCC_AHB1Periph_GPIOF;
	} else if (GPIOx == GPIOG) {
		RCC->AHB1ENR &= ~RCC_AHB1Periph_GPIOG;
	} else if (GPIOx == GPIOH) {
		RCC->AHB1ENR &= ~RCC_AHB1Periph_GPIOH;
	} else if (GPIOx == GPIOI) {
		RCC->AHB1ENR &= ~RCC_AHB1Periph_GPIOI;
	} else if (GPIOx == GPIOJ) {
		RCC->AHB1ENR &= ~RCC_AHB1Periph_GPIOJ;
	} else if (GPIOx == GPIOK) {
		RCC->AHB1ENR &= ~RCC_AHB1Periph_GPIOK;
	}
}
