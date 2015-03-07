/**
 *	USB HID HOST library for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/08/library-31-usb-hid-host-for-stm32f4xx-devices
 *	@version 	v1.0
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 *	
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
 *
 * This library allows you to operate with USB keyboard and USB mouse devices
 *
 * It works in USB FS or USB HS in FS mode.
 *
 * By default, library works in USB FS mode (for STM32F4-Discovery board).
 * If you want to use this on STM32F429-Discovery board, you have to activate USB HS in FS mode.
 * Activate this with lines below in your defines.h file:
 *
 * 	//Activate USB HS in FS mode
 * 	#define USE_USB_OTG_HS
 *
 * Pinout (can not be changed)
 *
 * 	USB			|STM32F4xx FS mode				|STM32F4xx HS in FS mode	|Notes
 * 				|STM32F4-Discovery				|STM32F429-Discovery
 *
 * 	Data +		PA12							PB15						Data+ for USB, standard and used pin
 * 	Data -		PA11							PB14						Data- for USB, standard and used pin
 * 	ID			PA10							PB12						ID pin, used on F4 and F429 discovery boards, not needed if you don't like it
 *	VBUS		PA9								PB13						VBUS pin, used on F4 and F429 discovery board for activating USB chip.
 *																			You have to use VBUS on discovery boards, but on nucleo, it's ok with only Data+ and Data- pins
 * Disable necessary pins
 *
 * USB technically needs only Data+ and Data- pins.
 * Also, ID pin can be used, but it is not needed.
 *
 * Disable ID PIN
 *
 * If you need pin for something else, where ID is located, you can disable this pin for USB.
 * Add lines below in your defines.h file:
 *
 * 	//Disable ID pin
 * 	#define USB_HID_HOST_DISABLE_ID
 *
 * Disable VBUS PIN
 *
 * VBUS pin is located on Discovery boards, to activate USB chip on board.
 * If you are working with Discovery boards, then you need this pin, otherise USB will not work.
 * But if you are working on other application (or Nucleo board), you only need Data+ and Data- pins.
 * To disable VBUS pin, add lines below in your defines.h file:
 *
 * 	//Disable VBUS pin
 * 	#define USB_HID_HOST_DISABLE_VBUS
 *	
 * Default configuration for keyboard is AZERTY style. If you want to use QWERTY keyboard style,
 * then add line below in defines.h to activate it.
 *
 *	//Enable QUERTY keyboard style
 *	#define QWERTY_KEYBOARD
 * */
#ifndef TM_USB_HID_HOST_H
#define TM_USB_HID_HOST_H 100
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - defines.h
 * - USB HID stack
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"

#include "usb_bsp.h"
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_hid_core.h"

/* Reinitialize mouse values after read */
#ifndef USB_HIDHOST_REINITIALIZE_MOUSE_AFTER_READ
#define USB_HIDHOST_REINITIALIZE_MOUSE_AFTER_READ		0
#endif

/* Reiintialize keyboard values after read */
#ifndef USB_HIDHOST_REINITIALIZE_KEYBOARD_AFTER_READ
#define USB_HIDHOST_REINITIALIZE_KEYBOARD_AFTER_READ	0
#endif

/**
 * HID Host result enumeration
 *
 * Parameters:
 * 	- TM_USB_HIDHOST_Result_Error
 * 		An error occured
 * 	- TM_USB_HIDHOST_Result_KeyboardConnected
 * 		Keyboard is connected and ready to use
 * 	- TM_USB_HIDHOST_Result_MouseConnected
 * 		Mouse is connected and ready to use
 * 	- TM_USB_HIDHOST_Result_Disconnected
 * 		Device is not connected
 * 	- TM_USB_HIDHOST_Result_DeviceNotSupported
 * 		Device is not supported
 * 	- TM_USB_HIDHOST_Result_LibraryNotInitialized
 * 		Library is not initialized
 */
typedef enum {
	TM_USB_HIDHOST_Result_Error,
	TM_USB_HIDHOST_Result_KeyboardConnected,
	TM_USB_HIDHOST_Result_MouseConnected,
	TM_USB_HIDHOST_Result_Disconnected,
	TM_USB_HIDHOST_Result_DeviceNotSupported,
	TM_USB_HIDHOST_Result_LibraryNotInitialized
} TM_USB_HIDHOST_Result_t;

/**
 * HID Host Button enumeration
 *
 * Parameters:
 * 	- TM_USB_HIDHOST_Button_Pressed
 * 		Button was pressed
 * 	- TM_USB_HIDHOST_Button_Released
 * 		Button was released
 */
typedef enum {
	TM_USB_HIDHOST_Button_Pressed = 0,
	TM_USB_HIDHOST_Button_Released
} TM_USB_HIDHOST_Button_t;

/**
 * HID Host keyboard struct
 *
 * Parameters:
 * 	- TM_USB_HIDHOST_Button_t ButtonStatus
 * 		Indicates if button is pressed or released
 * 	- uint8_t Button
 * 		Button number pressed
 */
typedef struct {
	TM_USB_HIDHOST_Button_t ButtonStatus;
	uint8_t Button;
} TM_USB_HIDHOST_Keyboard_t;

/**
 * HID Host mouse struct
 *
 * Parameters:
 * 	- int16_t AbsoluteX
 * 		Absolute cursor X position
 * 	- int16_t AbsoluteY
 * 		Absolute cursor Y position
 * 	- int16_t DiffX
 * 		Difference cursor X position from last check
 * 	- int16_t DiffY
 * 		Difference cursor Y position from last check
 * 	- TM_USB_HIDHOST_Button_t LeftButton
 * 		Indicates if left button is pressed or released
 * 	- TM_USB_HIDHOST_Button_t RightButton
 * 		Indicates if right button is pressed or released
 * 	- TM_USB_HIDHOST_Button_t MiddleButton
 * 		Indicates if middle button is pressed or released
 */
typedef struct {
	/* Cursor movement */
	int16_t AbsoluteX;
	int16_t AbsoluteY;
	int16_t DiffX;
	int16_t DiffY;
	
	/* Buttons */
	TM_USB_HIDHOST_Button_t LeftButton;
	TM_USB_HIDHOST_Button_t RightButton;
	TM_USB_HIDHOST_Button_t MiddleButton;
} TM_USB_HIDHOST_Mouse_t;

/**
 * Initialize USB HID Host
 *
 *
 */
extern void TM_USB_HIDHOST_Init(void);

/**
 * Process USB HID library
 *
 * This function has to be called periodically, as fast as possible
 *
 * Returns member of TM_USB_HIDHOST_Result_t typedef
 */
extern TM_USB_HIDHOST_Result_t TM_USB_HIDHOST_Process(void);

/**
 * Checks device status
 *
 * Returns member of TM_USB_HIDHOST_Result_t typedef
 */
extern TM_USB_HIDHOST_Result_t TM_USB_HIDHOST_Device(void);

/**
 * Read keyboard data
 *
 * Parameters:
 * 	- TM_USB_HIDHOST_Keyboard_t* Keyboard
 * 		Pointer to TM_USB_HIDHOST_Keyboard_t keyboard struct
 *
 * Returns TM_USB_HIDHOST_Result_KeyboardConnected if keyboard is connected
 */
extern TM_USB_HIDHOST_Result_t TM_USB_HIDHOST_ReadKeyboard(TM_USB_HIDHOST_Keyboard_t* Keyboard);

/**
 * Read mouse data
 *
 * Parameters:
 * 	- TM_USB_HIDHOST_Mouse_t* Mouse
 * 		Pointer to TM_USB_HIDHOST_Mouse_t mouse struct
 *
 * Returns TM_USB_HIDHOST_Result_MouseConnected if mouse is connected
 */
extern TM_USB_HIDHOST_Result_t TM_USB_HIDHOST_ReadMouse(TM_USB_HIDHOST_Mouse_t* Mouse);

#endif

