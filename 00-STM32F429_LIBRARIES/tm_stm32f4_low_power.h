/**
 *	Put STM32F4xx into low power mode
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/10/library-39-power-consumption-modes-for-stm32f4
 *	@version 	v1.1
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 *	
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
 *	
 * Changelog
 *	- Version 1.1
 *	- October 18, 2014
 *	- Added TM_LOWPOWER_StandbyReset(). This allows you to check if system was reset from standby wakeup
 *
 * This library can put MCU into 3 different modes:
 *
 * SLEEP mode
 *
 *	- Cortex-M4 core is stopped
 *	- Peripherals kept running
 *	- How to enter this mode?
 *		- Use TM_LOWPOWER_SleepUntilInterrupt() or
 *		- Use TM_LOWPOWER_SleepUntilEvent();
 *	- How to exit this mode?
 *		- Any peripheral interrupt acknowledged by the nested vectored interrupt controller (NVIC)
 *
 * STOP mode
 *
 *	- All clocks in 1.2V domain are stopped
 *	- PLL, HSI and HSE RC oscillators disabled
 *	- Internal SRAM and register contents are preserved
 *	- Voltage regulator in low-power mode
 *	- How to enter this mode?
 *		- Use TM_LOWPOWER_StopUntilEvent() or
 *		- Use TM_LOWPOWER_StopUntilInterrupt()
 *	- How to exit this mode?
 *		- Any EXTI Line (Internal or External) configured in Interrupt/Event mode
 *
 * STANDBY mode
 *
 *	- Cortex-M4 deepsleep mode
 *	- Voltage regulator disabled
 *	- 1.2V domain consequently powered off
 *	- PLL, HSI and HSE RC oscillators disabled
 *	- SRAM and register contents are lost except for the RTC registers, RTC backup registers, backup SRAM and Standby circuitry
 *	- How to enter this mode?
 *		- Use TM_LOWPOWER_Standby() function
 *	- How to exit this mode?
 *		- Wakeup pin rising edge
 *		- RTC alarm (Alarm A and B) and wakeup
 *		- tamper and time-stamp event
 *		- external reset in NRST pin
 *		- IWDG reset
 */
#ifndef TM_LOWPOWER_H
#define TM_LOWPOWER_H 110

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx PWR
 * - defines.h
 * - TM DELAY
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_pwr.h"
#include "defines.h"
#include "tm_stm32f4_delay.h"

/**
 * Go to sleep mode
 * MCU will be in sleep mode until next interrupt occured.
 * This means, that delay timer (Systick or any user selectable delay timer) can wakeup STM32F4 too.
 * If you want prevent this, you have to disable delay timer as well it.
 * This can be done with this function
 *
 * Parameters:
 * 	- uint8_t delay_timer:
 * 		0: Leave delay timer as it is
 * 		> 0: Before enter sleep mode, disable delay timer, after wakeup, enable delay timer back
 *
 * No return
 */
extern void TM_LOWPOWER_SleepUntilInterrupt(uint8_t delay_timer);

/**
 * Go to sleep mode
 * MCU will be in sleep mode until next event occured.
 *
 * No return
 */
extern void TM_LOWPOWER_SleepUntilEvent(void);

/**
 * Go to STOP mode with "Wait For Interrupt" instruction
 *
 * No return
 */
extern void TM_LOWPOWER_StopUntilInterrupt(void);

/**
 * Go to STOP mode with "Wait For Event" instruction
 *
 * No return
 */
extern void TM_LOWPOWER_StopUntilEvent(void);

/**
 * Go to standby mode
 *
 * No return
 */
extern void TM_LOWPOWER_Standby(void);

/**
 * Check if system was reset because of wakeup from standby mode
 *
 * Returns
 * 	0: System was not reset because of wake up from standby mode
 * 	1: System was reset because of wake up from standby mode
 */
extern uint8_t TM_LOWPOWER_StandbyReset(void);

/**
 * Enable wakeup pin, fixed pin, PA0.
 * Rising edge of this pin will wakeup MCU from Standby mode.
 *
 * No return
 */
extern void TM_LOWPOWER_EnableWakeUpPin(void);

/**
 * Disable wakeup pin functionality
 *
 * No return
 */
extern void TM_LOWPOWER_DisableWakeUpPin(void);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
