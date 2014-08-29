/**
 *	USB MSC HOST library for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/07/library-21-read-sd-card-fatfs-stm32f4xx-devices/
 *	@link		http://stm32f4-discovery.com/2014/08/library-29-usb-msc-host-usb-flash-drive-stm32f4xx-devices
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
 * This library allows you to read and write data to USB mass storage devices.
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
 * 	#define USB_MSC_HOST_DISABLE_ID
 * 	
 * Disable VBUS PIN
 * 
 * VBUS pin is located on Discovery boards, to activate USB chip on board.
 * If you are working with Discovery boards, then you need this pin, otherise USB will not work.
 * But if you are working on other application (or Nucleo board), you only need Data+ and Data- pins.
 * To disable VBUS pin, add lines below in your defines.h file:
 * 
 * 	//Disable VBUS pin
 * 	#define USB_MSC_HOST_DISABLE_VBUS
 *
 * -------------------------------------------------------------------------------------
 * Now, we have initialized USB, we have to enable it for FATFS too.
 * Open your project defines.h file and add line below to activate FatFS for USB.
 *	
 *	//Activate FatFS library for USB communication
 *	#define FATFS_USE_USB		1
 *
 * Too see, how to use FatFS with USB, open file tm_stm32f4_fatfs.h and check it's manual
 * -------------------------------------------------------------------------------------
 * */
#ifndef TM_USB_MSCHOST
#define TM_USB_MSCHOST	100
/**
 * Dependencies
 * - STM32F4xx
 * - defines.h
 * - USB HOST stack
 * - TM FATFS library
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "defines.h"

#include "ff.h"
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_msc_core.h"

/**
 * Result enumeration
 * - TM_USB_MSCHOST_Result_Error
 * 		An error occured
 * - TM_USB_MSCHOST_Result_Connected
 * 		Device is connected and ready to use with FATFS
 * - TM_USB_MSCHOST_Result_Disconnected
 * 		Device is not connected
 * - TM_USB_MSCHOST_Result_DeviceNotSupported
 * 		Device is not supported
 * - TM_USB_MSCHOST_Result_WriteProtected
 * 		Device is write protected
 * - TM_USB_MSCHOST_Result_LibraryNotInitialized
 *		Library is not initialized yet
 */
typedef enum {
	TM_USB_MSCHOST_Result_Error,
	TM_USB_MSCHOST_Result_Connected,
	TM_USB_MSCHOST_Result_Disconnected,
	TM_USB_MSCHOST_Result_DeviceNotSupported,
	TM_USB_MSCHOST_Result_WriteProtected,
	TM_USB_MSCHOST_Result_LibraryNotInitialized
} TM_USB_MSCHOST_Result_t;

/**
 * Initialize USB MSC HOST peripheral
 * 
 * Returns current device status. A member of TM_USB_MSCHOST_Result_t typedef is returned
 */
extern TM_USB_MSCHOST_Result_t TM_USB_MSCHOST_Init(void);

/**
 * This function has to be called periodically. 
 * As fast as possible.
 * 
 * It handles USB device
 * 
 * Returns TM_USB_MSCHOST_Result_t typedef member
 */
extern void TM_USB_MSCHOST_Process(void);

/**
 * Check device status.
 * 
 * Returns current device status. A member of TM_USB_MSCHOST_Result_t typedef is returned
 * if TM_USB_MSCHOST_Result_Connected is returned, then you are ready to work
 */
extern TM_USB_MSCHOST_Result_t TM_USB_MSCHOST_Device(void);

#endif

