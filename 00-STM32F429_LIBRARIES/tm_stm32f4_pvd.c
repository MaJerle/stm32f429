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
#include "tm_stm32f4_pvd.h"

/* Bit for PVD in EXTI registers */
#define PVD_EXTI_LINE         0x00010000

void TM_PVD_Enable(TM_PVD_Level_t Level, TM_PVD_Trigger_t Trigger) {
	NVIC_InitTypeDef NVIC_InitStruct;
	
	/* Enable PWR clock */
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	
	/* Select EXTI values */
	if (Trigger == TM_PVD_Trigger_Rising) {
		EXTI->RTSR |= PVD_EXTI_LINE;
	} else if (Trigger == TM_PVD_Trigger_Falling) {
		EXTI->FTSR |= PVD_EXTI_LINE;
	} else {
		EXTI->RTSR |= PVD_EXTI_LINE;
		EXTI->FTSR |= PVD_EXTI_LINE;
	}
	
	/* Clear interrupt */
	EXTI->PR = PVD_EXTI_LINE;
	
	/* Enable EXTI interrupt */
	EXTI->IMR |= PVD_EXTI_LINE;
	
	/* Set NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = PVD_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = PVD_NVIC_PRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = PVD_NVIC_SUBPRIORITY;
	NVIC_Init(&NVIC_InitStruct);
	
	/* Select PVD trigger */
	PWR->CR = (PWR->CR & 0xE0) | (((uint8_t) Level) << 5);
	
	/* Enable PVD */
	PWR->CR |= PWR_CR_PVDE;
}

void TM_PVD_Disable(void) {
	/* Disable PVD */
	PWR->CR &= ~PWR_CR_PVDE;
	
	/* Clear EXTI */
	EXTI->IMR &= ~PVD_EXTI_LINE;
	
	/* Disable NVIC */
	NVIC_DisableIRQ(PVD_IRQn);
}

/*****************************************************************/
/*                 PVD INTERRUPT USER CALLBACK                   */
/*****************************************************************/
__weak void TM_PVD_Handler(uint8_t status) {
	/* NOTE: This function should not be modified, when the callback is needed,
            the TM_PVD_Handler could be implemented in the user file
	*/
}

/*****************************************************************/
/*                    PVD INTERRUPT HANDLER                      */
/*****************************************************************/
void PVD_IRQHandler(void) {
	/* Call user function */
	if (EXTI->PR & PVD_EXTI_LINE) {
		/* Clear flag */
		EXTI->PR = PVD_EXTI_LINE;
		
		/* Call user function with status */
		TM_PVD_Handler(PWR->CSR & PWR_CSR_PVDO);
	}
}
