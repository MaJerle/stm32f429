/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    
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
extern C {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_RCC
 * @brief    RCC library for STM32F4xx devices
 * @{
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

/**
 * @defgroup TM_RCC_Macros
 * @brief    Library defines
 * @{
 */

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
/* Typedefs here */
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
 *            For that purpose, this function enabled do the following things:
 *              - Enables HSI as system core clock
 *              - Disables PLL
 *              - Sets PLL parameters
 *              - Enables PLL
 *              - Waits will PLL is ready and locked
 *              - Enables PLL as system core clock
 *              - Updates system core clock variable
 * @param  PLLM: Set PLL M parameter. This value can be between 2 and 63.    Use 0 if you don't want to change parameter.
 * @param  PLLN: Set PLL N parameter. This value can be between 192 and 432. Use 0 if you don't want to change parameter.
 * @param  PLLP: Set PLL P parameter. This value can be 2, 4, 6 or 8.        Use 0 if you don't want to change parameter.
 * @param  PLLQ: Set PLL Q parameter. This value can be between 2 and 15.    Use 0 if you don't want to change parameter.
 * @param  PLLR: Set PLL R parameter. This value can be between 2 and 7 and is only available for STM32F446 devices. Use 0 if you don't want to change parameter.
 * @retval None
 */
void TM_RCC_SetPLL(uint16_t PLLM, uint16_t PLLN, uint16_t PLLP, uint16_t PLLQ, uint16_t PLLR);

/** 
 * @brief  Sets PLL M parameter
 * @note   PLL must be stopped and HSI/HSE clock must be used as system clock before you can change this setting
 * @note   Use this function only if you know how PLL clock works in STM32F4xx. Use @ref TM_RCC_SetPLL() instead
 * @param  PLLM: Integer number between 2 and 63
 * @retval None
 */
void TM_RCC_SetPLL_M(uint16_t PLLM);

/** 
 * @brief  Sets PLL N parameter
 * @note   PLL must be stopped and HSI/HSE clock must be used as system clock before you can change this setting
 * @note   Use this function only if you know how PLL clock works in STM32F4xx. Use @ref TM_RCC_SetPLL() instead
 * @param  PLLN: Integer number between 192 and 432
 * @retval None
 */
void TM_RCC_SetPLL_N(uint16_t PLLN);

/** 
 * @brief  Sets PLL P parameter
 * @note   PLL must be stopped and HSI/HSE clock must be used as system clock before you can change this setting
 * @note   Use this function only if you know how PLL clock works in STM32F4xx. Use @ref TM_RCC_SetPLL() instead
 * @param  PLLP: Integer numbers 2, 4, 6, 8 are valid
 * @retval None
 */
void TM_RCC_SetPLL_P(uint16_t PLLP);

/** 
 * @brief  Sets PLL Q parameter
 * @note   PLL must be stopped and HSI/HSE clock must be used as system clock before you can change this setting
 * @note   Use this function only if you know how PLL clock works in STM32F4xx. Use @ref TM_RCC_SetPLL() instead
 * @param  PLLQ: Integer number between 2 and 15
 * @retval None
 */
void TM_RCC_SetPLL_Q(uint16_t PLLQ);

/** 
 * @brief  Sets PLL R parameter
 * @note   PLL must be stopped and HSI/HSE clock must be used as system clock before you can change this setting
 * @note   Use this function only if you know how PLL clock works in STM32F4xx. Use @ref TM_RCC_SetPLL() instead
 * @param  PLLR: Integer number between 2 and 7
 * @note   Only for STM32F446xx devices
 * @retval None
 */
void TM_RCC_SetPLL_R(uint16_t PLLR);

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
