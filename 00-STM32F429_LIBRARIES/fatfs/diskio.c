/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "ff.h"

/* Not USB in use */
/* Define it in defines.h project file if you want to use USB */
#ifndef FATFS_USE_USB
	#define FATFS_USE_USB				0
#endif

/* Set in defines.h file if you want it */
#ifndef TM_FATFS_CUSTOM_FATTIME
	#define TM_FATFS_CUSTOM_FATTIME		0
#endif

/* Defined in defines.h */
/* We are using FATFS with USB */
#if FATFS_USE_USB == 1
	/* If SDIO is not defined, set to 2, to disable SD card */
	/* You can set FATFS_USE_SDIO in defines.h file */
	/* This is for error fixes */
	#ifndef FATFS_USE_SDIO
		#define FATFS_USE_SDIO			2
	#endif
#else
	/* If USB is not used, set default settings back */
	/* By default, SDIO is used */
	#ifndef FATFS_USE_SDIO
		#define FATFS_USE_SDIO			1
	#endif
#endif

#if FATFS_USE_USB == 1
	#include "fatfs_usb.h"
#endif 	/* FATFS_USE_USB */

/* Include SD card files if is enabled */
#if FATFS_USE_SDIO == 1
	#include "fatfs_sd_sdio.h"
#elif FATFS_USE_SDIO == 0
	#include "fatfs_sd.h"
#endif


/* Definitions of physical drive number for each media */
#define ATA		0
#define USB		1

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS status = STA_NOINIT;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			#if FATFS_USE_SDIO == 1
				status = TM_FATFS_SD_SDIO_disk_initialize();	/* SDIO communication */
			#elif FATFS_USE_SDIO == 0
				status = TM_FATFS_SD_disk_initialize();			/* SPI communication */
			#endif
			break;
		case USB:	/* USB storage */
			#if FATFS_USE_USB == 1
				status = TM_FATFS_USB_disk_initialize();			/* USB */
			#endif
			break;
		default:
			status = STA_NOINIT;
	}
	
	return status;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{

	DSTATUS status = STA_NOINIT;
	
	switch (pdrv) {
		case ATA:	/* SD CARD */
			#if FATFS_USE_SDIO == 1
				status = TM_FATFS_SD_SDIO_disk_status();	/* SDIO communication */
			#elif FATFS_USE_SDIO == 0
				status = TM_FATFS_SD_disk_status();		/* SPI communication */
			#endif
			break;
		case USB:	/* USB storage */
			#if FATFS_USE_USB == 1
				status = TM_FATFS_USB_disk_status();				/* USB */
			#endif
			break;
		default:
			status = STA_NOINIT;
	}
	
	return status;
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
	DRESULT status = RES_PARERR;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			#if FATFS_USE_SDIO == 1
				status = TM_FATFS_SD_SDIO_disk_read(buff, sector, count);	/* SDIO communication */
			#elif FATFS_USE_SDIO == 0
				status = TM_FATFS_SD_disk_read(buff, sector, count);		/* SPI communication */
			#endif
			break;
		case USB:	/* USB storage */
			#if FATFS_USE_USB == 1
				status = TM_FATFS_USB_disk_read(buff, sector, count);			/* USB */
			#endif
			break;
		default:
			status = RES_PARERR;
	}
	
	return status;
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
	DRESULT status = RES_PARERR;
	if (!count) {
		return RES_PARERR;		/* Check parameter */
	}
	
	switch (pdrv) {
		case ATA:	/* SD CARD */
			#if FATFS_USE_SDIO == 1
				status = TM_FATFS_SD_SDIO_disk_write((BYTE *)buff, sector, count);	/* SDIO communication */
			#elif FATFS_USE_SDIO == 0
				status = TM_FATFS_SD_disk_write(buff, sector, count);				/* SPI communication */
			#endif
			break;
		case USB:	/* USB storage */
			#if FATFS_USE_USB == 1
				status = TM_FATFS_USB_disk_write(buff, sector, count);					/* USB */
			#endif
			break;
		default:
			status = RES_PARERR;
	}
	
	return status;
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
	DRESULT status = RES_PARERR;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			#if FATFS_USE_SDIO == 1
				status = TM_FATFS_SD_SDIO_disk_ioctl(cmd, buff);					/* SDIO communication */
			#elif FATFS_USE_SDIO == 0
				status = TM_FATFS_SD_disk_ioctl(cmd, buff);							/* SPI communication */
			#endif
			break;
		case USB:	/* USB storage */
			#if FATFS_USE_USB == 1
				status = TM_FATFS_USB_disk_ioctl(cmd, buff);						/* USB */
			#endif
			break;
		default:
			status = RES_PARERR;
	}
	return status;
}
#endif

__weak DWORD get_fattime(void) {
	/* Returns current time packed into a DWORD variable */
	return	  ((DWORD)(2013 - 1980) << 25)	/* Year 2013 */
			| ((DWORD)7 << 21)				/* Month 7 */
			| ((DWORD)28 << 16)				/* Mday 28 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				/* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}


