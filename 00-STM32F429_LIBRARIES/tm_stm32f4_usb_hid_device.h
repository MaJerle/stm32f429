/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/09/library-34-stm32f4-usb-hid-device
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   USB HID Device library for STM32F4
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
#ifndef TM_USB_HIDDEVICE_H
#define TM_USB_HIDDEVICE_H 100
/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_USB_HID_DEVICE
 * @brief    USB HID Device library for STM32F4xx - http://stm32f4-discovery.com/2014/09/library-34-stm32f4-usb-hid-device
 * @{
 *
 * Library is designed to operate as HID device. This means that STM32F4xx device will be shown to your computer 
 * as USB Keyboard, Mouse and 2 game pads at the same time.
 *
 * It works in USB FS or USB HS in FS mode.
 *
 * By default, library works in USB FS mode (for STM32F4-Discovery board).
 * If you want to use this on STM32F429-Discovery board, you have to activate USB HS in FS mode.
 * Activate this with lines below in your defines.h file:
 *
@verbatim
//Activate USB HS in FS mode
#define USE_USB_OTG_HS
@endverbatim
 *
 * \par Pinout
 *
@verbatim
USB			|STM32F4xx FS mode				|STM32F4xx HS in FS mode	|Notes
			|STM32F4-Discovery				|STM32F429-Discovery

Data +		PA12							PB15						Data+ for USB, standard and used pin
Data -		PA11							PB14						Data- for USB, standard and used pin
ID			PA10							PB12						ID pin, used on F4 and F429 discovery boards, not needed if you don't like it
VBUS		PA9								PB13						VBUS pin, used on F4 and F429 discovery board for activating USB chip.
	
@endverbatim
 *
 * You have to use VBUS on discovery boards, but on nucleo, it's ok with only Data+ and Data- pins
 * Disable necessary pins
 *
 * USB technically needs only Data+ and Data- pins.
 * Also, ID pin can be used, but it is not needed.
 *
 * \par Disable ID PIN
 *
 * If you need pin for something else, where ID is located, you can disable this pin for USB.
 * Add lines below in your defines.h file:
 *
@verbatim
//Disable ID pin
#define USB_HID_HOST_DISABLE_ID
@endverbatim
 *
 * \par Disable VBUS PIN
 *
 * VBUS pin is located on Discovery boards, to activate USB chip on board.
 * If you are working with Discovery boards, then you need this pin, otherise USB will not work.
 * But if you are working on other application (or Nucleo board), you only need Data+ and Data- pins.
 * To disable VBUS pin, add lines below in your defines.h file:
 *
@verbatim
//Disable VBUS pin
#define USB_HID_HOST_DISABLE_VBUS
@endverbatim
 *
 * \par Changelog
 *
@verbatim
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - STM32F4xx GPIO
 - STM32F4xx EXTI
 - misc.h
 - defines.h
 - USB HID DEVICE
@endverbatim
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
 * @defgroup TM_USB_HID_DEVICE_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  USB HID device enumeration	
 */
typedef enum {
	TM_USB_HIDDEVICE_Status_LibraryNotInitialized = 0x00, /*!< Library is not initialized yet */
	TM_USB_HIDDEVICE_Status_Connected,                    /*!< Device is connected and ready to use */
	TM_USB_HIDDEVICE_Status_Disconnected,                 /*!< Device is not connected */
	TM_USB_HIDDEVICE_Status_IdleMode,                     /*!< Device is is IDLE mode */
	TM_USB_HIDDEVICE_Status_SuspendMode                   /*!< Device is in suspend mode */
} TM_USB_HIDDEVICE_Status_t;

/**
 * @brief  Button status enumeration
 */
typedef enum {
	TM_USB_HIDDEVICE_Button_Released = 0x00, /*!< Button is not pressed */
	TM_USB_HIDDEVICE_Button_Pressed = 0x01   /*!< Button is pressed */
} TM_USB_HIDDEVICE_Button_t;

/**
 * @brief  2 Game pads are supported simultaneously to work with
 */
typedef enum {
	TM_USB_HIDDEVICE_Gamepad_Number_1 = 0x03, /*!< Send data to computer for game pad 1 */
	TM_USB_HIDDEVICE_Gamepad_Number_2 = 0x04  /*!< Send data to computer for game pad 2 */
} TM_USB_HIDDEVICE_Gamepad_Number_t;

/**
 * @brief  Mouse structure, to work with mouse
 */
typedef struct {
	TM_USB_HIDDEVICE_Button_t LeftButton;   /*!< Detect if left button is pressed and set this to send command to computer, This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t RightButton;  /*!< Detect if right button is pressed and set this to send command to computer, This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t MiddleButton; /*!< Detect if middle button is pressed and set this to send command to computer, This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	int8_t XAxis;                           /*!< Mouse X axis */
	int8_t YAxis;                           /*!< Mouse Y axis */
	int8_t Wheel;                           /*!< Mouse wheel rotation */
} TM_USB_HIDDEVICE_Mouse_t;

/**
 * @brief  Game pad structure for 2 game pads available
 */
typedef struct {
	TM_USB_HIDDEVICE_Button_t Button1;  /*!< Game pad button 1 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button2;  /*!< Game pad button 2 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button3;  /*!< Game pad button 3 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button4;  /*!< Game pad button 4 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button5;  /*!< Game pad button 5 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button6;  /*!< Game pad button 6 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button7;  /*!< Game pad button 7 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button8;  /*!< Game pad button 8 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button9;  /*!< Game pad button 9 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button10; /*!< Game pad button 10 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button11; /*!< Game pad button 11 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button12; /*!< Game pad button 12 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button13; /*!< Game pad button 13 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button14; /*!< Game pad button 14 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button15; /*!< Game pad button 15 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t Button16; /*!< Game pad button 16 status. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	int8_t LeftXAxis;                   /*!< Left joystick X axis */
	int8_t LeftYAxis;                   /*!< Left joystick Y axis */
	int8_t RightXAxis;                  /*!< Right joystick X axis */
	int8_t RightYAxis;                  /*!< Right joystick Y axis */
} TM_USB_HIDDEVICE_Gamepad_t;

/**
 * @brief  Keyboard structure
 * @note   Keyboard has special 8 buttons (CTRL, ALT, SHIFT, GUI (or WIN), all are left and right).
 *         It also supports 6 others keys to be pressed at same time, let's say Key1 = 'a', Key2 = 'b', and you will get "ab" result on the screen.
 *         If key is not used, then 0x00 value should be set!
 */
typedef struct {
	TM_USB_HIDDEVICE_Button_t L_CTRL;  /*!< Left CTRL button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t L_ALT;   /*!< Left ALT button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t L_SHIFT; /*!< Left SHIFT button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t L_GUI;   /*!< Left GUI (Win) button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t R_CTRL;  /*!< Right CTRL button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t R_ALT;   /*!< Right ALT button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t R_SHIFT; /*!< Right SHIFT button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	TM_USB_HIDDEVICE_Button_t R_GUI;   /*!< Right GUI (Win) button. This parameter can be a value of @ref TM_USB_HIDDEVICE_Button_t enumeration */
	uint8_t Key1;                      /*!< Key used with keyboard. This can be whatever. Like numbers, letters, everything. */
	uint8_t Key2;                      /*!< Key used with keyboard. This can be whatever. Like numbers, letters, everything. */
	uint8_t Key3;                      /*!< Key used with keyboard. This can be whatever. Like numbers, letters, everything. */
	uint8_t Key4;                      /*!< Key used with keyboard. This can be whatever. Like numbers, letters, everything. */
	uint8_t Key5;                      /*!< Key used with keyboard. This can be whatever. Like numbers, letters, everything. */
	uint8_t Key6;                      /*!< Key used with keyboard. This can be whatever. Like numbers, letters, everything. */
} TM_USB_HIDDEVICE_Keyboard_t;

/**
 * @}
 */

/**
 * @defgroup TM_USB_HID_DEVICE_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes USB HID Device library
 * @param  None
 * @retval TM_USB_HIDDEVICE_Status_Disconnected
 */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_Init(void);

/**
 * @brief  Gets USB status
 * @param  None
 * @retval Library usage status:
 *            - TM_USB_HIDDEVICE_Status_Connected: Library is ready to use with USB
 *            - Others: Look for @ref TM_USB_HIDDEVICE_Status_t enumeration
 */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_GetStatus(void);

/**
 * @brief  Initializes structure for mouse 
 *         Sets default values, before you start working
 * @param  *Mouse_Data: Pointer to empty @ref TM_USB_HIDDEVICE_Mouse_t structure
 * @retval Member of @ref TM_USB_HIDDEVICE_Status_t enumeration
 */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_MouseStructInit(TM_USB_HIDDEVICE_Mouse_t* Mouse_Data);

/**
 * @brief  Sends mouse data over USB
 * @param  *Mouse_Data: Pointer to empty @ref TM_USB_HIDDEVICE_Mouse_t structure to get data from
 * @retval Member of @ref TM_USB_HIDDEVICE_Status_t enumeration
 */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_MouseSend(TM_USB_HIDDEVICE_Mouse_t* Mouse_Data);

/**
 * @brief  Sends command to release all mouse data in computer.
 *         This will release all button in computer for mouse and cursor will stop
 * @param  None
 * @retval Member of @ref TM_USB_HIDDEVICE_Status_t enumeration
 */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_MouseReleaseAll(void);

/**
 * @brief  Initializes structure for game pad 
 *         Sets default values, before you start working with game pads
 * @param  *Gamepad_Data: Pointer to empty @ref TM_USB_HIDDEVICE_Gamepad_t structure
 * @retval Member of @ref TM_USB_HIDDEVICE_Status_t enumeration
 */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_GamepadStructInit(TM_USB_HIDDEVICE_Gamepad_t* Gamepad_Data);

/**
 * @brief  Sends game pad report over USB
 * @param  gamepad_id: Game pad number to work with. This parameter can be a value of @ref TM_USB_HIDDEVICE_Gamepad_Number_t enumeration
 * @param  *Gamepad_Data: Pointer to working @ref TM_USB_HIDDEVICE_Gamepad_t structure to get data from
 * @retval Member of @ref TM_USB_HIDDEVICE_Status_t enumeration
 */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_GamepadSend(TM_USB_HIDDEVICE_Gamepad_Number_t gamepad_id, TM_USB_HIDDEVICE_Gamepad_t* Gamepad_Data);

/**
 * @brief  Releases all buttons and joysticks over USB
 * @note   If you press a button, and don't release it, computer will recognize as long pressed button
 * @param  gamepad_id: Game pad number to work with. This parameter can be a value of @ref TM_USB_HIDDEVICE_Gamepad_Number_t enumeration
 * @retval Member of @ref TM_USB_HIDDEVICE_Status_t enumeration
 */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_GamepadReleaseAll(TM_USB_HIDDEVICE_Gamepad_Number_t gamepad_id);

/**
 * @brief  Sets default values to work with keyboard
 * @param  *Keyboard_Data: Pointer to empty @ref TM_USB_HIDDEVICE_Keyboard_t structure
 * @retval Member of @ref TM_USB_HIDDEVICE_Status_t enumeration
 */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_KeyboardStructInit(TM_USB_HIDDEVICE_Keyboard_t* Keyboard_Data);

/**
 * @brief  Sends keyboard report over USB
 * @param  *Keyboard_Data: Pointer to @ref TM_USB_HIDDEVICE_Keyboard_t structure to get data from
 * @retval Member of @ref TM_USB_HIDDEVICE_Status_t enumeration
 */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_KeyboardSend(TM_USB_HIDDEVICE_Keyboard_t* Keyboard_Data);

/**
 * @brief  Release all buttons from keyboard
 * @note   If you press a button and don't release it, computer will detect like long pressed button
 * @param  None
 * @retval Member of @ref TM_USB_HIDDEVICE_Status_t enumeration
 */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_KeyboardReleaseAll(void);

/**
 * @brief  Sends custom HID report over USB
 * @param  *buff: Pointer to data to be sent
 * @param  count: Number of bytes to be sent
 * @retval Member of @ref TM_USB_HIDDEVICE_Status_t enumeration
 */
TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_SendCustom(uint8_t* buff, uint8_t count);

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
