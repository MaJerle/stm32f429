/**	
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
 */
#include "tm_stm32f4_fatfs.h"

FRESULT TM_FATFS_DriveSize(uint32_t* total, uint32_t* free) {
	FATFS *fs;
    DWORD fre_clust;
	FRESULT res;

    /* Get volume information and free clusters of drive */
    res = f_getfree("0:", &fre_clust, &fs);
    if (res != FR_OK) {
		return res;
	}

    /* Get total sectors and free sectors */
    *total = (fs->n_fatent - 2) * fs->csize / 2;
    *free = fre_clust * fs->csize / 2;
	
	/* Return OK */
	return FR_OK;
}

FRESULT TM_FATFS_USBDriveSize(uint32_t* total, uint32_t* free) {
	FATFS *fs;
    DWORD fre_clust;
	FRESULT res;

    /* Get volume information and free clusters of drive */
    res = f_getfree("1:", &fre_clust, &fs);
    if (res != FR_OK) {
		return res;
	}

    /* Get total sectors and free sectors */
    *total = (fs->n_fatent - 2) * fs->csize / 2;
    *free = fre_clust * fs->csize / 2;
	
	/* Return OK */
	return FR_OK;
}

FRESULT TM_FATFS_TruncateBeginning(FIL* fil, uint32_t index) {
	uint8_t Buffer[FATFS_TRUNCATE_BUFFER_SIZE];			/* Buffer for temporary data */

	uint32_t FileSize = f_size(fil);					/* Size of file */
	uint32_t ReadIndex = index;							/* Starting read index */
	uint32_t WriteIndex = 0;							/* We have to write at beginning */
	uint32_t TotalSize = FileSize - ReadIndex;			/* New file size after truncate */
	uint32_t NewSize = TotalSize;						/* Save new file size */
	uint32_t BlockSize;									/* Block size for read operation */
	uint32_t Read;										/* Read bytes */
	uint32_t Written;									/* Written bytes */
	FRESULT fr;											/* Result typedef */
	
	/* Index is 0 or file is empty, nothing to do */
	if (index == 0 || FileSize == 0) {
		return FR_OK;
	}
	
	/* Check if index is more than file size, truncate all */
	if (index > FileSize) {
		fr = f_lseek(fil, 0);								/* Go to beginning */
		if (fr) return fr;									/* Check for success */
		return f_truncate(fil);								/* Truncate file from new end to actual end */
	}
	
	/* Until we have available data in file after user specific index */
	while (TotalSize > 0) {
		/* Calculate new block size for new read operation */
		BlockSize = (TotalSize > FATFS_TRUNCATE_BUFFER_SIZE) ? (FATFS_TRUNCATE_BUFFER_SIZE) : (TotalSize);
	
		fr = f_lseek(fil, ReadIndex);						/* Go to the read index */
		if (fr) return fr;									/* Check for success */
		fr = f_read(fil, &Buffer, BlockSize, &Read);		/* Read data */
		if (fr) return fr;									/* Check for success */

		fr = f_lseek(fil, WriteIndex);						/* Go back to the write index */
		if (fr) return fr;									/* Check for success */
		fr = f_write(fil, &Buffer, BlockSize, &Written);/* Write data */
		if (fr) return fr;									/* Check for success */

		TotalSize -= BlockSize;								/* Calculate new total size we have more to move everything */
		ReadIndex += Read;									/* Calculate new read pointer */
		WriteIndex += Written;								/* Calculate new write pointer */
	}
	
	fr = f_lseek(fil, NewSize);								/* Move pointer to the "end" of new file */
	if (fr) return fr;										/* Check for success */
	fr =  f_truncate(fil);									/* Truncate file from new end to actual end */
	return f_lseek(fil, 0);									/* Move pointer to the beginning */
}
