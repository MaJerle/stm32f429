/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/12/library-44-stm32f4-unique-id-and-flash-size
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Identification library for STM32F4xx devices
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
#ifndef TM_IDENTIFICATION_H
#define TM_IDENTIFICATION_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_ID
 * @brief    Identification library and flash size for STM32F4xx - http://stm32f4-discovery.com/2014/12/library-44-stm32f4-unique-id-and-flash-size
 * @{
 *
 * \par This library allows you to read following things:
 *  - Device's 96bit unique ID
 *  - Device signature 
 *  - Revision
 *  - Flash size
 *
 * \par Revisions possible:
 *  - 0x1000: Revision A
 *  - 0x1001: Revision Z
 *  - 0x1003: Revision Y
 *  - 0x1007: Revision 1
 *  - 0x2001: Revision 3
 *
 * \par Device signatures:
 *  - 0x0413: STM32F405xx/07xx and STM32F415xx/17xx)
 *  - 0x0419: STM32F42xxx and STM32F43xxx
 *  - 0x0423: STM32F401xB/C
 *  - 0x0433: STM32F401xD/E
 *  - 0x0431: STM32F411xC/E
 *
 * \par Flash size:
 *  - Device has stored value of flash size in kB
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
@endverbatim
 */

#include "stm32f4xx.h"

/**
 * @defgroup TM_ID_Macros
 * @brief    Library defines
 *
 * Defines for register locations inside STM32F4xx devices
 * @{
 */

/**
 * @brief Unique ID register address location
 */
#define ID_UNIQUE_ADDRESS		0x1FFF7A10

/**
 * @brief Flash size register address
 */
#define ID_FLASH_ADDRESS		0x1FFF7A22

/**
 * @brief Device ID register address
 */
#define ID_DBGMCU_IDCODE		0xE0042000

 /**
 * @}
 */

/**
 * @defgroup TM_ID_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Get STM32F4xx device signature
 * @note   Defined as macro to get maximal response time
 * @param  None
 * @retval Device signature, bits 11:0 are valid, 15:12 are always 0.
 *           - 0x0413: STM32F405xx/07xx and STM32F415xx/17xx)
 *           - 0x0419: STM32F42xxx and STM32F43xxx
 *           - 0x0423: STM32F401xB/C
 *           - 0x0433: STM32F401xD/E
 *           - 0x0431: STM32F411xC/E
 */
#define TM_ID_GetSignature()	((*(uint16_t *) (ID_DBGMCU_IDCODE)) & 0x0FFF)

/**
 * @brief  Get STM32F4xx device revision
 * @note   Defined as macro to get maximal response time
 * @param  None
 * @retval Device revision value
 *           - 0x1000: Revision A
 *           - 0x1001: Revision Z
 *           - 0x1003: Revision Y
 *           - 0x1007: Revision 1
 *           - 0x2001: Revision 3
 */
#define TM_ID_GetRevision()		(*(uint16_t *) (DBGMCU->IDCODE + 2))

/**
 * @brief  Get STM32F4xx device's flash size in kilo bytes
 * @note   Defined as macro to get maximal response time
 * @param  None
 * @retval Flash size in kilo bytes
 */
#define TM_ID_GetFlashSize()	(*(uint16_t *) (ID_FLASH_ADDRESS))

/**
 * @brief  Get unique ID number in 8-bit format
 * @note   STM32F4xx has 96bits long unique ID, so 12 bytes are available for read in 8-bit format
 * @note   Defined as macro to get maximal response time
 * @param  x: Byte number: specify which part of 8 bits you want to read
 *               - Values between 0 and 11 are allowed
 * @retval Unique ID address
 */
#define TM_ID_GetUnique8(x)		((x >= 0 && x < 12) ? (*(uint8_t *) (ID_UNIQUE_ADDRESS + (x))) : 0)
	
/**
 * @brief  Get unique ID number in 16-bit format
 * @note   STM32F4xx has 96bits long unique ID, so 6 2-bytes values are available for read in 16-bit format
 * @note   Defined as macro to get maximal response time
 * @param  x: Byte number: specify which part of 16 bits you want to read
 *               - Values between 0 and 5 are allowed
 * @retval Unique ID address
 */
#define TM_ID_GetUnique16(x)	((x >= 0 && x < 6) ? (*(uint16_t *) (ID_UNIQUE_ADDRESS + 2 * (x))) : 0)

/**
 * @brief  Get unique ID number in 32-bit format
 * @note   STM32F4xx has 96bits long unique ID, so 3 4-bytes values are available for read in 32-bit format
 * @note   Defined as macro to get maximal response time
 * @param  x: Byte number: specify which part of 16 bits you want to read
 *               - Values between 0 and 2 are allowed
 * @retval Unique ID address
 */
#define TM_ID_GetUnique32(x)	((x >= 0 && x < 3) ? (*(uint32_t *) (ID_UNIQUE_ADDRESS + 4 * (x))) : 0)

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
