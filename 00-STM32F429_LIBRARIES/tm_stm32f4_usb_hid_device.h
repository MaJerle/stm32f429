/**
 *	USB HID Device library for STM32F4
 *	
 *	With this library, STM32F4 will be shown to computer as USB Keyboard, Mouse and 2 Gamepads at same time
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/09/library-34-stm32f4-usb-hid-device
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
 */
#ifndef TM_USB_HIDDEVICE_H
#define TM_USB_HIDDEVICE_H 100
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx EXTI
 * - misc.h
 * - defines.h
 * - USB HID Device
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "defines.h"

#include  "usbd_hid_core.h"
#include  "usbd_usr.h"
#include  "usbd_desc.h"

/**
 * Current USB HID status
 * 
 * Parameters:
 * 	- TM_USB_HIDDEVICE_Status_LibraryNotInitialized
 * 		Library is not initialized yet
 * 	- TM_USB_HIDDEVICE_Status_Connected
 * 		Device is connected and ready to use
 * 	- TM_USB_HIDDEVICE_Status_Disconnected
 * 		Device is not connected 
 * 	- TM_USB_HIDDEVICE_Status_IdleMode
 * 		Device is is IDLE mode
 * 	- TM_USB_HIDDEVICE_Status_SuspendMode
 * 		Device is in suspend mode
 */
typedef enum {
	TM_USB_HIDDEVICE_Status_LibraryNotInitialized,
	TM_USB_HIDDEVICE_Status_Connected,
	TM_USB_HIDDEVICE_Status_Disconnected,
	TM_USB_HIDDEVICE_Status_IdleMode,
	TM_USB_HIDDEVICE_Status_SuspendMode
} TM_USB_HIDDEVICE_Status_t;

/**
 * Button status typedef
 * 
 * Parameters:
 * 	- TM_USB_HIDDEVICE_Button_Released
 * 		Button is not pressed
 * 	- TM_USB_HIDDEVICE_Button_Pressed
 * 		Button is pressed
 */
typedef enum {
	TM_USB_HIDDEVICE_Button_Released = 0,
	TM_USB_HIDDEVICE_Button_Pressed = 1
} TM_USB_HIDDEVICE_Button_t;

/**
 * 2 Gamepads are supported simultaneously to work with
 * 
 * Parameters:
 * 	- TM_USB_HIDDEVICE_Gamepad_Number_1
 * 		Send data to computer for gamepad 1
 * 	- TM_USB_HIDDEVICE_Gamepad_Number_2
 * 		Send data to computer for gamepad 2
 */
typedef enum {
	TM_USB_HIDDEVICE_Gamepad_Number_1 = 0x03,
	TM_USB_HIDDEVICE_Gamepad_Number_2 = 0x04
} TM_USB_HIDDEVICE_Gamepad_Number_t;

/**
 * Mouse struct, to work with mouse
 * 
 * Parameters:
 * 	- TM_USB_HIDDEVICE_Button_t LeftButton:
 * 		Detect if left button is pressed and set this to send command to computer
 * 	- TM_USB_HIDDEVICE_Button_t RightButton:
 * 		Detect if right button is pressed and set this to send command to computer
 * 	- TM_USB_HIDDEVICE_Button_t MiddleButton:
 * 		Detect if middle button is pressed and set this to send command to computer
 * 	- int8_t XAxis:
 * 		Cursor's X axis relative movement 
 * 	- int8_t YAxis:
 * 		Cursor's Y axis relative movement
 * 	- int8_t Wheel:
 * 		Wheel movement
 */
typedef struct {
	TM_USB_HIDDEVICE_Button_t LeftButton;
	TM_USB_HIDDEVICE_Button_t RightButton;
	TM_USB_HIDDEVICE_Button_t MiddleButton;
	int8_t XAxis;
	int8_t YAxis;
	int8_t Wheel;
} TM_USB_HIDDEVICE_Mouse_t;

/**
 * Gamepad struct for 2 gamepads available
 * 
 * Parameters:
 * 	- TM_USB_HIDDEVICE_Button_t Button[1:16]:
 * 		Set value, according to if button on your project is pressed
 * 		Gamepad supports 16 buttons
 * 	- int8_t LeftXAxis:
 * 		Left joystick X axis
 * 	- int8_t LeftYAxis:
 * 		Left joystick Y axis
 * 	- int8_t RightXAxis:
 * 		Right joystick Y axis
 * 	- int8_t RightYAxis: 
 * 		Right joystick Y axis
 */
typedef struct {
	TM_USB_HIDDEVICE_Button_t Button1;
	TM_USB_HIDDEVICE_Button_t Button2;
	TM_USB_HIDDEVICE_Button_t Button3;
	TM_USB_HIDDEVICE_Button_t Button4;
	TM_USB_HIDDEVICE_Button_t Button5;
	TM_USB_HIDDEVICE_Button_t Button6;
	TM_USB_HIDDEVICE_Button_t Button7;
	TM_USB_HIDDEVICE_Button_t Button8;
	TM_USB_HIDDEVICE_Button_t Button9;
	TM_USB_HIDDEVICE_Button_t Button10;
	TM_USB_HIDDEVICE_Button_t Button11;
	TM_USB_HIDDEVICE_Button_t Button12;
	TM_USB_HIDDEVICE_Button_t Button13;
	TM_USB_HIDDEVICE_Button_t Button14;
	TM_USB_HIDDEVICE_Button_t Button15;
	TM_USB_HIDDEVICE_Button_t Button16;
	int8_t LeftXAxis;
	int8_t LeftYAxis;
	int8_t RightXAxis;
	int8_t RightYAxis;
} TM_USB_HIDDEVICE_Gamepad_t;

/**
 * Keyboard typedef
 * 
 * Keyboard has special 8 buttons (CTRL, ALT, SHIFT, GUI (or WIN), all are left and right).
 * It also supports 6 others keys to be pressed at same time, let's say Key1 = 'a', Key2 = 'b', and you will get "ab" result on the screen.
 * If key is not used, then 0x00 value should be set
 * 
 * Parameters:
 * 	- TM_USB_HIDDEVICE_Button_t L_CTRL:
 * 		Left CTRL button
 * 	- TM_USB_HIDDEVICE_Button_t L_ALT:
 * 		Left ALT button
 * 	- TM_USB_HIDDEVICE_Button_t L_SHIFT:
 * 		Left SHIFT button
 * 	- TM_USB_HIDDEVICE_Button_t L_GUI:
 * 		Left GUI (or Win) button
 * 	- TM_USB_HIDDEVICE_Button_t R_CTRL:
 * 		Right CTRL button
 * 	- TM_USB_HIDDEVICE_Button_t R_ALT:
 * 		Right ALT button
 * 	- TM_USB_HIDDEVICE_Button_t R_SHIFT:
 * 		Right SHIFT button
 * 	- TM_USB_HIDDEVICE_Button_t R_GUI:
 * 		Right GUI (or Win) button
 * 	- uint8_t Key[1:6]:
 * 		Key used with keyboard.
 * 		This can be whatever. Like numbers, letters, everything.
 * 		Also, 6 Key parameters you have. It means you can set 6
 * 		different numbers, letters, whatever in one keyboard update.
 */
typedef struct {
	TM_USB_HIDDEVICE_Button_t L_CTRL;
	TM_USB_HIDDEVICE_Button_t L_ALT;
	TM_USB_HIDDEVICE_Button_t L_SHIFT;
	TM_USB_HIDDEVICE_Button_t L_GUI;
	TM_USB_HIDDEVICE_Button_t R_CTRL;
	TM_USB_HIDDEVICE_Button_t R_ALT;
	TM_USB_HIDDEVICE_Button_t R_SHIFT;
	TM_USB_HIDDEVICE_Button_t R_GUI;
	uint8_t Key1;
	uint8_t Key2;
	uint8_t Key3;
	uint8_t Key4;
	uint8_t Key5;
	uint8_t Key6;
} TM_USB_HIDDEVICE_Keyboard_t;

/**
 * Initialize USB HID Device library for work
 * 
 * This library always returns TM_USB_HIDDEVICE_Status_Disconnected
 */
extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_Init(void);

/**
 * Get USB status
 * 
 * Returns TM_USB_HIDDEVICE_Status_Connected, if library is ready to use with USB,
 * otherwise one of members TM_USB_HIDDEVICE_Status_t is returned
 */
extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_GetStatus(void);

/**
 * Initialize structure for mouse 
 * 
 * Set default values, before you start working
 * 
 * Parameters:
 * 	- TM_USB_HIDDEVICE_Mouse_t* Mouse_Data
 * 		Pointer to mouse struct
 * 
 * Returns member of TM_USB_HIDDEVICE_Status_t
 */
extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_MouseStructInit(TM_USB_HIDDEVICE_Mouse_t* Mouse_Data);

/**
 * Sends mouse data over USB
 * 
 * Parameters:
 * 	- Pointer to mouse struct for data to be send
 * 
 * Returns member of TM_USB_HIDDEVICE_Status_t
 */
extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_MouseSend(TM_USB_HIDDEVICE_Mouse_t* Mouse_Data);

/**
 * Sends command to release all mouse data in computer.
 * This will release all button in computer for mouse and cursor will stop
 * 
 * Returns member of TM_USB_HIDDEVICE_Status_t
 */
extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_MouseReleaseAll(void);

/**
 * Initialize structure for gamepad 
 * 
 * Set default values, before you start working with gamepads
 * 
 * Parameters:
 * 	- TM_USB_HIDDEVICE_Gamepad_t* Gamepad_Data
 * 		Pointer to Gamepad struct
 * 
 * Returns member of TM_USB_HIDDEVICE_Status_t
 */
extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_GamepadStructInit(TM_USB_HIDDEVICE_Gamepad_t* Gamepad_Data);

/**
 * Sends Gamepad report over USB
 * 
 * Parameters:
 * 	- TM_USB_HIDDEVICE_Gamepad_Number_t gamepad_id:
 * 		Gamepad number. 2 of them are supported
 * 	- TM_USB_HIDDEVICE_Gamepad_t* Gamepad_Data:
 * 		Pointer to Gamepad data to be send over USB
 * 
 * Returns member of TM_USB_HIDDEVICE_Status_t
 */
extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_GamepadSend(TM_USB_HIDDEVICE_Gamepad_Number_t gamepad_id, TM_USB_HIDDEVICE_Gamepad_t* Gamepad_Data);

/**
 * Release all buttons and joysticks over USB
 * 
 * If you press a button, and don't release it, computer will recognize as long pressed button
 * 
 * Parameters:
 * 	- TM_USB_HIDDEVICE_Gamepad_Number_t gamepad_id
 * 		Gamepad number. 2 of them are supported
 * 
 * Returns member of TM_USB_HIDDEVICE_Status_t
 */
extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_GamepadReleaseAll(TM_USB_HIDDEVICE_Gamepad_Number_t gamepad_id);

/**
 * Set default values to work with keyboard
 * 
 * Parameters:
 * 	- TM_USB_HIDDEVICE_Keyboard_t* Keyboard_Data:
 * 		Pointer to keyboard struct
 * 
 * Returns member of TM_USB_HIDDEVICE_Status_t
 */
extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_KeyboardStructInit(TM_USB_HIDDEVICE_Keyboard_t* Keyboard_Data);

/**
 * Sends keyboard report over USB
 * 
 * Parameters:
 * 	- TM_USB_HIDDEVICE_Keyboard_t* Keyboard_Data:
 * 		Pointer to keyboard struct to be sent over USB
 * 
 * Returns member of TM_USB_HIDDEVICE_Status_t
 */
extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_KeyboardSend(TM_USB_HIDDEVICE_Keyboard_t* Keyboard_Data);

/**
 * Release all buttons from keyboard
 * 
 * If you press a button and don't release it, computer will detect like long pressed button
 * 
 * Returns member of TM_USB_HIDDEVICE_Status_t
 */
extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_KeyboardReleaseAll(void);

/**
 * Sends custom HID report over USB
 * 
 * Parameters:
 * 	- uint8_t* buff:
 * 		Pointer for data to be sent
 * 	- uint8_t count:
 * 		number of bytes to be sent
 * 
 * Returns member of TM_USB_HIDDEVICE_Status_t
 */
extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_SendCustom(uint8_t* buff, uint8_t count);

#endif
