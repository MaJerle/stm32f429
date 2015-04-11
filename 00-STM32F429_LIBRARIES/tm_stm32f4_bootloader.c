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
#include "tm_stm32f4_bootloader.h"

/* New type for function call */
typedef void (*Bootloader_Function)(void);
Bootloader_Function Jump_To_Application;

uint8_t TM_BOOTLOADER_JumpToProgram(uint32_t program_address) {
	/* Test if user code is programmed starting from address "program_address" */
    if (((*(__IO uint32_t *)program_address) & 0x2FFE0000 ) == 0x20000000) { 
		/* Set FLASH vector table */
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, program_address);
		/* Jump to user application */
		Jump_To_Application = (Bootloader_Function) *(__IO uint32_t*) (program_address + 4);
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t *) program_address);
		/* Jump to application */
		Jump_To_Application();
		
		/* Return 1 = This should never happen */
		return 1;
    }
	
	/* Return error in case we didn't jump */
	return 0;
}

