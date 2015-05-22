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

void TM_RCC_SetPLL(uint16_t PLLM, uint16_t PLLN, uint16_t PLLP, uint16_t PLLQ, uint16_t PLLR) {
	/*!< Select HSI clock as main clock */
	RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_SW)) | RCC_CFGR_SW_HSI;
	
	/*!< Disable PLL */
	RCC->CR &= ~RCC_CR_PLLON;
	
	/* Set PLL clocks */
	TM_RCC_SetPLL_M(PLLM);
	TM_RCC_SetPLL_N(PLLN);
	TM_RCC_SetPLL_P(PLLP);
	TM_RCC_SetPLL_Q(PLLQ);
	TM_RCC_SetPLL_R(PLLR);
	
	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;
	
	/* Wait till PLL is ready */
	while (!TM_RCC_IsPLLReady());
	
	/* Enable PLL as main clock */
	RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_SW)) | RCC_CFGR_SW_PLL;
	
	/* Update system core clock variable */
	SystemCoreClockUpdate();
}

void TM_RCC_SetPLL_M(uint16_t PLLM) {
	/* Set PLLCFGR register */
	if (PLLM >= 2 && PLLM <= 63) {
		RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLM_MASK) | (PLLM << RCC_PLLM_POS);
	}
}

void TM_RCC_SetPLL_N(uint16_t PLLN) {
	/* Set PLLCFGR register */
	if (PLLN >= 192 && PLLN <= 432) {
		RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLN_MASK) | (PLLN << RCC_PLLN_POS);
	}
}

void TM_RCC_SetPLL_P(uint16_t PLLP) {
	/* Set PLLCFGR register */
	if (PLLP == 2 || PLLP == 4 || PLLP == 6 || PLLP == 8) {
		RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLP_MASK) | (((PLLP >> 1) - 1) << RCC_PLLP_POS);
	}
}

void TM_RCC_SetPLL_Q(uint16_t PLLQ) {
	/* Set PLLCFGR register */
	if (PLLQ >= 2 && PLLQ <= 15) {
		RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLQ_MASK) | (PLLQ << RCC_PLLQ_POS);
	}
}

void TM_RCC_SetPLL_R(uint16_t PLLR) {
	/* Set PLLCFGR register */
	if (PLLR >= 2 && PLLR <= 7) {
		RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLR_MASK) | (PLLR << RCC_PLLR_POS);
	}
}

uint8_t TM_RCC_IsPLLReady(void) {
	/* Return PLL ready status */
	return (RCC->CR & RCC_CR_PLLRDY);
}
