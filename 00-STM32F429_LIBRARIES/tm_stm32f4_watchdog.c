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
#include "tm_stm32f4_watchdog.h"

uint8_t TM_WATCHDOG_Init(TM_WATCHDOG_Timeout_t timeout) {
	uint8_t result = 0;
	uint16_t reload = 0;

	/* Check if the system has resumed from IWDG reset */
	if (RCC->CSR & (0x20000000)) {
		/* Reset by IWDG */
		result = 1;
		
		/* Clear reset flags */
		RCC->CSR |= RCC_CSR_RMVF;
	}

	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG->KR = 0x5555;

	/* IWDG counter clock: LSI/32 = 1024Hz */
	IWDG->PR = 0x03;
	
	/* Set counter reload value */
	if (timeout == TM_WATCHDOG_Timeout_5ms) {
		reload = 5;
	} else if (timeout == TM_WATCHDOG_Timeout_10ms) {
		reload = 10;
	} else if (timeout == TM_WATCHDOG_Timeout_15ms) {
		reload = 15;
	} else if (timeout == TM_WATCHDOG_Timeout_30ms) {
		reload = 31;
	} else if (timeout == TM_WATCHDOG_Timeout_60ms) {
		reload = 61;
	} else if (timeout == TM_WATCHDOG_Timeout_120ms) {
		reload = 123;
	} else if (timeout == TM_WATCHDOG_Timeout_250ms) {
		reload = 256;
	} else if (timeout == TM_WATCHDOG_Timeout_500ms) {
		reload = 512;
	} else if (timeout == TM_WATCHDOG_Timeout_1s) {
		reload = 1024;
	} else if (timeout == TM_WATCHDOG_Timeout_2s) {
		reload = 2048;
	} else if (timeout == TM_WATCHDOG_Timeout_4s) {
		reload = 4095;
	}
	
	/* Set reload */
	IWDG->RLR = reload;

	/* Reload IWDG counter */
	IWDG->KR = 0xAAAA;

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG->KR = 0xCCCC;
	
	/* Return status */
	return result;
}

void TM_WATCHDOG_Reset(void) {
	/* Reload IWDG counter */
	IWDG->KR = 0xAAAA;
}

