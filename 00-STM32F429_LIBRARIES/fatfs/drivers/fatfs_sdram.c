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
#include "fatfs_sdram.h"

/* Status for SDRAM */
static volatile DSTATUS SDRAM_Status = STA_NOINIT;

DSTATUS TM_FATFS_SDRAM_disk_initialize(void) {
	/* Init SDRAM */
	if (!TM_SDRAM_Init()) {
		/* Set NOINIT flag */
		SDRAM_Status |= STA_NOINIT;
		
		/* Return error */
		return STA_NODISK;
	}
	
	/* Clear NOINIT flag */
	SDRAM_Status &= ~STA_NOINIT;
	
	/* Return status */
	return SDRAM_Status;
}

DSTATUS TM_FATFS_SDRAM_disk_status(void) {
	/* Return disk status */
	return SDRAM_Status;
}

DRESULT TM_FATFS_SDRAM_disk_ioctl(BYTE cmd, void* buff) {
	/* Get command */
	switch (cmd) {
		case GET_SECTOR_COUNT:	/* Get drive capacity in unit of sector (DWORD) */
			*(WORD *)buff = SDRAM_MEMORY_SIZE / FATFS_SDRAM_SECTOR_SIZE;
			break;
		case GET_BLOCK_SIZE:	/* Get erase block size in unit of sector (DWORD) */
			*(DWORD *)buff = 32;
			break;
		case CTRL_SYNC:
		case CTRL_ERASE_SECTOR:
			break;
		default:
			break;
	}
	
	/* Return OK */
	return RES_OK;
}

DRESULT TM_FATFS_SDRAM_disk_read(BYTE *buff, DWORD sector, UINT count) {
	DWORD start, cnt;
	
	/* If not initialized */
	if (SDRAM_Status & STA_NOINIT) {
		return RES_NOTRDY;
	}
	
	/* Start address of reading */
	start = sector * FATFS_SDRAM_SECTOR_SIZE;

	/* Get number of elements to read */
	cnt = count * FATFS_SDRAM_SECTOR_SIZE;
	
	/* Read data from external ram */
	memcpy((uint8_t *)buff, (uint8_t *)(SDRAM_START_ADR + start), cnt);
	
	/* Return OK */
	return RES_OK;
}

DRESULT TM_FATFS_SDRAM_disk_write(const BYTE *buff, DWORD sector, UINT count) {
	DWORD start, cnt;
	
	/* If not initialized */
	if (SDRAM_Status & STA_NOINIT) {
		return RES_NOTRDY;
	}
	
	/* Start address of reading */
	start = sector * FATFS_SDRAM_SECTOR_SIZE;

	/* Get number of elements to read */
	cnt = count * FATFS_SDRAM_SECTOR_SIZE;
	
	/* Read data from external ram */
	memcpy((uint8_t *)(SDRAM_START_ADR + start), (uint8_t *)buff, cnt);

	/* Return OK */
	return RES_OK;
}
