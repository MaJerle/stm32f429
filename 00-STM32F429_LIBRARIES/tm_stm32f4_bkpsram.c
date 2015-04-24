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
#include "tm_stm32f4_bkpsram.h"

void TM_BKPSRAM_Init(void) {
	/* Enable PWR clock */
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	
	/* Enable backup SRAM Clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_BKPSRAMEN;
	
	/* Allow access to backup domain */
	PWR_BackupAccessCmd(ENABLE);
	
	/* Enable the Backup SRAM low power Regulator */
	/* This will allow data to stay when using VBat mode */
	PWR_BackupRegulatorCmd(ENABLE);
	
	/* Wait for backup regulator to be ready  */
	while (!(PWR->CSR & (PWR_FLAG_BRR)));
}
