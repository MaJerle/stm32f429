/**
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		
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
 */
#ifndef TM_USB_HIDDEVICE_H
#define TM_USB_HIDDEVICE_H 100
/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx EXTI
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
#include "defines.h"

#include  "usbd_hid_core.h"
#include  "usbd_usr.h"
#include  "usbd_desc.h"

typedef enum {
	TM_USB_HIDDEVICE_Status_LibraryNotInitialized,
	TM_USB_HIDDEVICE_Status_Connected,
	TM_USB_HIDDEVICE_Status_Disconnected,
	TM_USB_HIDDEVICE_Status_IdleMode,
	TM_USB_HIDDEVICE_Status_SuspendMode
} TM_USB_HIDDEVICE_Status_t;

extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_Init(void);
extern TM_USB_HIDDEVICE_Status_t TM_USB_HIDDEVICE_GetStatus(void);

#endif
