/**
 *  Defines for your entire project at one place
 * 
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@version 	v1.0
 *	@ide		Keil uVision 5
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
 */
#ifndef TM_DEFINES_H
#define TM_DEFINES_H

/* Put your global defines for all libraries here used in your project */

/* Enable if you want to use USB HS in FS mode */
/* For "STM32F429-Discovery Target" I have done this define in preprocessor defines */
/* Under "Options for target" -> "C/C++" -> "Define" */
//#define USE_USB_OTG_HS	/* STM32F429-Discovery */

/* Uncomment if you want to disable ID pin for USB HID Device library */
#define USB_HID_DEVICE_DISABLE_ID

/* Uncomment if you want to disable VBUS pin for USB HID HOST library */
/* If you do this, on F4 and F429 Discovery boards USB will not work */
#define USB_HID_DEVICE_DISABLE_VBUS

#endif

