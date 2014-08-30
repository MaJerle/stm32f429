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
#include "usb_conf.h"
#include "usbh_usr.h"
#include "fatfs_usb.h"
#include "usbh_msc_core.h"
#include "tm_stm32f4_usb_msc_host.h"

static volatile DSTATUS USB_Stat = STA_NOINIT;	/* Disk status */
extern TM_USB_MSCHOST_Result_t 	TM_USB_MSCHOST_INT_Result;

extern USB_OTG_CORE_HANDLE   USB_OTG_Core;
extern USBH_HOST             USB_Host;

/*-----------------------------------------------------------------------*/
/* Initialize USB                                                        */
/*-----------------------------------------------------------------------*/
DSTATUS TM_FATFS_USB_disk_initialize(void) {
	if (HCD_IsDeviceConnected(&USB_OTG_Core) && TM_USB_MSCHOST_INT_Result == TM_USB_MSCHOST_Result_Connected) {
		USB_Stat &= ~STA_NOINIT;
	} else {
		USB_Stat |= STA_NOINIT;
	}

	return USB_Stat;
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/
DSTATUS TM_FATFS_USB_disk_status(void) {
	return USB_Stat;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT TM_FATFS_USB_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	BYTE status = USBH_MSC_OK;

	if (!count) {
		return RES_PARERR;
	}
	if (USB_Stat & STA_NOINIT) {
		return RES_NOTRDY;
	}

	if (HCD_IsDeviceConnected(&USB_OTG_Core) && TM_USB_MSCHOST_INT_Result == TM_USB_MSCHOST_Result_Connected) {
		do
		{
			status = USBH_MSC_Read10(&USB_OTG_Core, buff, sector, 512 * count);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);

			if (!HCD_IsDeviceConnected(&USB_OTG_Core)) { 
				return RES_ERROR;
			}
		} while (status == USBH_MSC_BUSY);
	}

	if (status == USBH_MSC_OK) {
		return RES_OK;
	}
	return RES_ERROR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT TM_FATFS_USB_disk_write (
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	BYTE status = USBH_MSC_OK;
	if (!count) {
		return RES_PARERR;
	}
	if (USB_Stat & STA_NOINIT) {
		return RES_NOTRDY;
	}
	if (TM_USB_MSCHOST_INT_Result == TM_USB_MSCHOST_Result_WriteProtected) {
		return RES_WRPRT;
	}

	if (HCD_IsDeviceConnected(&USB_OTG_Core) && TM_USB_MSCHOST_INT_Result == TM_USB_MSCHOST_Result_Connected) {
		do
		{
			status = USBH_MSC_Write10(&USB_OTG_Core, (BYTE*)buff, sector, 512 * count);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);

			if (!HCD_IsDeviceConnected(&USB_OTG_Core)) {
				return RES_ERROR;
			}
		} while (status == USBH_MSC_BUSY);
	}

	if (status == USBH_MSC_OK) {
		return RES_OK;
	}
	return RES_ERROR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT TM_FATFS_USB_disk_ioctl (
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;

	if (USB_Stat & STA_NOINIT) {
		return RES_NOTRDY;
	}

	switch (cmd) {
		case CTRL_SYNC :		/* Make sure that no pending write process */
			res = RES_OK;
			break;

		case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
			*(DWORD*)buff = (DWORD) USBH_MSC_Param.MSCapacity;
			res = RES_OK;
			break;

		case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
			*(WORD*)buff = 512;
			res = RES_OK;
			break;

		case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
			*(DWORD*)buff = 512;
			break;

		default:
			res = RES_PARERR;
	}

	return res;
}
#endif

