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
#include "tm_stm32f4_rcc.h"

void TM_RCC_SetPLL(TM_RCC_PLL_t* PLL_Settings) {
	uint16_t timeout;
	TM_RCC_PLL_t tmp;
	
	/* Read PLL settings */
	TM_RCC_GetPLL(&tmp);
	
	/* Check if structures are equal */
	if (memcmp(PLL_Settings, &tmp, sizeof(TM_RCC_PLL_t)) == 0) {
		/* Don't change PLL settings if settings are the same */
		return;
	}
	
	/* Enable HSI clock */
	RCC->CR |= RCC_CR_HSION;
	
	/* Wait till HSI is ready */
	timeout = 0xFFFF;
	while (!(RCC->CR & RCC_CR_HSIRDY) && timeout--);
	
	/* Select HSI clock as main clock */
	RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_SW)) | RCC_CFGR_SW_HSI;
	
	/* Disable PLL */
	RCC->CR &= ~RCC_CR_PLLON;
	
	/* Set PLL settings */
	if (PLL_Settings->PLLM) {
		RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLM_MASK) | ((PLL_Settings->PLLM << RCC_PLLM_POS) & RCC_PLLM_MASK);
	}
	if (PLL_Settings->PLLN) {
		RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLN_MASK) | ((PLL_Settings->PLLN << RCC_PLLN_POS) & RCC_PLLN_MASK);
	}
	if (PLL_Settings->PLLP) {
		RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLP_MASK) | ((((PLL_Settings->PLLP >> 1) - 1) << RCC_PLLP_POS) & RCC_PLLP_MASK);
	}
	if (PLL_Settings->PLLQ) {
		RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLQ_MASK) | ((PLL_Settings->PLLQ << RCC_PLLQ_POS) & RCC_PLLQ_MASK);
	}
	if (PLL_Settings->PLLR) {
		RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLR_MASK) | ((PLL_Settings->PLLR << RCC_PLLR_POS) & RCC_PLLR_MASK);
	}
	
	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;
	
	/* Wait till PLL is ready */
	timeout = 0xFFFF;
	while (!TM_RCC_IsPLLReady() && timeout--);
	
	/* Enable PLL as main clock */
	RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_SW)) | RCC_CFGR_SW_PLL;
	
	/* Update system core clock variable */
	SystemCoreClockUpdate();
}

void TM_RCC_GetPLL(TM_RCC_PLL_t* PLL_Settings) {
	/* Read all PLL settings */
	PLL_Settings->PLLM = (RCC->PLLCFGR & RCC_PLLM_MASK) >> RCC_PLLM_POS;
	PLL_Settings->PLLN = (RCC->PLLCFGR & RCC_PLLN_MASK) >> RCC_PLLN_POS;
	PLL_Settings->PLLP = (((RCC->PLLCFGR & RCC_PLLP_MASK) >> RCC_PLLP_POS) + 1) << 1;
	PLL_Settings->PLLQ = (RCC->PLLCFGR & RCC_PLLQ_MASK) >> RCC_PLLQ_POS;
	PLL_Settings->PLLR = (RCC->PLLCFGR & RCC_PLLR_MASK) >> RCC_PLLR_POS;
}

uint8_t TM_RCC_IsPLLReady(void) {
	/* Return PLL ready status */
	return (RCC->CR & RCC_CR_PLLRDY);
}
