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
#include "tm_stm32f4_usb_msc_host.h"

/* Private variables */
USB_OTG_CORE_HANDLE     USB_OTG_MSC_Core;
USBH_HOST               USB_Host;
uint8_t                 TM_USB_MSCHOST_INT_Initialized = 0;
TM_USB_MSCHOST_Result_t TM_USB_MSCHOST_INT_Result;

TM_USB_MSCHOST_Result_t TM_USB_MSCHOST_Init(void) {
	/* Set default */
	TM_USB_MSCHOST_INT_Result = TM_USB_MSCHOST_Result_Disconnected;
	/* Init Host Library */
	USBH_Init(	&USB_OTG_MSC_Core,
			#ifdef USE_USB_OTG_FS
				USB_OTG_FS_CORE_ID,
			#else
				USB_OTG_HS_CORE_ID,
			#endif
				&USB_Host,
				&USBH_MSC_cb, 
				&USR_Callbacks);
	
	/* Initialized */
	TM_USB_MSCHOST_INT_Initialized = 1;
	
	/* Process */
	TM_USB_MSCHOST_Process();
	
	/* Is connected already? */
	return TM_USB_MSCHOST_Device();
}

TM_USB_MSCHOST_Result_t TM_USB_MSCHOST_DeInit(void) {
	/* Deinit USB host */
	USBH_DeInit(&USB_OTG_MSC_Core, &USB_Host);
	
	/* Return OK */
	return TM_USB_MSCHOST_Result_Disconnected;
}

void TM_USB_MSCHOST_Process(void) {
	/* if library is initialized */
	if (TM_USB_MSCHOST_INT_Initialized) {
		/* Process */
		USBH_Process(&USB_OTG_MSC_Core, &USB_Host);
	}
}

TM_USB_MSCHOST_Result_t TM_USB_MSCHOST_Device(void) {
	/* Check if library is initialized */
	if (!TM_USB_MSCHOST_INT_Initialized) {
		return TM_USB_MSCHOST_Result_LibraryNotInitialized;
	}
	/* Return status, handled by USB library */
	return TM_USB_MSCHOST_INT_Result;
}

