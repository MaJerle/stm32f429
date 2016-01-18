/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/05/library-59-change-pll-settings-while-stm32f4xx-is-running
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   RCC library for STM32F4xx devices
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
#ifndef TM_RCC_H
#define TM_RCC_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_RCC
 * @brief    RCC library for STM32F4xx devices - http://stm32f4-discovery.com/2015/05/library-59-change-pll-settings-while-stm32f4xx-is-running
 * @{
 *
 * This library allows you to change PLL settings on the FLY.
 * 
 * It can be used to dynamically change system clock for various reasons.
 *
 * @note After you change PLL settings, some peripherals (like USART, Systick, TIMERS) will no longer work properly.
 *       Doing a reinit would fix that!
 * 
 * \par Changelog
 *
@verbatim
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
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "defines.h"
#include "string.h"

/**
 * @defgroup TM_RCC_Macros
 * @brief    Library defines
 * @{
 */

/* PLL configuration */
#define RCC_PLLM_MASK    ((uint32_t)0x0000003F)
#define RCC_PLLM_POS     0
#define RCC_PLLN_MASK    ((uint32_t)0x00007FC0)
#define RCC_PLLN_POS     6
#define RCC_PLLP_MASK    ((uint32_t)0x00030000)
#define RCC_PLLP_POS     16
#define RCC_PLLQ_MASK    ((uint32_t)0x0F000000)
#define RCC_PLLQ_POS     24
#define RCC_PLLR_MASK    ((uint32_t)0x70000000)
#define RCC_PLLR_POS     28

/**
 * @}
 */
 
/**
 * @defgroup TM_RCC_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  PLL structure with settings for read and write operations 
 */
typedef struct {
	uint16_t PLLM; /*!< PLL M parameter. This value can be between 2 and 63.    Use 0 if you don't want to change parameter. */
	uint16_t PLLN; /*!< PLL N parameter. This value can be between 192 and 432. Use 0 if you don't want to change parameter. */ 
	uint16_t PLLP; /*!< PLL P parameter. This value can be 2, 4, 6 or 8.        Use 0 if you don't want to change parameter. */
	uint16_t PLLQ; /*!< PLL Q parameter. This value can be between 2 and 15.    Use 0 if you don't want to change parameter. */
	uint16_t PLLR; /*!< PLL R parameter. This value can be between 2 and 7 and is only available for STM32F446 devices. 
	                       Use 0 if you don't want to change parameter. */
} TM_RCC_PLL_t;

/**
 * @}
 */

/**
 * @defgroup TM_RCC_Functions
 * @brief    Library Functions
 * @{
 */

/** 
 * @brief  Sets the main PLL settings for STM32F4xx device
 * @note   PLL can only be configured when PLL is not used as system clock.
 *            For that purpose, this function does the following things:
 *              - Enables HSI as system core clock
 *              - Disables PLL
 *              - Sets PLL parameters passed as parameters in function
 *              - Enables PLL
 *              - Waits will PLL is ready and locked
 *              - Enables PLL as system core clock
 *              - Updates system core clock variable
 * @param  *PLL_Settings: Pointer to @ref TM_RCC_PLL_t structure with PLL settings.
 *            Use 0 for setting you don't want to change
 * @retval None
 */
void TM_RCC_SetPLL(TM_RCC_PLL_t* PLL_Settings);

/**
 * @brief  Gets current PLL settings from RCC registers
 * @note   You can use this function to read current PLL settings before applying new settings
 * @param  *PLL_Settings: Pointer to @ref TM_RCC_PLL_t structure where PLL settings will be stored
 * @retval None
 */
void TM_RCC_GetPLL(TM_RCC_PLL_t* PLL_Settings);

/**
 * @brief  Checks if main PLL is ready
 * @param  None
 * @retval PLL ready status:
 *            - 0: PLL is not ready
 *            - > 0: PLL is ready
 */
uint8_t TM_RCC_IsPLLReady(void);

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
