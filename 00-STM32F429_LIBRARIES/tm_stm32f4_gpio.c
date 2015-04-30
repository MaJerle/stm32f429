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

/* Private functions */
void TM_GPIO_INT_EnableClock(GPIO_TypeDef* GPIOx);
void TM_GPIO_INT_DisableClock(GPIO_TypeDef* GPIOx);
void TM_GPIO_INT_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_GPIO_Mode_t GPIO_Mode, TM_GPIO_OType_t GPIO_OType, TM_GPIO_PuPd_t GPIO_PuPd, TM_GPIO_Speed_t GPIO_Speed);

void TM_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_GPIO_Mode_t GPIO_Mode, TM_GPIO_OType_t GPIO_OType, TM_GPIO_PuPd_t GPIO_PuPd, TM_GPIO_Speed_t GPIO_Speed) {	
	/* Check input */
	if (GPIO_Pin == 0x00) {
		return;
	}
	
	/* Enable clock for GPIO */
	TM_GPIO_INT_EnableClock(GPIOx);
	
	/* Do initialization */
	TM_GPIO_INT_Init(GPIOx, GPIO_Pin, GPIO_Mode, GPIO_OType, GPIO_PuPd, GPIO_Speed);
}

void TM_GPIO_InitAlternate(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_GPIO_OType_t GPIO_OType, TM_GPIO_PuPd_t GPIO_PuPd, TM_GPIO_Speed_t GPIO_Speed, uint8_t Alternate) {
	uint32_t pinpos;

	/* Check input */
	if (GPIO_Pin == 0x00) {
		return;
	}
	
	/* Enable GPIOx clock */
	TM_GPIO_INT_EnableClock(GPIOx);
	
	/* Set alternate functions for all pins */
	for (pinpos = 0; pinpos < 0x10; pinpos++) {
		/* Check pin */
		if ((GPIO_Pin & (1 << pinpos)) == 0) {
			continue;
		}
		
		/* Set alternate function */
		GPIOx->AFR[pinpos >> 0x03] = (GPIOx->AFR[pinpos >> 0x03] & ~(0x0F << (4 * (pinpos & 0x07)))) | (Alternate << (4 * (pinpos & 0x07)));
	}
	
	/* Do initialization */
	TM_GPIO_INT_Init(GPIOx, GPIO_Pin, TM_GPIO_Mode_AF, GPIO_OType, GPIO_PuPd, GPIO_Speed);
}

void TM_GPIO_DeInit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	uint8_t i;
	/* Go through all pins */
	for (i = 0x00; i < 0x10; i++) {
		/* Pin is set */
		if (GPIO_Pin & (1 << i)) {
			/* Set 11 bits combination for analog mode */
			GPIOx->MODER |= (0x03 << (2 * i));
		}
	}
}

void TM_GPIO_SetPinAsInput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	uint8_t i;
	/* Go through all pins */
	for (i = 0x00; i < 0x10; i++) {
		/* Pin is set */
		if (GPIO_Pin & (1 << i)) {
			/* Set 00 bits combination for input */
			GPIOx->MODER &= ~(0x03 << (2 * i));
		}
	}
}

void TM_GPIO_SetPinAsOutput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	uint8_t i;
	/* Go through all pins */
	for (i = 0x00; i < 0x10; i++) {
		/* Pin is set */
		if (GPIO_Pin & (1 << i)) {
			/* Set 01 bits combination for output */
			GPIOx->MODER = (GPIOx->MODER & ~(0x03 << (2 * i))) | (0x01 << (2 * i));
		}
	}
}

void TM_GPIO_SetPinAsAnalog(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	uint8_t i;
	/* Go through all pins */
	for (i = 0x00; i < 0x10; i++) {
		/* Pin is set */
		if (GPIO_Pin & (1 << i)) {
			/* Set 11 bits combination for analog mode */
			GPIOx->MODER |= (0x03 << (2 * i));
		}
	}
}

void TM_GPIO_SetPullResistor(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_GPIO_PuPd_t GPIO_PuPd) {
	uint8_t pinpos;
	
	/* Go through all pins */
	for (pinpos = 0; pinpos < 0x10; pinpos++) {
		/* Check if pin available */
		if ((GPIO_Pin & (1 << pinpos)) == 0) {
			continue;
		}

		/* Set GPIO PUPD register */
		GPIOx->PUPDR = (GPIOx->PUPDR & ~(0x03 << (2 * pinpos))) | ((uint32_t)(GPIO_PuPd << (2 * pinpos)));
	}
}

void TM_GPIO_Lock(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	uint32_t d;
	
	/* Set GPIO pin with 16th bit set to 1 */
	d = 0x00010000 | GPIO_Pin;
	
	/* Write to LCKR register */
	GPIOx->LCKR = d;
	GPIOx->LCKR = GPIO_Pin;
	GPIOx->LCKR = d;
	
	/* Read twice */
	(void)GPIOx->LCKR;
	(void)GPIOx->LCKR;
}

uint16_t TM_GPIO_GetPortSource(GPIO_TypeDef* GPIOx) {
	uint8_t portsource = 0;
#ifdef GPIOA
	if (GPIOx == GPIOA) {
		portsource = 0x00;
	}
#endif
#ifdef GPIOB
	if (GPIOx == GPIOB) {
		portsource = 0x01;
	}
#endif
#ifdef GPIOC
	if (GPIOx == GPIOC) {
		portsource = 0x02;
	}
#endif
#ifdef GPIOD
	if (GPIOx == GPIOD) {
		portsource = 0x03;
	}
#endif
#ifdef GPIOE
	if (GPIOx == GPIOE) {
		portsource = 0x04;
	}
#endif
#ifdef GPIOF
	if (GPIOx == GPIOF) {
		portsource = 0x05;
	}
#endif
#ifdef GPIOG
	if (GPIOx == GPIOG) {
		portsource = 0x06;
	}
#endif
#ifdef GPIOH
	if (GPIOx == GPIOH) {
		portsource = 0x07;
	}
#endif
#ifdef GPIOI
	if (GPIOx == GPIOI) {
		portsource = 0x08;
	}
#endif
#ifdef GPIOJ
	if (GPIOx == GPIOJ) {
		portsource = 0x09;
	}
#endif
#ifdef GPIOK
	if (GPIOx == GPIOK) {
		portsource = 0x0A;
	}
#endif
	
	/* Return portsource */
	return portsource;
}

uint16_t TM_GPIO_GetPinSource(uint16_t GPIO_Pin) {
	uint16_t pinsource = 0;
	
	/* Get pinsource */
	while (GPIO_Pin > 1) {
		/* Increase pinsource */
		pinsource++;
		/* Shift right */
		GPIO_Pin >>= 1;
	}
	
	/* Return source */
	return pinsource;
}

/* Private functions */
void TM_GPIO_INT_EnableClock(GPIO_TypeDef* GPIOx) {
#ifdef GPIOA
	if (GPIOx == GPIOA) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	}
#endif
#ifdef GPIOB
	if (GPIOx == GPIOB) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	}
#endif
#ifdef GPIOC
	if (GPIOx == GPIOC) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	}
#endif
#ifdef GPIOD
	if (GPIOx == GPIOD) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	}
#endif
#ifdef GPIOE
	if (GPIOx == GPIOE) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	}
#endif
#ifdef GPIOF
	if (GPIOx == GPIOF) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
	}
#endif
#ifdef GPIOG
	if (GPIOx == GPIOG) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	}
#endif
#ifdef GPIOH
	if (GPIOx == GPIOH) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
	}
#endif
#ifdef GPIOI
	if (GPIOx == GPIOI) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
	}
#endif
#ifdef GPIOJ
	if (GPIOx == GPIOJ) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;
	}
#endif
#ifdef GPIOK
	if (GPIOx == GPIOK) {
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOKEN;
	}
#endif
}

void TM_GPIO_INT_DisableClock(GPIO_TypeDef* GPIOx) {
#ifdef GPIOA
	if (GPIOx == GPIOA) {
		RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOAEN;
	}
#endif
#ifdef GPIOB
	if (GPIOx == GPIOB) {
		RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOBEN;
	}
#endif
#ifdef GPIOC
	if (GPIOx == GPIOC) {
		RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOCEN;
	}
#endif
#ifdef GPIOD
	if (GPIOx == GPIOD) {
		RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIODEN;
	}
#endif
#ifdef GPIOE
	if (GPIOx == GPIOE) {
		RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOEEN;
	}
#endif
#ifdef GPIOF
	if (GPIOx == GPIOF) {
		RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOFEN;
	}
#endif
#ifdef GPIOG
	if (GPIOx == GPIOG) {
		RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOGEN;
	}
#endif
#ifdef GPIOH
	if (GPIOx == GPIOH) {
		RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOHEN;
	}
#endif
#ifdef GPIOI
	if (GPIOx == GPIOI) {
		RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOIEN;
	}
#endif
#ifdef GPIOJ
	if (GPIOx == GPIOJ) {
		RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOJEN;
	}
#endif
#ifdef GPIOK
	if (GPIOx == GPIOK) {
		RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOKEN;
	}
#endif
}

void TM_GPIO_INT_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, TM_GPIO_Mode_t GPIO_Mode, TM_GPIO_OType_t GPIO_OType, TM_GPIO_PuPd_t GPIO_PuPd, TM_GPIO_Speed_t GPIO_Speed) {
	uint8_t pinpos;
	
	/* Go through all pins */
	for (pinpos = 0; pinpos < 0x10; pinpos++) {
		/* Check if pin available */
		if ((GPIO_Pin & (1 << pinpos)) == 0) {
			continue;
		}
		
		/* Set GPIO MODE register */
		GPIOx->MODER = (GPIOx->MODER & ~((uint32_t)(0x03 << (2 * pinpos)))) | ((uint32_t)(GPIO_Mode << (2 * pinpos)));
		
		/* Set only if output or alternate functions */
		if (GPIO_Mode == TM_GPIO_Mode_OUT || GPIO_Mode == TM_GPIO_Mode_AF) {		
			/* Set GPIO OTYPE register */
			GPIOx->OTYPER = (GPIOx->OTYPER & ~(uint16_t)(0x01 << pinpos)) | ((uint16_t)(GPIO_OType << pinpos));
			
			/* Set GPIO OSPEED register */
			GPIOx->OSPEEDR = (GPIOx->OSPEEDR & ~((uint32_t)(0x03 << (2 * pinpos)))) | ((uint32_t)(GPIO_Speed << (2 * pinpos)));
		}
		
		/* Set GPIO PUPD register */
		GPIOx->PUPDR = (GPIOx->PUPDR & ~(0x03 << (2 * pinpos))) | ((uint32_t)(GPIO_PuPd << (2 * pinpos)));
	}
}
