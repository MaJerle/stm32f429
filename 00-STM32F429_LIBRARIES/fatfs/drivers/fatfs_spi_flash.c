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
#include "fatfs_spi_flash.h"

/* Status for SPIFLASH */
static volatile DSTATUS SPI_FLASH_Status = STA_NOINIT;

DSTATUS TM_FATFS_SPI_FLASH_disk_initialize(void) {
	/* Clear NOINIT flag */
	//SPI_FLASH_Status &= ~STA_NOINIT;
	
	/* Return status */
	return SPI_FLASH_Status;
}

DSTATUS TM_FATFS_SPI_FLASH_disk_status(void) {
	/* Return disk status */
	return SPI_FLASH_Status;
}

DRESULT TM_FATFS_SPI_FLASH_disk_ioctl(BYTE cmd, void *buff) {
	/* Get command */
	switch (cmd) {
		case GET_SECTOR_COUNT:	/* Get drive capacity in unit of sector (DWORD) */
			//*(WORD *)buff = SPI_FLASH_MEMORY_SIZE / FATFS_SPI_FLASH_SECTOR_SIZE;
			break;
		case GET_BLOCK_SIZE:	/* Get erase block size in unit of sector (DWORD) */
			//*(DWORD *)buff = 32;
			break;
		case CTRL_SYNC:
		case CTRL_ERASE_SECTOR:
			break;
		default:
			break;
	}
	
	/* Return ERROR */
	return RES_ERROR;
}

DRESULT TM_FATFS_SPI_FLASH_disk_read(BYTE *buff, DWORD sector, UINT count) {
	/* Future use */
	return RES_ERROR;
}

DRESULT TM_FATFS_SPI_FLASH_disk_write(const BYTE *buff, DWORD sector, UINT count) {
	/* Future use */
	return RES_ERROR;
}
