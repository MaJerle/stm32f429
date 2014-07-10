#include "tm_stm32f4_fatfs.h"

FRESULT TM_FATFS_DriveSize(uint32_t* total, uint32_t* free) {
	FATFS *fs;
    DWORD fre_clust;
	FRESULT res;

    // Get volume information and free clusters of drive
    res = f_getfree("0:", &fre_clust, &fs);
    if (res != FR_OK) {
		return res;
	}

    // Get total sectors and free sectors
    *total = (fs->n_fatent - 2) * fs->csize / 2;
    *free = fre_clust * fs->csize / 2;
	
	return FR_OK;
}

