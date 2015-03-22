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
#include "tm_stm32f4_usb_hid_host.h"

/* Private variables */
extern USB_OTG_CORE_HANDLE				USB_OTG_Core;
extern USBH_HOST						USB_Host;
extern TM_USB_HIDHOST_Result_t 			TM_USB_HIDHOST_INT_Result;
extern TM_USB_HIDHOST_Keyboard_t 		TM_USB_HIDHOST_INT_Keyboard;
extern TM_USB_HIDHOST_Mouse_t 			TM_USB_HIDHOST_INT_Mouse;
uint8_t TM_USB_HIDHOST_Initialized = 	0;

void TM_USB_HIDHOST_Init(void) {
	/* Init Host Library */
	USBH_Init(&USB_OTG_Core, 
#ifdef USE_USB_OTG_FS
            USB_OTG_FS_CORE_ID,
#else
            USB_OTG_HS_CORE_ID,
#endif
            &USB_Host,
            &HID_cb, 
            &USR_Callbacks);
	/* We are initialized */
	TM_USB_HIDHOST_Initialized = 1;
}

TM_USB_HIDHOST_Result_t TM_USB_HIDHOST_Process(void) {
	/* Not initialized */
	if (!TM_USB_HIDHOST_Initialized) {
		return TM_USB_HIDHOST_Result_LibraryNotInitialized;
	}
	
	/* Host Task handler */
	USBH_Process(&USB_OTG_Core, &USB_Host);
	
	/* Return device status */
	return TM_USB_HIDHOST_Device();
}

TM_USB_HIDHOST_Result_t TM_USB_HIDHOST_Device(void) {
	/* Not initialized */
	if (!TM_USB_HIDHOST_Initialized) {
		return TM_USB_HIDHOST_Result_LibraryNotInitialized;
	}
	return TM_USB_HIDHOST_INT_Result;
}

TM_USB_HIDHOST_Result_t TM_USB_HIDHOST_ReadKeyboard(TM_USB_HIDHOST_Keyboard_t* Keyboard) {
	if (TM_USB_HIDHOST_INT_Result != TM_USB_HIDHOST_Result_KeyboardConnected) {
		/* Keyboard not initialized */
		return TM_USB_HIDHOST_Result_Error;
	}
	
	/* Fill data */
	Keyboard->Button = TM_USB_HIDHOST_INT_Keyboard.Button;
	Keyboard->ButtonStatus = TM_USB_HIDHOST_INT_Keyboard.ButtonStatus;
	
	/* Reset internal data */
#if USB_HIDHOST_REINITIALIZE_KEYBOARD_AFTER_READ > 0
	TM_USB_HIDHOST_INT_Keyboard.ButtonStatus = TM_USB_HIDHOST_Button_Released;
	TM_USB_HIDHOST_INT_Keyboard.Button = 0;
#endif
	
	/* Return keyboard connected */
	return TM_USB_HIDHOST_Result_KeyboardConnected;
}

TM_USB_HIDHOST_Result_t TM_USB_HIDHOST_ReadMouse(TM_USB_HIDHOST_Mouse_t* Mouse) {
	if (TM_USB_HIDHOST_INT_Result != TM_USB_HIDHOST_Result_MouseConnected) {
		/* Mouse is not connected */
		return TM_USB_HIDHOST_Result_Error;
	}

	/* Fill data */
	Mouse->AbsoluteX =    TM_USB_HIDHOST_INT_Mouse.AbsoluteX;
	Mouse->AbsoluteY =    TM_USB_HIDHOST_INT_Mouse.AbsoluteY;
	Mouse->DiffX =        TM_USB_HIDHOST_INT_Mouse.DiffX;
	Mouse->DiffY =        TM_USB_HIDHOST_INT_Mouse.DiffY;
	Mouse->LeftButton =   TM_USB_HIDHOST_INT_Mouse.LeftButton;
	Mouse->MiddleButton = TM_USB_HIDHOST_INT_Mouse.MiddleButton;
	Mouse->RightButton =  TM_USB_HIDHOST_INT_Mouse.RightButton;
	
	/* Reset internal data */
	/* Difference from last call is 0 */
	TM_USB_HIDHOST_INT_Mouse.DiffX = 0;
	TM_USB_HIDHOST_INT_Mouse.DiffY = 0;
	
	/* Buttons are not pressed any more */
#if USB_HIDHOST_REINITIALIZE_MOUSE_AFTER_READ > 0
	TM_USB_HIDHOST_INT_Mouse.LeftButton = TM_USB_HIDHOST_Button_Released;
	TM_USB_HIDHOST_INT_Mouse.MiddleButton = TM_USB_HIDHOST_Button_Released;
	TM_USB_HIDHOST_INT_Mouse.RightButton = TM_USB_HIDHOST_Button_Released;
#endif
	
	/* Return mouse connected */
	return TM_USB_HIDHOST_Result_MouseConnected;
}

