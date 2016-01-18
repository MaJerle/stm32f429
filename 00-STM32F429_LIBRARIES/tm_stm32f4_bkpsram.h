/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/12/library-45-interface-backup-sram-on-stm32f4
 * @version v1.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   4-kBytes backup ram library for STM32F4xx.
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
#ifndef TM_BKPSRAM_H
#define TM_BKPSRAM_H 110

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_BKPSRAM
 * @brief    4-kBytes backup ram library for STM32F4xx - http://stm32f4-discovery.com/2014/12/library-45-interface-backup-sram-on-stm32f4
 * @{
 *
 * This library is not supported for STM32F401 and STM32F411, because they don't have backup SRAM.
 *
 * @warning If you will try to read from "SRAM memory" on these devices, you will get a "HardFault" error.
 *
 * \par Changelog
 *
@verbatim
 Version 1.1
  - December 22, 2014
  - You can now store float variables, too

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
@endverbatim
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_pwr.h"
#include "defines.h"

/* Check compatibility */
#if defined(STM32F401xx) || defined(STM32F411xE)
	#error "Not available on F401/F411 devices"
#endif	

/**
 * @defgroup TM_BKPSRAM_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Gets memory size for internal backup SRAM
 * @param  None
 * @retval Memory size in bytes
 */
#define TM_BKPSRAM_GetMemorySize()			(0x00001000)

/**
 * @brief  Initializes backup SRAM peripheral
 * @note   This function initializes and enable backup SRAM domain.
 *         
 * @note   With this settings you have access to save/get from locations where SRAM is.
 * @param  None
 * @retval None
 */
void TM_BKPSRAM_Init(void);

/**
 * @brief  Writes 8-bit value to backup SRAM at desired location
 * @param  address: Address where to save data in SRAM.
 * 		      - Value between 0 and TM_BKPSRAM_GetMemorySize() - 1 is valid, if more, HardFault error can happen.
 * @param  value: 8-bit value which will be stored to backup SRAM
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_BKPSRAM_Write8(address, value)	(*(__IO uint8_t *) (BKPSRAM_BASE + (address)) = (value))

/**
 * @brief  Reads 8-bit value from backup SRAM at desired location
 *
 * Parameters:
 * @param  address: Address where to save data in SRAM.
 * 		      - Value between 0 and TM_BKPSRAM_GetMemorySize() - 1 is valid, if more, HardFault error can happen.
 * @retval 8-bit value at specific location
 * @note   Defined as macro for faster execution
 */
#define TM_BKPSRAM_Read8(address)			(*(__IO uint8_t *) (BKPSRAM_BASE + address))

/**
 * @brief  Writes 16-bit value to backup SRAM at desired location
 * @param  address: Address where to save data in SRAM.
 * 		      - Value between 0 and TM_BKPSRAM_GetMemorySize() - 2 is valid, if more, HardFault error can happen.
 * @param  value: 16-bit value which will be stored to backup SRAM
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_BKPSRAM_Write16(address, value)	(*(__IO uint16_t *) (BKPSRAM_BASE + (address)) = (value))

/**
 * @brief  Reads 16-bit value from backup SRAM at desired location
 *
 * Parameters:
 * @param  address: Address where to save data in SRAM.
 * 		      - Value between 0 and TM_BKPSRAM_GetMemorySize() - 2 is valid, if more, HardFault error can happen.
 * @retval 16-bit value at specific location
 * @note   Defined as macro for faster execution
 */
#define TM_BKPSRAM_Read16(address)			(*(__IO uint16_t *) (BKPSRAM_BASE + address))

/**
 * @brief  Writes 32-bit value to backup SRAM at desired location
 * @param  address: Address where to save data in SRAM.
 * 		      - Value between 0 and TM_BKPSRAM_GetMemorySize() - 4 is valid, if more, HardFault error can happen.
 * @param  value: 32-bit value which will be stored to backup SRAM
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_BKPSRAM_Write32(address, value)	(*(__IO uint32_t *) (BKPSRAM_BASE + (address)) = (value))

/**
 * @brief  Reads 32-bit value from backup SRAM at desired location
 *
 * Parameters:
 * @param  address: Address where to save data in SRAM.
 * 		      - Value between 0 and TM_BKPSRAM_GetMemorySize() - 4 is valid, if more, HardFault error can happen.
 * @retval 32-bit value at specific location
 * @note   Defined as macro for faster execution
 */
#define TM_BKPSRAM_Read32(address)			(*(__IO uint32_t *) (BKPSRAM_BASE + address))

/**
 * @brief  Writes 32-bit float value to backup SRAM at desired location
 * @param  address: Address where to save data in SRAM.
 * 		      - Value between 0 and TM_BKPSRAM_GetMemorySize() - 4 is valid, if more, HardFault error can happen.
 * @param  value: 32-bit float value which will be stored to backup SRAM
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_BKPSRAM_WriteFloat(address, value)	(*(__IO float *) (BKPSRAM_BASE + (address)) = (value))

/**
 * @brief  Reads 32-bit float value from backup SRAM at desired location
 *
 * Parameters:
 * @param  address: Address where to save data in SRAM.
 * 		      - Value between 0 and TM_BKPSRAM_GetMemorySize() - 4 is valid, if more, HardFault error can happen.
 * @retval 32-bit float value at specific location
 * @note   Defined as macro for faster execution
 */
#define TM_BKPSRAM_ReadFloat(address)			(*(__IO float *) (BKPSRAM_BASE + address))

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
