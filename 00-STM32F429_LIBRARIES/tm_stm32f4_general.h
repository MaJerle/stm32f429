/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/03/library-54-general-library-for-stm32f4xx-devices
 * @version v1.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   GENERAL library for STM32F4xx devices
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
#ifndef TM_GENERAL_H
#define TM_GENERAL_H 110

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_GENERAL
 * @brief    GENERAL library for STM32F4xx devices - http://stm32f4-discovery.com/2015/03/library-54-general-library-for-stm32f4xx-devices
 * @{
 *
 * This is a simple library, where it's content can be used in a lot of places in your project.
 *
 *
 * \par Supported actions with library
 *
@verbatim
- Support for software reset with callback
- Get reset source, what resets your MCU
- Disable or enable global NVIC interrupts
- Get different clock speeds in your system
- Operate with Cortex-M4 DWT hardware counter
@endverbatim
 *
 * \par Changelog
 *
@verbatim
 Version 1.1
  - April 07, 2015
  - Added support for DWT counter
  
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - defines.h
 - attributes.h
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "defines.h"
#include "attributes.h"

/**
 * @defgroup TM_GENERAL_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  System speed in MHz
 */
extern uint16_t GENERAL_SystemSpeedInMHz;

/**
 * @}
 */
 
/**
 * @defgroup TM_GENERAL_Variables
 * @brief    Library variables
 * @{
 */
 
/**
 * @}
 */
 
/**
 * @defgroup TM_GENERAL_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Clock speed enumeration
 */
typedef enum {
	TM_GENERAL_Clock_HSI,    /*!< High speed internal clock */
	TM_GENERAL_Clock_HSE,    /*!< High speed external clock */
	TM_GENERAL_Clock_SYSCLK, /*!< System core clock */
	TM_GENERAL_Clock_PCLK1,  /*!< PCLK1 (APB1) peripheral clock */
	TM_GENERAL_Clock_PCLK2,  /*!< PCLK2 (APB2) peripheral clock */
	TM_GENERAL_Clock_HCLK    /*!< HCLK (AHB1) high speed clock */
} TM_GENERAL_Clock_t;

/**
 * @brief  All possible reset sources
 */
typedef enum {
	TM_GENERAL_ResetSource_None = 0x00,     /*!< No reset source detected. Flags are already cleared */
	TM_GENERAL_ResetSource_LowPower = 0x01, /*!< Low-power management reset occurs */
	TM_GENERAL_ResetSource_WWDG = 0x02,     /*!< Window watchdog reset occurs */
	TM_GENERAL_ResetSource_IWDG = 0x03,     /*!< Independent watchdog reset occurs */
	TM_GENERAL_ResetSource_Software = 0x04, /*!< Software reset occurs */
	TM_GENERAL_ResetSource_POR = 0x05,      /*!< POR/PDR reset occurs */
	TM_GENERAL_ResetSource_PIN = 0x06,      /*!< NRST pin is set to low by hardware reset, hardware reset */
	TM_GENERAL_ResetSource_BOR = 0x07,      /*!< BOR reset occurs */
} TM_GENERAL_ResetSource_t;

/**
 * @}
 */

/**
 * @defgroup TM_GENERAL_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Performs a system reset
 * @note   Before system will be reset, @ref TM_GENERAL_SoftwareResetCallback() will be called,
 *         where you can do important stuff if necessary
 * @param  None
 * @retval None
 */
void TM_GENERAL_SystemReset(void);

/**
 * @brief  Gets reset source why system was reset
 * @param  reset_flags: After read, clear reset flags
 *            - 0: Flags will stay untouched
 *            - > 0: All reset flags will reset
 * @retval Member of @ref TM_GENERAL_ResetSource_t containing reset source
 */
TM_GENERAL_ResetSource_t TM_GENERAL_GetResetSource(uint8_t reset_flags);

/**
 * @brief  Disables all interrupts in system
 * @param  None
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_GENERAL_DisableInterrupts()   __disable_irq()

/**
 * @brief  Enables interrupts in system.
 * @note   Only defined interrupts in NVIC are enabled
 * @param  None
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_GENERAL_EnableInterrupts()   __enable_irq()

/**
 * @brief  Gets specific clock speed value from STM32F4xx device
 * @param  clock: Clock type you want to know speed for. This parameter can be a value of @ref TM_GENERAL_Clock_t enumeration
 * @retval Clock speed in units of hertz
 */
uint32_t TM_GENERAL_GetClockSpeed(TM_GENERAL_Clock_t clock);

/**
 * @brief  Gets system clock speed in units of MHz
 * @param  None
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_GENERAL_GetSystemClockMHz()    ((uint16_t)(SystemCoreClock * (float)0.000001))

/**
 * @brief  Enables DWT counter in Cortex-M4 core
 * @param  None
 * @retval DWT Status:
 *            - 0: DWT has not started, hardware/software reset is required
 *            - >0: DWT has started and is ready to use
 * @note   It may happen, that DWT counter won't start after reprogramming device.
 *         This happened to me when I use onboard ST-Link on Discovery or Nucleo boards.
 *         When I used external debugger (J-Link or ULINK2) it worked always without problems.
 *         If your DWT doesn't start, you should perform software/hardware reset by yourself.
 */
uint8_t TM_GENERAL_DWTCounterEnable(void);

/**
 * @brief  Disables DWT counter in Cortex-M4 core
 * @param  None
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_GENERAL_DWTCounterDisable()    (DWT->CTRL &= ~0x00000001)

/**
 * @brief  Gets current DWT counter value
 * @param  None
 * @retval DWT counter value
 * @note   Defined as macro for faster execution
 */
#define TM_GENERAL_DWTCounterGetValue()  (DWT->CYCCNT)

/**
 * @brief  Sets DWT counter value
 * @param  x: Set value for DWT counter
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_GENERAL_DWTCounterSetValue(x)  (DWT->CYCCNT = (x))

/**
 * @brief  Delays for amount of microseconds using DWT counter
 * @param  micros: Number of micro seconds for delay 
 * @retval None
 * @note   DWT Counter HAVE to be initialized first using @ref TM_GENERAL_EnableDWTCounter()
 */
static __INLINE void TM_GENERAL_DWTCounterDelayus(uint32_t micros) {
	uint32_t c = TM_GENERAL_DWTCounterGetValue();
	
	/* Calculate clock cycles */
	micros *= (SystemCoreClock / 1000000);
	micros -= 12;
	
	/* Wait till done */
	while ((TM_GENERAL_DWTCounterGetValue() - c) < micros) {
		/* Do nothing */
	}
}

/**
 * @brief  Delays for amount of milliseconds using DWT counter
 * @param  millis: Number of micro seconds for delay 
 * @retval None
 * @note   DWT Counter HAVE to be initialized first using @ref TM_GENERAL_EnableDWTCounter()
 */
static __INLINE void TM_GENERAL_DWTCounterDelayms(uint32_t millis) {
	uint32_t c = TM_GENERAL_DWTCounterGetValue();
	
	/* Calculate clock cycles */
	millis *= (SystemCoreClock / 1000);
	millis -= 12;
	
	/* Wait till done */
	while ((TM_GENERAL_DWTCounterGetValue() - c) < millis) {
		/* Do nothing */
	}
}

/**
 * @brief  Software reset callback.
 *         Function is called before software reset occurs.
 * @param  None
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
__weak void TM_GENERAL_SoftwareResetCallback(void);

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
