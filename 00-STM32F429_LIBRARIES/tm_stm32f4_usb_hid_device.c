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
#include "tm_stm32f4_usb_hid_device.h"

extern USB_OTG_CORE_HANDLE USB_OTG_dev;
extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_INT_Status;

TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_Init(void) {
	/* Initialize HID device */
	USBD_Init(&USB_OTG_dev,
	#ifdef USE_USB_OTG_HS 
			USB_OTG_HS_CORE_ID,
	#else            
			USB_OTG_FS_CORE_ID,
	#endif
			&USR_desc, 
			&USBD_HID_cb, 
			&USR_cb);
	
	/* Set not connected */
	TM_USB_HIDDEVICE_INT_Status = TM_USB_HIDDEVICE_Status_Disconnected;
	
	/* Device not connected */
	return TM_USB_HIDDEVICE_INT_Status;
}

TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_GetStatus(void) {
	/* Return status */
	return TM_USB_HIDDEVICE_INT_Status;
}

/* Mouse */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_MouseStructInit(TM_USB_HIDDEVICE_Mouse_t* Mouse_Data) {
	/* Set defaults */
	Mouse_Data->LeftButton = TM_USB_HIDDEVICE_Button_Released;
	Mouse_Data->RightButton = TM_USB_HIDDEVICE_Button_Released;
	Mouse_Data->MiddleButton = TM_USB_HIDDEVICE_Button_Released;
	Mouse_Data->XAxis = 0;
	Mouse_Data->YAxis = 0;
	Mouse_Data->Wheel = 0;
	
	/* Return currect status */
	return TM_USB_HIDDEVICE_INT_Status;
}

TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_MouseSend(TM_USB_HIDDEVICE_Mouse_t* Mouse_Data) {
	uint8_t buff[5]; /* 5 bytes long report */
	
	/* Check status */
	if (TM_USB_HIDDEVICE_INT_Status != TM_USB_HIDDEVICE_Status_Connected) {
		return TM_USB_HIDDEVICE_Status_Disconnected;
	}
	
	/* Report ID */
	buff[0] = 0x02;	/* Mouse */
	
	/* Set buttons */
	buff[1] = 0;
	buff[1] |= Mouse_Data->LeftButton << 0;		/* Bit 0 */
	buff[1] |= Mouse_Data->RightButton << 1;	/* Bit 1 */
	buff[1] |= Mouse_Data->MiddleButton << 2;	/* Bit 2 */
	
	/* Set X and Y offset */
	buff[2] = Mouse_Data->XAxis;
	buff[3] = Mouse_Data->YAxis;
	
	/* Set wheel */
	buff[4] = Mouse_Data->Wheel;
	
	/* Send to USB */
	USBD_HID_SendReport(&USB_OTG_dev, buff, 5);
	
	/* Return connected */
	return TM_USB_HIDDEVICE_Status_Connected;
}

TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_MouseReleaseAll(void) {
	uint8_t buff[5] = {0, 0, 0, 0, 0}; /* 4 bytes long report */
	
	/* Check status */
	if (TM_USB_HIDDEVICE_INT_Status != TM_USB_HIDDEVICE_Status_Connected) {
		return TM_USB_HIDDEVICE_Status_Disconnected;
	}
	
	/* Report ID */
	buff[0] = 0x02;	/* Mouse */
	
	/* Send to USB to release all buttons and axes */
	USBD_HID_SendReport(&USB_OTG_dev, buff, 5);
	
	/* Return connected */
	return TM_USB_HIDDEVICE_Status_Connected;
}

/* Gamepad */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_GamepadStructInit(TM_USB_HIDDEVICE_Gamepad_t* Gamepad_Data) {
	/* Set defaults */
	Gamepad_Data->Button1 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button2 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button3 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button4 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button5 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button6 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button7 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button8 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button9 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button10 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button11 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button12 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button13 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button14 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button15 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->Button16 = TM_USB_HIDDEVICE_Button_Released;
	Gamepad_Data->LeftXAxis = 0;
	Gamepad_Data->LeftYAxis = 0;
	Gamepad_Data->RightXAxis = 0;
	Gamepad_Data->RightYAxis = 0;
	
	/* Return currect status */
	return TM_USB_HIDDEVICE_INT_Status;
}

TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_GamepadSend(TM_USB_HIDDEVICE_Gamepad_Number_t gamepad_id, TM_USB_HIDDEVICE_Gamepad_t* Gamepad_Data) {
	uint8_t buff[7]; /* 7 bytes long report */
	
	/* Check status */
	if (TM_USB_HIDDEVICE_INT_Status != TM_USB_HIDDEVICE_Status_Connected) {
		return TM_USB_HIDDEVICE_Status_Disconnected;
	}
	
	/* Report ID */
	buff[0] = gamepad_id; /* gamepad id */
	
	/* Buttons pressed, byte 1 */
	buff[1] = 0;
	buff[1] |= Gamepad_Data->Button1 	<< 0;	/* Bit 0 */
	buff[1] |= Gamepad_Data->Button2 	<< 1;	/* Bit 1 */
	buff[1] |= Gamepad_Data->Button3 	<< 2;	/* Bit 2 */
	buff[1] |= Gamepad_Data->Button4 	<< 3;	/* Bit 3 */
	buff[1] |= Gamepad_Data->Button5 	<< 4;	/* Bit 4 */
	buff[1] |= Gamepad_Data->Button6 	<< 5;	/* Bit 5 */
	buff[1] |= Gamepad_Data->Button7 	<< 6;	/* Bit 6 */
	buff[1] |= Gamepad_Data->Button8 	<< 7;	/* Bit 7 */
	
	/* Buttons pressed, byte 2 */
	buff[2] = 0;
	buff[2] |= Gamepad_Data->Button9 	<< 0;	/* Bit 0 */
	buff[2] |= Gamepad_Data->Button10 	<< 1;	/* Bit 1 */
	buff[2] |= Gamepad_Data->Button11 	<< 2;	/* Bit 2 */
	buff[2] |= Gamepad_Data->Button12 	<< 3;	/* Bit 3 */
	buff[2] |= Gamepad_Data->Button13 	<< 4;	/* Bit 4 */
	buff[2] |= Gamepad_Data->Button14 	<< 5;	/* Bit 5 */
	buff[2] |= Gamepad_Data->Button15 	<< 6;	/* Bit 6 */
	buff[2] |= Gamepad_Data->Button16 	<< 7;	/* Bit 7 */
	
	/* Left joystick */
	buff[3] = Gamepad_Data->LeftXAxis;
	buff[4] = Gamepad_Data->LeftYAxis;
	
	/* Right joystick */
	buff[5] = Gamepad_Data->RightXAxis;
	buff[6] = Gamepad_Data->RightYAxis;
	
	/* Send to USB gamepad data */
	USBD_HID_SendReport(&USB_OTG_dev, buff, 7);
	
	/* Return connected */
	return TM_USB_HIDDEVICE_Status_Connected;
}

TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_GamepadReleaseAll(TM_USB_HIDDEVICE_Gamepad_Number_t gamepad_id) {
	uint8_t buff[7] = {0, 0, 0, 0, 0, 0, 0}; /* 7 bytes long report */
	
	/* Check status */
	if (TM_USB_HIDDEVICE_INT_Status != TM_USB_HIDDEVICE_Status_Connected) {
		return TM_USB_HIDDEVICE_Status_Disconnected;
	}
	
	/* Report ID */
	buff[0] = gamepad_id;
	
	/* Send to USB gamepad release all buttons and joysticks */
	USBD_HID_SendReport(&USB_OTG_dev, buff, 7);
	
	/* Return connected */
	return TM_USB_HIDDEVICE_Status_Connected;	
}

/* Keyboard */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_KeyboardStructInit(TM_USB_HIDDEVICE_Keyboard_t* Keyboard_Data) {
	/* Set defaults */
	Keyboard_Data->L_CTRL = TM_USB_HIDDEVICE_Button_Released;
	Keyboard_Data->L_ALT = TM_USB_HIDDEVICE_Button_Released;
	Keyboard_Data->L_SHIFT = TM_USB_HIDDEVICE_Button_Released;
	Keyboard_Data->L_GUI = TM_USB_HIDDEVICE_Button_Released;
	Keyboard_Data->R_CTRL = TM_USB_HIDDEVICE_Button_Released;
	Keyboard_Data->R_ALT = TM_USB_HIDDEVICE_Button_Released;
	Keyboard_Data->R_SHIFT = TM_USB_HIDDEVICE_Button_Released;
	Keyboard_Data->R_GUI = TM_USB_HIDDEVICE_Button_Released;
	Keyboard_Data->Key1 = 0;
	Keyboard_Data->Key2 = 0;
	Keyboard_Data->Key3 = 0;
	Keyboard_Data->Key4 = 0;
	Keyboard_Data->Key5 = 0;
	Keyboard_Data->Key6 = 0;
	
	/* Return currect status */
	return TM_USB_HIDDEVICE_INT_Status;
}

TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_KeyboardSend(TM_USB_HIDDEVICE_Keyboard_t* Keyboard_Data) {
	uint8_t buff[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};; /* 9 bytes long report */
	
	/* Check status */
	if (TM_USB_HIDDEVICE_INT_Status != TM_USB_HIDDEVICE_Status_Connected) {
		return TM_USB_HIDDEVICE_Status_Disconnected;
	}
	
	/* Report ID */
	buff[0] = 0x01; /* Keyboard */
	
	/* Control buttons */
	buff[1] = 0;
	buff[1] |= Keyboard_Data->L_CTRL 	<< 0;	/* Bit 0 */
	buff[1] |= Keyboard_Data->L_SHIFT 	<< 1;	/* Bit 1 */
	buff[1] |= Keyboard_Data->L_ALT 	<< 2;	/* Bit 2 */
	buff[1] |= Keyboard_Data->L_GUI 	<< 3;	/* Bit 3 */
	buff[1] |= Keyboard_Data->R_CTRL 	<< 4;	/* Bit 4 */
	buff[1] |= Keyboard_Data->R_SHIFT 	<< 5;	/* Bit 5 */
	buff[1] |= Keyboard_Data->R_ALT 	<< 6;	/* Bit 6 */
	buff[1] |= Keyboard_Data->R_GUI 	<< 7;	/* Bit 7 */
	
	/* Padding */
	buff[2] = 0x00;
	
	/* Keys */
	buff[3] = Keyboard_Data->Key1;
	buff[4] = Keyboard_Data->Key2;
	buff[5] = Keyboard_Data->Key3;
	buff[6] = Keyboard_Data->Key4;
	buff[7] = Keyboard_Data->Key5;
	buff[8] = Keyboard_Data->Key6;
	
	/* Send to USB */
	USBD_HID_SendReport(&USB_OTG_dev, buff, 9);
	
	/* Return connected */
	return TM_USB_HIDDEVICE_Status_Connected;	
}

TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_KeyboardReleaseAll(void) {
	uint8_t buff[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; /* 9 bytes long report */
	
	/* Check status */
	if (TM_USB_HIDDEVICE_INT_Status != TM_USB_HIDDEVICE_Status_Connected) {
		return TM_USB_HIDDEVICE_Status_Disconnected;
	}	
	
	/* Report ID */
	buff[0] = 0x01; /* Keyboard */
	
	/* Send to USB */
	USBD_HID_SendReport(&USB_OTG_dev, buff, 9);
	
	/* Return connected */
	return TM_USB_HIDDEVICE_Status_Connected;
}

/* Custom report */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_SendCustom(uint8_t* buff, uint8_t count) {
	/* Check status */
	if (TM_USB_HIDDEVICE_INT_Status != TM_USB_HIDDEVICE_Status_Connected) {
		return TM_USB_HIDDEVICE_Status_Disconnected;
	}	
	
	/* Send to USB */
	USBD_HID_SendReport(&USB_OTG_dev, buff, count);
	
	/* Return connected */
	return TM_USB_HIDDEVICE_Status_Connected;
}

