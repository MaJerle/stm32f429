/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/05/library-14-working-with-sdram-on-stm32f429-discovery/
 * @version v1.3
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   External SDRAM for STM32F429-Discovery or STM324x9-EVAL boards
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
#ifndef TM_SDRAM_H
#define TM_SDRAM_H 130
/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_SDRAM
 * @brief    Connect external SDRAM to STM32F4xx devices with integrated FMC peripheral -  http://stm32f4-discovery.com/2014/05/library-14-working-with-sdram-on-stm32f429-discovery/
 * @{
 *
 * This library allows you to connect and use external SDRAM.
 *
 * It supports 2 STM32F4 boards:
 *  
 *  - STM32F429-Discovery:
 *     - 8MB (64Mbit) memory size
 *  - STM324x9-EVAL:
 *     - 8MB (64Mbit) memory size
 *
 * @par Set your target
 * 
 * By default, STM32F429-Discovery configuration is used, because this board is cheap and most users uses this.
 *
 * If you are working with STM324x9-EVAL board, you can add define in defines.h file and enable configuration for SDRAM on this board.
 *
@verbatim
//Use SDRAM on STM324x9-EVAL board
#define SDRAM_USE_STM324x9_EVAL
@endverbatim
 *
 * \par STM32F429-Discovery pinout
 *
@verbatim
PB5  <-> FMC_SDCKE | PC0  <-> FMC_SDNWE | PD0  <-> FMC_D2   | PE0  <-> FMC_NBL0  | PF0  <-> FMC_A0    | PG0  <-> FMC_A10
PB6  <-> FMC_SDNE1 |                    | PD1  <-> FMC_D3   | PE1  <-> FMC_NBL1  | PF1  <-> FMC_A1    | PG1  <-> FMC_A11
                   |                    | PD8  <-> FMC_D13  | PE7  <-> FMC_D4    | PF2  <-> FMC_A2    | PG4  <-> FMC_BA0
                   |                    | PD9  <-> FMC_D14  | PE8  <-> FMC_D5    | PF3  <-> FMC_A3    | PG4  <-> FMC_BA1
                   |                    | PD10 <-> FMC_D15  | PE9  <-> FMC_D6    | PF4  <-> FMC_A4    | PG8  <-> FMC_SDCLK
                   |                    | PD14 <-> FMC_D0   | PE10 <-> FMC_D7    | PF5  <-> FMC_A5    | PG15 <-> FMC_NCAS
                   |                    | PD15 <-> FMC_D1   | PE11 <-> FMC_D8    | PF11 <-> FMC_NRAS  | 
                   |                    |                   | PE12 <-> FMC_D9    | PF12 <-> FMC_A6    | 
                   |                    |                   | PE13 <-> FMC_D10   | PF13 <-> FMC_A7    |    
                   |                    |                   | PE14 <-> FMC_D11   | PF14 <-> FMC_A8    |
                   |                    |                   | PE15 <-> FMC_D12   | PF15 <-> FMC_A9    |
@endverbatim
 *
 * \par STM324x9-EVAL pinout
 * 
@verbatim
PD0  <-> FMC_D2   | PE0  <-> FMC_NBL0  | PF0  <-> FMC_A0    | PG0  <-> FMC_A10   | PH2  <-> FMC_SDCKE0| PI0  <-> FMC_D24  
PD1  <-> FMC_D3   | PE1  <-> FMC_NBL1  | PF1  <-> FMC_A1    | PG1  <-> FMC_A11   | PH3  <-> FMC_SDNE0 | PI1  <-> FMC_D25   
PD8  <-> FMC_D13  | PE7  <-> FMC_D4    | PF2  <-> FMC_A2    | PG4  <-> FMC_A14   | PH5  <-> FMC_SDNW  | PI2  <-> FMC_D26  
PD9  <-> FMC_D14  | PE8  <-> FMC_D5    | PF3  <-> FMC_A3    | PG5  <-> FMC_A15   | PH8  <-> FMC_D16   | PI3  <-> FMC_D27  
PD10 <-> FMC_D15  | PE9  <-> FMC_D6    | PF4  <-> FMC_A4    | PG8  <-> FC_SDCLK  | PH9  <-> FMC_D17   | PI4  <-> FMC_NBL2
PD14 <-> FMC_D0   | PE10 <-> FMC_D7    | PF5  <-> FMC_A5    | PG15 <-> FMC_NCAS  | PH10 <-> FMC_D18   | PI5  <-> FMC_NBL3 
PD15 <-> FMC_D1   | PE11 <-> FMC_D8    | PF11 <-> FC_NRAS   | PH11 <-> FMC_D19   | PH12 <-> FMC_D20   | PI6  <-> FMC_D28  
                  | PE12 <-> FMC_D9    | PF12 <-> FMC_A6    |                    | PH13 <-> FMC_D21   | PI7  <-> FMC_D29      
                  | PE13 <-> FMC_D10   | PF13 <-> FMC_A7    |                    | PH14 <-> FMC_D22   | PI9  <-> FMC_D30  
                  | PE14 <-> FMC_D11   | PF14 <-> FMC_A8    |                    | PH15 <-> FMC_D23   | PI10 <-> FMC_D31
                  | PE15 <-> FMC_D12   | PF15 <-> FMC_A9    |                    |                    |
@endverbatim                                                                                            
 *          
 * \par Changelog
 *
@verbatim
 Version 1.3
  - March 19, 2015
  - Added support for STM324x9_EVAL board

 Version 1.2
  - March 10, 2015
  - Added support for my new GPIO library

 Version 1.1
  - November 23, 2014
  - Instead of calling functions to read/write data to RAM, functions are now defined as macros
  - Much faster execution because we don't need to call functions and put data on stack.
 
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - STM32F4xx FMC
 - defines.h
 - attributes.h
 - TM GPIO
@endverbatim
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_fmc.h"
#include "defines.h"
#include "attributes.h"
#include "tm_stm32f4_gpio.h"

/* Library supports STM324x9_Eval board too */

/**
 * @defgroup TM_SDRAM_Macros
 * @brief    Library defines
 * @{
 *
 * Library defines depends on board you use.
 */
#if defined(SDRAM_USE_STM324x9_EVAL) || defined(TM_DISCO_STM324x9_EVAL)

/* Make define */
#ifndef SDRAM_USE_STM324x9_EVAL
#define SDRAM_USE_STM324x9_EVAL
#endif

/**
 * @defgroup TM_SDRAM_STM324x9_EVAL_Macros
 * @brief    Macros for STM324x9-EVAL board
 * @{
 */
	/* SDRAM start address = FMC peripheral start address */
	#define SDRAM_START_ADR             (uint32_t)0xC0000000
	/* SDRAM max memory size = 8MB */
	#define SDRAM_MEMORY_SIZE           (uint32_t)0x800000
	/* SDRAM bank */
	#define SDRAM_BANK                  FMC_Bank1_SDRAM
	/* Bank target command */
	#define SDRAM_COMMAND_TARGET_BANK   FMC_Command_Target_bank1
	/* Refresh count */
	#define SDRAM_REFRESH_COUNT         1385
	/* Number of rows */
	#define SDRAM_ROWBITS_NUMBER        FMC_RowBits_Number_11b
	/* Data witdh */
	#define SDRAM_MEMORY_WIDTH          FMC_SDMemory_Width_32b
	/* Read burst */
	#define SDRAM_READ_BURST_STATE      FMC_Read_Burst_Disable
	/* Register value */
	#define SDRAM_REG_VALUE             0x0230
/**
 * @}
 */
#else
/**
 * @defgroup TM_SDRAM_STM32F429_Discovery_Macros
 * @brief    Macros for STM32F429-Discovery board
 * @{
 */
	/* SDRAM start address = FMC peripheral start address */
	#define SDRAM_START_ADR             (uint32_t)0xD0000000
	/* SDRAM max memory size = 8MB */
	#define SDRAM_MEMORY_SIZE           (uint32_t)0x800000
	/* SDRAM bank */
	#define SDRAM_BANK                  FMC_Bank2_SDRAM
	/* Bank target command */
	#define SDRAM_COMMAND_TARGET_BANK   FMC_Command_Target_bank2
	/* Refresh count */
	#define SDRAM_REFRESH_COUNT         680
	/* Number of rows */
	#define SDRAM_ROWBITS_NUMBER        FMC_RowBits_Number_12b
	/* Data witdh */
	#define SDRAM_MEMORY_WIDTH          FMC_SDMemory_Width_16b
	/* Read burst */
	#define SDRAM_READ_BURST_STATE      FMC_Read_Burst_Disable
	/* Register value */
	#define SDRAM_REG_VALUE             0x0231
/**
 * @}
 */
#endif /* SDRAM_USE_STM324x9_EVAL */

/* Timeout for SDRAM initialization */
#define SDRAM_TIMEOUT                   ((uint32_t)0xFFFF) 

/**
 * @}
 */

/**
 * @defgroup TM_SDRAM_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes SDRAM
 * @param  None
 * @retval SDRAM status
 *            - 0: SDRAM is not configured OK
 *            - > 0: SDRAM is configured OK and prepared to work
 */
uint8_t TM_SDRAM_Init(void);

/**
 * @brief  Gets SDRAM memory size
 * @param  None
 * @retval SDRAM memory size in bytes
 */
#define TM_SDRAM_GetSize()                  (SDRAM_MEMORY_SIZE)

/**
 * @brief  Writes 8-bit value to SDRAM
 * @param  address: Offset address from starting address of SDRAM
 * @param  value: 8-bit value to be saved in SDRAM
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_SDRAM_Write8(address, value)		(*(__IO uint8_t *) (SDRAM_START_ADR + (address)) = (value))

/**
 * @brief  Reads 8-bit value from SDRAM
 * @param  address: offset address from starting address of SDRAM
 * @retval 8-bit value stored at desired location
 * @note   Defined as macro for faster execution
 */
#define TM_SDRAM_Read8(address)				(*(__IO uint8_t *) (SDRAM_START_ADR + (address)))

/**
 * @brief  Writes 16-bit value to SDRAM
 * @param  address: Offset address from starting address of SDRAM
 * @param  value: 16-bit value to be saved in SDRAM
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_SDRAM_Write16(address, value)	(*(__IO uint16_t *) (SDRAM_START_ADR + (address)) = (value))

/**
 * @brief  Reads 16-bit value from SDRAM
 * @param  address: offset address from starting address of SDRAM
 * @retval 16-bit value stored at desired location
 * @note   Defined as macro for faster execution
 */
#define TM_SDRAM_Read16(address)			(*(__IO uint16_t *) (SDRAM_START_ADR + (address)))

/**
 * @brief  Writes 32-bit value to SDRAM
 * @param  address: Offset address from starting address of SDRAM
 * @param  value: 32-bit value to be saved in SDRAM
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_SDRAM_Write32(address, value)	(*(__IO uint32_t *) (SDRAM_START_ADR + (address)) = (value))

/**
 * @brief  Reads 32-bit value from SDRAM
 * @param  address: offset address from starting address of SDRAM
 * @retval 32-bit value stored at desired location
 * @note   Defined as macro for faster execution
 */
#define TM_SDRAM_Read32(address)			(*(__IO uint32_t *) (SDRAM_START_ADR + (address)))

/**
 * @brief  Writes float value to SDRAM
 * @param  address: Offset address from starting address of SDRAM
 * @param  value: float value to be saved in SDRAM
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_SDRAM_WriteFloat(address, value)	(*(__IO float *) (SDRAM_START_ADR + (address)) = (value))

/**
 * @brief  Reads float value from SDRAM
 * @param  address: offset address from starting address of SDRAM
 * @retval float value stored at desired location
 * @note   Defined as macro for faster execution
 */
#define TM_SDRAM_ReadFloat(address)			(*(__IO float *) (SDRAM_START_ADR + (address)))

/**
 * @brief  Initialize custom pins callback.
 *         It can be used by user to implement custom pins for application if needed
 * @note   This function is called every time @ref TM_SDRAM_Init() function is called
 * @param  None
 * @retval Initialization status:
 *            - 0: User did not initialize custom pins, default will be used, depending on already supported boards
 *            - > 0: User has initialized custom pins, default pins will be ignored
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
uint8_t TM_SDRAM_InitCustomPinsCallback(void);

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

#endif
