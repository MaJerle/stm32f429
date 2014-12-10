/**
 *	Identification library for STM32F4
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/12/library-44-stm32f4-unique-id-and-flash-size
 *	@version 	v1.0
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
 * This library allows you to read following things:
 *	- Device's 96bit unique ID
 *	- Device signature 
 *	- Revision
 *	- Flash size
 *
 * Revisions possible:
 *	- 0x1000: Revision A
 *	- 0x1001: Revision Z
 *	- 0x1003: Revision Y
 *	- 0x1007: Revision 1
 *	- 0x2001: Revision 3
 *
 * Device signatures:
 *	- 0x0413: STM32F405xx/07xx and STM32F415xx/17xx)
 *	- 0x0419: STM32F42xxx and STM32F43xxx
 *	- 0x0423: STM32F401xB/C
 *	- 0x0433: STM32F401xD/E
 *	- 0x0431: STM32F411xC/E
 *
 * Flash size:
 * 	- Device has stored value of flash size in kB
 */
#ifndef TM_IDENTIFICATION_H
#define TM_IDENTIFICATION_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 */
/**
 * Includes
 */
#include "stm32f4xx.h"

/* Addresses in device */
#define ID_UNIQUE_ADDRESS		0x1FFF7A10
#define ID_FLASH_ADDRESS		0x1FFF7A22
#define ID_DBGMCU_IDCODE		0xE0042000

/**
 * With this "function" you are able to get signature of device.
 *
 * Possible returns:
 * 	- 0x0413: STM32F405xx/07xx and STM32F415xx/17xx)
 *	- 0x0419: STM32F42xxx and STM32F43xxx
 *	- 0x0423: STM32F401xB/C
 *	- 0x0433: STM32F401xD/E
 *	- 0x0431: STM32F411xC/E
 *
 * Returned data is in 16-bit mode, but only bits 11:0 are valid, bits 15:12 are always 0.
 * Defined as macro
 */
#define TM_ID_GetSignature()	((*(uint16_t *) (ID_DBGMCU_IDCODE)) & 0x0FFF)

/**
 * With this "function" you are able to get revision of device.
 *
 * Revisions possible:
 *	- 0x1000: Revision A
 *	- 0x1001: Revision Z
 *	- 0x1003: Revision Y
 *	- 0x1007: Revision 1
 *	- 0x2001: Revision 3
 *
 * Returned data is in 16-bit mode.
 * Defined as macro
 */
#define TM_ID_GetRevision()		(*(uint16_t *) (ID_DBGMCU_IDCODE + 2))

/**
 * With this "function" you are able to get flash size of device.
 *
 * Returned data is in 16-bit mode, returned value is flash size in kB (kilo bytes).
 * Defined as macro
 */
#define TM_ID_GetFlashSize()	(*(uint16_t *) (ID_FLASH_ADDRESS))

/**
 * With this "function" you are able to get unique ID of device in 8-bit (byte) read mode.
 * Unique ID is 96bit long, but if you need just some parts of it, you can read them with 8bit function.
 *
 * Parameters:
 * 	- uint8_t x:
 * 		Value between 0 and 11, corresponding to byte you want to read from 96bits (12bytes)
 *
 * Returned data is 8-bit
 * Defined as macro
 */
#define TM_ID_GetUnique8(x)		((x >= 0 && x < 12) ? (*(uint8_t *) (ID_UNIQUE_ADDRESS + (x))) : 0)
	
/**
 * With this "function" you are able to get unique ID of device in 16-bit (byte) read mode.
 * Unique ID is 96bit long, but if you need just some parts of it, you can read them with 16bit function.
 *
 * Parameters:
 * 	- uint8_t x:
 * 		Value between 0 and 5, corresponding to 2-bytes you want to read from 96bits (12bytes)
 *
 * Returned data is 16-bit
 * Defined as macro
 */
#define TM_ID_GetUnique16(x)	((x >= 0 && x < 6) ? (*(uint16_t *) (ID_UNIQUE_ADDRESS + 2 * (x))) : 0)

/**
 * With this "function" you are able to get unique ID of device in 32-bit (byte) read mode.
 * Unique ID is 96bit long, but if you need just some parts of it, you can read them with 32bit function.
 *
 * Parameters:
 * 	- uint8_t x:
 * 		Value between 0 and 2, corresponding to 4-bytes you want to read from 96bits (12bytes)
 *
 * Returned data is 32-bit
 * Defined as macro
 */
#define TM_ID_GetUnique32(x)	((x >= 0 && x < 3) ? (*(uint32_t *) (ID_UNIQUE_ADDRESS + 4 * (x))) : 0)

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
