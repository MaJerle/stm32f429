/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/08/library-31-usb-hid-host-for-stm32f4xx-devices
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   USB HID HOST library for STM32F4xx devices
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
#ifndef TM_USB_HID_HOST_H
#define TM_USB_HID_HOST_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_USB_HID_HOST
 * @brief    USB HID HOST library for STM32F4xx devices - http://stm32f4-discovery.com/2014/08/library-31-usb-hid-host-for-stm32f4xx-devices
 * @{
 *
 * This library allows you to operate with USB keyboard and USB mouse devices
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
 * Pinout (can not be changed)
 *
@verbatim
USB			|STM32F4xx FS mode				|STM32F4xx HS in FS mode	|Notes
			|STM32F4-Discovery				|STM32F429-Discovery
Data +		PA12							PB15						Data+ for USB, standard and used pin
Data -		PA11							PB14						Data- for USB, standard and used pin
ID			PA10							PB12						ID pin, used on F4 and F429 discovery boards, not needed if you don't like it
VBUS		PA9								PB13						VBUS pin, used on F4 and F429 discovery board for activating USB chip.
																		You have to use VBUS on discovery boards, but on nucleo, it's ok with only Data+ and Data- pins
@endverbatim
 *
 * \par Disable necessary pins
 *
 * USB technically needs only Data+ and Data- pins.
 * Also, ID pin can be used, but it is not needed.
 *
 * Disable ID PIN
 *
 * If you need pin for something else, where ID is located, you can disable this pin for USB.
 * Add lines below in your defines.h file:
 *
@verbatim//Disable ID pin
#define USB_HID_HOST_DISABLE_ID
@endverbatim
 *
 * Disable VBUS PIN
 *
 * VBUS pin is located on Discovery boards, to activate USB chip on board.
 * If you are working with Discovery boards, then you need this pin, otherise USB will not work.
 * But if you are working on other application (or Nucleo board), you only need Data+ and Data- pins.
 * To disable VBUS pin, add lines below in your defines.h file:
 *
@verbatim//Disable VBUS pin
#define USB_HID_HOST_DISABLE_VBUS
@endverbatim
 *	
 * \par Choose keyboard style
 *
 * Default configuration for keyboard is AZERTY style. If you want to use QWERTY keyboard style,
 * then add line below in defines.h to activate it.
 *
@verbatim
//Enable QUERTY keyboard style
#define QWERTY_KEYBOARD
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
 - defines.h
 - USB HID HOST stack
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"

#include "usb_bsp.h"
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_hid_core.h"

/**
 * @defgroup TM_USB_HID_HOST_Macros
 * @brief    Library defines
 * @{
 */

/** 
 * @brief  Re initialize mouse values after read
 */
#ifndef USB_HIDHOST_REINITIALIZE_MOUSE_AFTER_READ
#define USB_HIDHOST_REINITIALIZE_MOUSE_AFTER_READ		0
#endif

/** 
 * @brief  Re initialize keyboard values after read
 */
#ifndef USB_HIDHOST_REINITIALIZE_KEYBOARD_AFTER_READ
#define USB_HIDHOST_REINITIALIZE_KEYBOARD_AFTER_READ	0
#endif

/**
 * @}
 */
 
/**
 * @defgroup TM_USB_HID_HOST_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  USB HID Host result enumeration 		
 */
typedef enum {
	TM_USB_HIDHOST_Result_Error,                /*!< An error occurred */
	TM_USB_HIDHOST_Result_KeyboardConnected,    /*!< Keyboard is connected and ready to use */
	TM_USB_HIDHOST_Result_MouseConnected,       /*!< Mouse is connected and ready to use */
	TM_USB_HIDHOST_Result_Disconnected,         /*!< Device is not connected */
	TM_USB_HIDHOST_Result_DeviceNotSupported,   /*!< Device is not supported */
	TM_USB_HIDHOST_Result_LibraryNotInitialized /*!< Library is not initialized */
} TM_USB_HIDHOST_Result_t;

/**
 * @brief  USB HID Host Button enumeration
 */
typedef enum {
	TM_USB_HIDHOST_Button_Pressed = 0, /*!< Button was pressed */
	TM_USB_HIDHOST_Button_Released     /*!< Button was released */
} TM_USB_HIDHOST_Button_t;

/**
 * @brief  USB HID Host keyboard structure	
 */
typedef struct {
	TM_USB_HIDHOST_Button_t ButtonStatus; /*!< Indicates if button is pressed or released */
	uint8_t Button;                       /*!< Button number pressed */
} TM_USB_HIDHOST_Keyboard_t;

/**
 * @brief  USB HID Host mouse structure
 */
typedef struct {
	/* Cursor movement */
	int16_t AbsoluteX;                    /*!< Absolute cursor X position */
	int16_t AbsoluteY;                    /*!< Absolute cursor Y position */
	int16_t DiffX;                        /*!< Difference cursor X position from last check */
	int16_t DiffY;                        /*!< Difference cursor Y position from last check */
	
	/* Buttons */
	TM_USB_HIDHOST_Button_t LeftButton;   /*!< Indicates if left button is pressed or released */
	TM_USB_HIDHOST_Button_t RightButton;  /*!< Indicates if right button is pressed or released */
	TM_USB_HIDHOST_Button_t MiddleButton; /*!< Indicates if middle button is pressed or released */
} TM_USB_HIDHOST_Mouse_t;

/**
 * @}
 */

/**
 * @defgroup TM_USB_HID_HOST_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes USB HID Host
 * @param  None
 * @retval None
 */
void TM_USB_HIDHOST_Init(void);

/**
 * @brief  Processes USB HID library
 * @note   This function has to be called periodically, as fast as possible
 * @param  None
 * @retval Status: Member of @ref TM_USB_HIDHOST_Result_t
 */
TM_USB_HIDHOST_Result_t TM_USB_HIDHOST_Process(void);

/**
 * @brief  Checks device status
 * @param  None
 * @retval Status: Member of @ref TM_USB_HIDHOST_Result_t
 */
TM_USB_HIDHOST_Result_t TM_USB_HIDHOST_Device(void);

/**
 * @param  Reads keyboard data
 * @param  *Keyboard: Pointer to @ref TM_USB_HIDHOST_Keyboard_t keyboard structure
 * @retval Keyboard status: 
 *            - @ref TM_USB_HIDHOST_Result_KeyboardConnected if keyboard is connected
 */
TM_USB_HIDHOST_Result_t TM_USB_HIDHOST_ReadKeyboard(TM_USB_HIDHOST_Keyboard_t* Keyboard);

/**
 * @param  Reads mouse data
 * @param  *Mouse: Pointer to @ref TM_USB_HIDHOST_Mouse_t mouse structure
 * @retval Mouse status: 
 *            - @ref TM_USB_HIDHOST_Result_MouseConnected if mouse is connected
 */
TM_USB_HIDHOST_Result_t TM_USB_HIDHOST_ReadMouse(TM_USB_HIDHOST_Mouse_t* Mouse);

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

