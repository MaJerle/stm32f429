
#include "usb_conf.h"
#include "usbh_usr.h"
#include "diskio.h"
#include "usbh_msc_core.h"

extern uint8_t USBH_deviceOk;
/*--------------------------------------------------------------------------

Module Private Functions and Variables

---------------------------------------------------------------------------*/

static volatile DSTATUS Stat = STA_NOINIT;	/* Disk status */

extern USB_OTG_CORE_HANDLE   USB_OTG_Core;
extern USBH_HOST             USB_Host;

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
                         BYTE drv		/* Physical drive number (0) */
                           )
{
	if (HCD_IsDeviceConnected(&USB_OTG_Core)/* && USBH_deviceOk*/) {  
		FATFS_DEBUG_SEND_USART("disk_initialize: init");
		Stat &= ~STA_NOINIT;
	} else {
		Stat |= STA_NOINIT;
		FATFS_DEBUG_SEND_USART("disk_initialize: not init");
	}

	return Stat;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
                     BYTE drv		/* Physical drive number (0) */
                       )
{
	if (drv) {
		FATFS_DEBUG_SEND_USART("disk_status: not init");
		return STA_NOINIT;
	}
	FATFS_DEBUG_SEND_USART("disk_status: OK");
	return Stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
                   BYTE drv,			/* Physical drive number (0) */
                   BYTE *buff,			/* Pointer to the data buffer to store read data */
                   DWORD sector,		/* Start sector number (LBA) */
                   UINT count			/* Sector count (1..255) */
                     )
{
	BYTE status = USBH_MSC_OK;
	uint32_t timeout;

	if (drv || !count) {
		FATFS_DEBUG_SEND_USART("disk_read: parerr");
		return RES_PARERR;
	}
	if (Stat & STA_NOINIT) {
		FATFS_DEBUG_SEND_USART("disk_read: notrdy");
		return RES_NOTRDY;
	}

	if (HCD_IsDeviceConnected(&USB_OTG_Core)/* && USBH_deviceOk*/) {
		FATFS_DEBUG_SEND_USART("disk_read: connected");
		timeout = FATFS_USB_TIMEOUT;
		do
		{
			status = USBH_MSC_Read10(&USB_OTG_Core, buff, sector, 512 * count);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);

			if (!HCD_IsDeviceConnected(&USB_OTG_Core)) { 
				FATFS_DEBUG_SEND_USART("disk_read: RES_ERROR not connected");
				return RES_ERROR;
			}
			FATFS_DEBUG_SEND_USART("disk_read: While busy");
		} while (status == USBH_MSC_BUSY && timeout);
		
		FATFS_DEBUG_SEND_USART("disk_read: not busy");
	}

	if (status == USBH_MSC_OK) {
		FATFS_DEBUG_SEND_USART("disk_read: RES_OK");
		return RES_OK;
	}
	FATFS_DEBUG_SEND_USART("disk_read: RES_ERROR");
	return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT disk_write (
                    BYTE drv,			/* Physical drive number (0) */
                    const BYTE *buff,	/* Pointer to the data to be written */
                    DWORD sector,		/* Start sector number (LBA) */
                    UINT count			/* Sector count (1..255) */
                      )
{
	BYTE status = USBH_MSC_OK;
	uint32_t timeout;
	if (drv || !count) {
		return RES_PARERR;
	}
	if (Stat & STA_NOINIT) {
		return RES_NOTRDY;
	}
	if (Stat & STA_PROTECT) {
		return RES_WRPRT;
	}



	if (HCD_IsDeviceConnected(&USB_OTG_Core)/* && USBH_deviceOk*/) {
		FATFS_DEBUG_SEND_USART("disk_write: connected");
		
		timeout = FATFS_USB_TIMEOUT;
		do
		{
			status = USBH_MSC_Write10(&USB_OTG_Core, (BYTE*)buff, sector, 512 * count);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);

			if (!HCD_IsDeviceConnected(&USB_OTG_Core)/* || !USBH_deviceOk*/) { 
				FATFS_DEBUG_SEND_USART("disk_write: RES_ERROR busy");
				return RES_ERROR;
			}
			FATFS_DEBUG_SEND_USART("disk_read: While busy");
		} while(status == USBH_MSC_BUSY && timeout);
	}

	if (status == USBH_MSC_OK) {
		FATFS_DEBUG_SEND_USART("disk_write: RES OK");
		return RES_OK;
	}
	FATFS_DEBUG_SEND_USART("disk_write: RES ERRORs");
	return RES_ERROR;
}
#endif /* _READONLY == 0 */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL != 0
DRESULT disk_ioctl (
                    BYTE drv,		/* Physical drive number (0) */
                    BYTE ctrl,		/* Control code */
                    void *buff		/* Buffer to send/receive control data */
                      )
{
	DRESULT res = RES_OK;

	if (drv) {
		return RES_PARERR;
	}

	//res = RES_ERROR;

	if (Stat & STA_NOINIT) {
		return RES_NOTRDY;
	}

	switch (ctrl) {
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
#endif /* _USE_IOCTL != 0 */

DWORD get_fattime(void) {
	return 0;
}

