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

/* Set USART receive buffer length to 256 bytes */
#define TM_USART_BUFFER_SIZE	256

/* Change custom USART */
/* USART1 is selected by default, if you want custom setting, */
/* then uncomment lines below and edit them. */
//#define GPS_USART				USART1
//#define GPS_USART_PINSPACK	TM_USART_PinsPack_2

/* By default, each of this data has to be detected in order to get "VALID" data. */
/* If your GPS does not return any of this statement, you can disable option. */
/* If you disable any of statements, then you will loose data, corresponding to statement. */
/* Uncomment line which statement is not supported by your GPS */
//#define GPS_DISABLE_GPGGA 	/* Disable GPGGA statement */
//#define GPS_DISABLE_GPRMC		/* Disable GPRMC statement */
//#define GPS_DISABLE_GPGSA		/* Disable GPGSA statement */
//#define GPS_DISABLE_GPGSV		/* Disable GPGSV statement */

#endif

