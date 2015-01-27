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

/* Default LCD width in pixels */
//#define DMA2D_GRAPHIC_LCD_WIDTH		240

/* Default LCD height in pixels */
//#define DMA2D_GRAPHIC_LCD_HEIGHT		320

/* Number of LCD pixels */
//#define DMA2D_GRAPHIC_PIXELS			DMA2D_GRAPHIC_LCD_WIDTH * DMA2D_GRAPHIC_LCD_HEIGHT

/* RAM Start address for LCD */
/* On STM32F429-Discovery, this is address for SDRAM which operate with LCD and LTDC peripheral */
//#define DMA2D_GRAPHIC_RAM_ADDR		0xD0000000

#endif
