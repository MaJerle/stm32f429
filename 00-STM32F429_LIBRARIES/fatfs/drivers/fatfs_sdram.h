/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Library template 
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
#ifndef TM_FATFS_SDRAM_H
#define TM_FATFS_SDRAM_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_FATFS_SDRAM
 * @brief    Library description here
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
 - defines.h
 - TM SDRAM
@endverbatim
 */

#include "stm32f4xx.h"
#include "defines.h"
#include "tm_stm32f4_sdram.h"

/* FATFS functions */
#include "diskio.h"

/**
 * @defgroup TM_FATFS_SDRAM_Macros
 * @brief    Library defines
 * @{
 */

#ifndef FATFS_SDRAM_SECTOR_SIZE
#define FATFS_SDRAM_SECTOR_SIZE    512
#endif

/**
 * @}
 */
 
/**
 * @defgroup TM_FATFS_SDRAM_Typedefs
 * @brief    Library Typedefs
 * @{
 */
/* Typedefs here */
/**
 * @}
 */

/**
 * @defgroup TM_FATFS_SDRAM_Functions
 * @brief    Library Functions - Low level functions for FATFS implementation
 * @{
 */

DSTATUS TM_FATFS_SDRAM_disk_initialize(void);
DSTATUS TM_FATFS_SDRAM_disk_status(void);
DRESULT TM_FATFS_SDRAM_disk_ioctl(BYTE cmd, void *buff);
DRESULT TM_FATFS_SDRAM_disk_read(BYTE *buff, DWORD sector, UINT count);
DRESULT TM_FATFS_SDRAM_disk_write(const BYTE *buff, DWORD sector, UINT count);

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
