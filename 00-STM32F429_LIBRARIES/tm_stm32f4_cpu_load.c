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
#include "tm_stm32f4_cpu_load.h"

uint8_t TM_CPULOAD_Init(TM_CPULOAD_t* CPU_Load) {
	/* Set values to 0 */
	CPU_Load->Load = 0;
	CPU_Load->SCNT = 0;
	CPU_Load->WCNT = 0;
	CPU_Load->Updated = 0;
	
	/* Return DWT counter enabled status */
	return TM_GENERAL_DWTCounterEnable();
}

uint8_t TM_CPULOAD_GoToSleepMode(TM_CPULOAD_t* CPU_Load, TM_LOWPOWERMODE_t PowerMode) {
	uint32_t t;
	static uint32_t l = 0;
	static uint32_t WorkingTime = 0;
	static uint32_t SleepingTime = 0;
	
	/* Add to working time */
	WorkingTime += DWT->CYCCNT - l;
	
	/* Save count cycle time */
	t = DWT->CYCCNT;
	
	/* Disable interrupts */
	__disable_irq();
	
	/* Go to sleep mode */
	/* Wait for wake up interrupt, systick can do it too */
	if (PowerMode == TM_LOWPOWERMODE_SleepUntilInterrupt) {
		__WFI();
	} else {
		__WFE();
	}
	
	/* Increase number of sleeping time in CPU cycles */
	SleepingTime += DWT->CYCCNT - t;
	
	/* Save current time to get number of working CPU cycles */
	l = DWT->CYCCNT;
	
	/* Enable interrupts, process/execute an interrupt routine which wake up CPU */
	__enable_irq();
	
	/* Reset flag */
	CPU_Load->Updated = 0;
	
	/* Every 1000ms print CPU load via USART */
	if ((SleepingTime + WorkingTime) >= SystemCoreClock) {
		/* Save values */
		CPU_Load->SCNT = SleepingTime;
		CPU_Load->WCNT = WorkingTime;
		CPU_Load->Load = ((float)WorkingTime / (float)(SleepingTime + WorkingTime) * 100);
		CPU_Load->Updated = 1;
		
		/* Reset time */
		SleepingTime = 0;
		WorkingTime = 0;
	}
	
	/* Return updated status */
	return CPU_Load->Updated;
}
