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


/* Uncomment if you want to disable ID pin for USB MSC HOST library */
//#define USB_MSC_HOST_DISABLE_ID

/* Uncomment if you want to disable VBUS pin for USB MSC HOST library */
/* If you do this, on F4 and F429 Discovery boards USB will not work */
//#define USB_MSC_HOST_DISABLE_VBUS

/* Uncomment if you want to enable USB HS in FS mode */
/* By default, FS mode is used */
//#define USE_USB_OTG_HS		/* STM32F429-Discovery */

/* This line MUST BE ADDED in order to use FATFS with USB */
/* Otherwise you will have errors */
#define FATFS_USE_USB		1

/* By default, if you use USB, then SD card is disabled */
/* If you don't use USB, then SDIO is used by default! */
/* If you want to use both, USB and sdcard at the same time */
/* Enable SD card communication below: */

/* Enable SDIO communication */
#define FATFS_USE_SDIO	1
/* If you want SPI communication instead of SDIO, enable with */
//#define FATFS_USE_SDIO	0

#endif
