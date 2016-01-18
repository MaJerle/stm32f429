/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/10/library-39-power-consumption-modes-for-stm32f4
 * @version v1.2
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   STM32F4xx low power modes library 
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
#ifndef TM_LOWPOWER_H
#define TM_LOWPOWER_H 120

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_LOWPOWER
 * @brief    Low power library for STM32F4xx devices - http://stm32f4-discovery.com/2014/10/library-39-power-consumption-modes-for-stm32f4
 * @{
 *
 * This library can put MCU into 3 different modes:
 *
 * \par SLEEP mode
 *
 * - Cortex-M4 core is stopped
 * - Peripherals kept running
 * - How to enter this mode?
 *    - Use TM_LOWPOWER_SleepUntilInterrupt() or
 *    - Use TM_LOWPOWER_SleepUntilEvent();
 * - How to exit this mode?
 *    - Any peripheral interrupt acknowledged by the nested vectored interrupt controller (NVIC)
 *
 * \par STOP mode
 *
 * - All clocks in 1.2V domain are stopped
 * - PLL, HSI and HSE RC oscillators disabled
 * - Internal SRAM and register contents are preserved
 * - Voltage regulator in low-power mode
 * - How to enter this mode?
 *    - Use TM_LOWPOWER_StopUntilEvent() or
 *    - Use TM_LOWPOWER_StopUntilInterrupt()
 * - How to exit this mode?
 *    - Any EXTI Line (Internal or External) configured in Interrupt/Event mode
 *
 * \par STANDBY mode
 *
 * - Cortex-M4 deepsleep mode
 * - Voltage regulator disabled
 * - 1.2V domain consequently powered off
 * - PLL, HSI and HSE RC oscillators disabled
 * - SRAM and register contents are lost except for the RTC registers, RTC backup registers, backup SRAM and Standby circuitry
 * - How to enter this mode?
 *    - Use TM_LOWPOWER_Standby() function
 * - How to exit this mode?
 *    - Wakeup pin rising edge
 *    - RTC alarm (Alarm A and B) and wakeup
 *    - tamper and time-stamp event
 *    - external reset in NRST pin
 *    - IWDG reset
 * 
 * \par Changelog
 *
@verbatim
 Version 1.2
  - March 23, 2015
  - Fixed problems with entering to standby mode
  
 Version 1.1
  - October 18, 2014
  - Added TM_LOWPOWER_StandbyReset(). This allows you to check if system was reset from standby wakeup

 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - STM32F4xx PWR
 - defines.h
 - TM DELAY
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_pwr.h"
#include "defines.h"
#include "tm_stm32f4_delay.h"

/**
 * @defgroup TM_LOWPOWER_Macros
 * @brief    Library macros
 * @{
 */
 
/**
 * @}
 */
 
/**
 * @defgroup TM_LOWPOWER_Typedefs
 * @brief    Library typedefs
 * @{
 */

typedef enum {
	TM_LOWPOWERMODE_SleepUntilInterrupt = 0x00, /*!< Sleep mode until any interrupt in NVIC occurs */
	TM_LOWPOWERMODE_SleepUntilEvent,            /*!< Sleep mode until any event occurs */
	TM_LOWPOWERMODE_StopUntilInterrupt,         /*!< Stop mode until interrupt in EXTI line occurs */
	TM_LOWPOWERMODE_StopUntilEvent,             /*!< Stop mode until event occurs */
	TM_LOWPOWERMODE_Standby                     /*!< Standby mode until any interrupt occurs */
} TM_LOWPOWERMODE_t;

/**
 * @}
 */
 
/**
 * @defgroup TM_LOWPOWER_Functions
 * @brief    Library Functions
 *
 *           All functions for sleep mode configuration
 * @{
 */

/**
 * @brief  Put device into sleep mode
 * 
 * @note   MCU will be in sleep mode until next interrupt occured.
 *         This means, that delay timer (Systick or any user selectable delay timer) can wakeup STM32F4 too.
 *         If you want prevent this, you have to disable delay timer as well it.
 *         This can be done with this function.
 *
 * @param  delay_timer: Set to > 0 and delay timer will be disabled or set to 0 and it will stay as is
 * @retval None
 */
void TM_LOWPOWER_SleepUntilInterrupt(uint8_t delay_timer);

/**
 * @brief  Put device into sleep mode
 * @note   MCU will be in sleep mode until next event occured
 * @param  None
 * @retval None
 */
void TM_LOWPOWER_SleepUntilEvent(void);

/**
 * @brief  Go to STOP mode with "Wait For Interrupt" instruction
 * @param  None
 * @retval None
 */
void TM_LOWPOWER_StopUntilInterrupt(void);

/**
 * @brief  Go to STOP mode with "Wait For Event" instruction
 * @param  None
 * @retval None
 */
void TM_LOWPOWER_StopUntilEvent(void);

/**
 * @brief  To go Standby mode
 * @param  None
 * @retval None
 */
void TM_LOWPOWER_Standby(void);

/**
 * @brief  Checks if system was reset because of wakeup from standby mode
 * @param  None
 * @retval 0: System was not reset because of wake up from standby mode
 *         1: System was reset because of wake up from standby mode
 */
uint8_t TM_LOWPOWER_StandbyReset(void);

/**
 * @brief  Enables Wakeup pin, fixed pin, PA0
 * @note   This pin is fixed and can't be changed.
 * @note   Rising edge on wakeup pin will wake up device from standby mode
 * @param  None
 * @retval None
 */
void TM_LOWPOWER_EnableWakeUpPin(void);

/**
 * @brief  Disables Wakeup pin
 * @note   If you go to standby mode after PA0 is disabled, then you can't wake him up with rising edge
 * @param  None
 * @retval None
 */
void TM_LOWPOWER_DisableWakeUpPin(void);

/**
 * @}
 */

/**
 * @}
 */
 
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
