/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */

#if FATFS_USE_SDIO == 1
	#include "fatfs_sd_sdio.h"
#else
	#include "fatfs_sd.h"
#endif



/* Definitions of physical drive number for each media */
#define ATA		0
#define MMC		1
#define USB		2

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	if (pdrv) {
		return STA_NOINIT;
	}
	
#if FATFS_USE_SDIO == 1
	return TM_FATFS_SD_SDIO_disk_initialize();
#else
	return TM_FATFS_SD_disk_initialize();
#endif
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	if (pdrv) {
		return STA_NOINIT;
	}
	
#if FATFS_USE_SDIO == 1
	return TM_FATFS_SD_SDIO_disk_status();
#else
	return TM_FATFS_SD_disk_status();
#endif
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	if (pdrv || !count) {
		return RES_PARERR;		/* Check parameter */
	}

#if FATFS_USE_SDIO == 1
	return TM_FATFS_SD_SDIO_disk_read(buff, sector, count);
#else
	return TM_FATFS_SD_disk_read(buff, sector, count);
#endif
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	if (pdrv || !count) {
		return RES_PARERR;		/* Check parameter */
	}
	
#if FATFS_USE_SDIO == 1
	return TM_FATFS_SD_SDIO_disk_write((uint8_t *)buff, sector, count);
#else
	return TM_FATFS_SD_disk_write(buff, sector, count);
#endif
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	if (pdrv) {
		return RES_PARERR;					/* Check parameter */
	}
#if FATFS_USE_SDIO == 1
	return TM_FATFS_SD_SDIO_disk_ioctl(cmd, buff);
#else
	return TM_FATFS_SD_disk_ioctl(cmd, buff);
#endif
}
#endif

#if FATFS_CUSTOM_FATTIME == 0
DWORD get_fattime (void) {
	/* Returns current time packed into a DWORD variable */
	return	  ((DWORD)(2013 - 1980) << 25)	/* Year 2013 */
			| ((DWORD)7 << 21)				/* Month 7 */
			| ((DWORD)28 << 16)				/* Mday 28 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				/* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}
#endif

