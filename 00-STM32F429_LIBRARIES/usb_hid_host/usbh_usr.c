/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This file includes the user application layer
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbh_usr.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"
#include "tm_stm32f4_usb_hid_host.h"


/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_HID_DEMO_USER_CALLBACKS
* @{
*/

/** @defgroup USBH_USR
* @brief This file is the Header file for usbh_usr.c
* @{
*/ 


/** @defgroup USBH_CORE_Exported_TypesDefinitions
* @{
*/


/**
* @}
*/ 
extern  int16_t  x_loc, y_loc; 
extern __IO int16_t  prev_x, prev_y;

/* TM variable */
TM_USB_HIDHOST_Result_t 	TM_USB_HIDHOST_INT_Result;
TM_USB_HIDHOST_Keyboard_t 	TM_USB_HIDHOST_INT_Keyboard;
TM_USB_HIDHOST_Mouse_t 		TM_USB_HIDHOST_INT_Mouse;

/** @defgroup USBH_USR_Private_Variables
* @{
*/
/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_Callbacks = {
	USBH_USR_Init,
	USBH_USR_DeInit,
	USBH_USR_DeviceAttached,
	USBH_USR_ResetDevice,
	USBH_USR_DeviceDisconnected,
	USBH_USR_OverCurrentDetected,
	USBH_USR_DeviceSpeedDetected,
	USBH_USR_Device_DescAvailable,
	USBH_USR_DeviceAddressAssigned,
	USBH_USR_Configuration_DescAvailable,
	USBH_USR_Manufacturer_String,
	USBH_USR_Product_String,
	USBH_USR_SerialNum_String,
	USBH_USR_EnumerationDone,
	USBH_USR_UserInput,
	NULL,
	USBH_USR_DeviceNotSupported,
	USBH_USR_UnrecoveredError
};

/**
* @brief  USBH_USR_Init 
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void) {
	TM_USB_HIDHOST_INT_Result = TM_USB_HIDHOST_Result_Disconnected;
}

/**
* @brief  USBH_USR_DeviceAttached 
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void) {
	TM_USB_HIDHOST_INT_Result = TM_USB_HIDHOST_Result_Disconnected;
}

/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError(void) {
	TM_USB_HIDHOST_INT_Result = TM_USB_HIDHOST_Result_Error;
}

/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval None
*/
void USBH_USR_DeviceDisconnected (void) {
	TM_USB_HIDHOST_INT_Result = TM_USB_HIDHOST_Result_Disconnected;
}

/**
* @brief  USBH_USR_ResetUSBDevice 
*         Reset USB Device
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void) {
	TM_USB_HIDHOST_INT_Result = TM_USB_HIDHOST_Result_Disconnected;
}


/**
* @brief  USBH_USR_DeviceSpeedDetected 
*         Displays the message on LCD for device speed
* @param  Devicespeed : Device Speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed) {
	if (DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED) {
	} else if (DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED) {
	} else if (DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED) {
	} else {
		TM_USB_HIDHOST_INT_Result = TM_USB_HIDHOST_Result_Error;
	}
}

/**
* @brief  USBH_USR_Device_DescAvailable 
*         Displays the message on LCD for device descriptor
* @param  DeviceDesc : device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc) {
	
}

/**
* @brief  USBH_USR_DeviceAddressAssigned 
*         USB device is successfully assigned the Address 
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void) {
  
}


/**
* @brief  USBH_USR_Conf_Desc 
*         Displays the message on LCD for configuration descriptor
* @param  ConfDesc : Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
   
}

/**
* @brief  USBH_USR_Manufacturer_String 
*         Displays the message on LCD for Manufacturer String 
* @param  ManufacturerString : Manufacturer String of Device
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString) {
	
}

/**
* @brief  USBH_USR_Product_String 
*         Displays the message on LCD for Product String
* @param  ProductString : Product String of Device
* @retval None
*/
void USBH_USR_Product_String(void *ProductString) {

}

/**
* @brief  USBH_USR_SerialNum_String 
*         Displays the message on LCD for SerialNum_String 
* @param  SerialNumString : SerialNum_String of device
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString) {
	
} 

/**
* @brief  EnumerationDone 
*         User response request is displayed to ask for
*         application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void) {
	
} 

/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void) {
	TM_USB_HIDHOST_INT_Result = TM_USB_HIDHOST_Result_DeviceNotSupported;
}


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void) {
	return USBH_USR_RESP_OK;
} 

/**
* @brief  USBH_USR_OverCurrentDetected
*         Device Overcurrent detection event
* @param  None
* @retval None
*/
void USBH_USR_OverCurrentDetected(void) {
	TM_USB_HIDHOST_INT_Result = TM_USB_HIDHOST_Result_Error;
}

/**
* @brief  USR_MOUSE_Init
*         Init Mouse window
* @param  None
* @retval None
*/
void USR_MOUSE_Init(void) {
	static uint8_t firstInit = 1;
	/* Mouse is connected and ready to use */
	TM_USB_HIDHOST_INT_Result = TM_USB_HIDHOST_Result_MouseConnected;
	
	if (firstInit) {
		firstInit = 0;
		
		/* Reset values */
		TM_USB_HIDHOST_INT_Mouse.AbsoluteX = 0;
		TM_USB_HIDHOST_INT_Mouse.AbsoluteY = 0;
		TM_USB_HIDHOST_INT_Mouse.DiffX = 0;
		TM_USB_HIDHOST_INT_Mouse.DiffY = 0;
		TM_USB_HIDHOST_INT_Mouse.LeftButton = TM_USB_HIDHOST_Button_Released;
		TM_USB_HIDHOST_INT_Mouse.RightButton = TM_USB_HIDHOST_Button_Released;
		TM_USB_HIDHOST_INT_Mouse.MiddleButton = TM_USB_HIDHOST_Button_Released;
	}
}

/**
* @brief  USR_MOUSE_ProcessData
*         Process Mouse data
* @param  data : Mouse data to be displayed
* @retval None
*/
void USR_MOUSE_ProcessData(HID_MOUSE_Data_TypeDef *data) {
	static uint8_t mouseButtons[] = {0, 0, 0};
	/* Check X movement */
	if (data->x != 0) {
		TM_USB_HIDHOST_INT_Mouse.DiffX = (int8_t)data->x;
		TM_USB_HIDHOST_INT_Mouse.AbsoluteX += (int8_t)data->x;
	}
	/* Check Y movement */
	if (data->y != 0) {
		TM_USB_HIDHOST_INT_Mouse.DiffY = (int8_t)data->y;
		TM_USB_HIDHOST_INT_Mouse.AbsoluteY += (int8_t)data->y;
	}
	
	/* Process button 1 */
	if (data->button & 0x01) {
		/* Button 1 pressed */
		if (mouseButtons[0] == 0) {
			mouseButtons[0] = 1;
			TM_USB_HIDHOST_INT_Mouse.LeftButton = TM_USB_HIDHOST_Button_Pressed;
		}
	} else {
		/* Button 1 released */
		if (mouseButtons[0] == 1) {
			mouseButtons[0] = 0;
			TM_USB_HIDHOST_INT_Mouse.LeftButton = TM_USB_HIDHOST_Button_Released;
		}
	}
	
	/* Process button 2 */
	if (data->button & 0x02) {
		/* Button 2 pressed */
		if (mouseButtons[1] == 0) {
			mouseButtons[1] = 1;
			TM_USB_HIDHOST_INT_Mouse.RightButton = TM_USB_HIDHOST_Button_Pressed;
		}
	} else {
		/* Button 2 released */
		if (mouseButtons[1] == 1) {
			mouseButtons[1] = 0;
			TM_USB_HIDHOST_INT_Mouse.RightButton = TM_USB_HIDHOST_Button_Released;
		}
	}
	
	/* Process button 3 */
	if (data->button & 0x04) {
		/* Button 3 pressed */
		if (mouseButtons[2] == 0) {
			mouseButtons[2] = 1;
			TM_USB_HIDHOST_INT_Mouse.MiddleButton = TM_USB_HIDHOST_Button_Pressed;
		}
	} else {
		/* Button 3 released */
		if (mouseButtons[2] == 1) {
			mouseButtons[2] = 0;
			TM_USB_HIDHOST_INT_Mouse.MiddleButton = TM_USB_HIDHOST_Button_Released;
		}
	}
}

/**
* @brief  USR_KEYBRD_Init
*         Init Keyboard window
* @param  None
* @retval None
*/
void USR_KEYBRD_Init(void) {
	/* Keyboard is connected */
	TM_USB_HIDHOST_INT_Result = TM_USB_HIDHOST_Result_KeyboardConnected;
	
	/* Reset */
	TM_USB_HIDHOST_INT_Keyboard.ButtonStatus = TM_USB_HIDHOST_Button_Released;
}


/**
* @brief  USR_KEYBRD_ProcessData
*         Process Keyboard data
* @param  data : Keyboard data to be displayed
* @retval None
*/
void USR_KEYBRD_ProcessData(uint8_t data) {
	/* Set keyboard button */
	/* Button has been pressed */
	TM_USB_HIDHOST_INT_Keyboard.ButtonStatus = TM_USB_HIDHOST_Button_Pressed;
	/* Set button value */
	TM_USB_HIDHOST_INT_Keyboard.Button = data;
}

/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void) {
	//TM_USB_HIDHOST_INT_Result = TM_USB_HIDHOST_Result_Disconnected;
}

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/

/**
* @}
*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

