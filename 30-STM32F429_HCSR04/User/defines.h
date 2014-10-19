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

/* Trigger PIN */
/* Uncomment lines, if you want to select your custom pin for TRIGGER */
//#define HCSR04_TRIGGER_RCC		RCC_AHB1Periph_GPIOB
//#define HCSR04_TRIGGER_PORT		GPIOB
//#define HCSR04_TRIGGER_PIN		GPIO_Pin_2

/* Echo PIN */
/* Uncomment lines, if you want to select your custom pin for ECHO */
//#define HCSR04_ECHO_RCC			RCC_AHB1Periph_GPIOB
//#define HCSR04_ECHO_PORT			GPIOB
//#define HCSR04_ECHO_PIN			GPIO_Pin_3

/* Default timeout pulses for  */
/* Uncomment and change only if you know what are you doing */
/* These are clock pulses, how many time we get to sensor to response to our action */
//#define HCSR04_TIMEOUT			1000000

#endif

