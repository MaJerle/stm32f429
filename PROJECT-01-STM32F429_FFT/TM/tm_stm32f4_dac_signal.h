/**
 *	DAC with DMA and TIM signal generator feature for STM32F4
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/09/library-36-dac-signal-generator-stm32f4
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
 * Library supports 4 output signals:
 * 	- Square
 * 	- Sinus
 * 	- Triangle
 * 	- Sawtooth
 * 	
 * Signals work well to about 50kHz. You can use 2 DAC channel simultaneously.
 * Timers, available for signal generation are:
 * 	TIM2, TIM4, TIM5, TIM6, TIM7, TIM8
 * 
 * Pinout:
 * 	
 * 	DACx	STM32F4		Description
 * 	
 * 	DAC1	PA4			DAC Channel 1
 * 	DAC2	PA5			DAC Channel 2
 * 	
 * Pins for DAC are fixed, analog and can not be changed
 */
#ifndef TM_DAC_SIGNAL_H
#define TM_DAC_SIGNAL_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx DAC
 * - STM32F4xx TIM
 * - STM32F4xx DMA
 * - defines.h
 * - TM TIMER PROPERTIES
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dma.h"
#include "defines.h"
#include "tm_stm32f4_timer_properties.h"

/* Signals used */
#define DAC_SIGNAL_SINUS_LENGTH			32
#define DAC_SIGNAL_TRIANGLE_LENGTH		32
#define DAC_SIGNAL_SAWTOOTH_LENGTH		32
#define DAC_SIGNAL_SQUARE_LENGTH		2

/* DAC DMA settings */
#define DAC_SIGNAL_DMA_DAC1_STREAM		DMA1_Stream5
#define DAC_SIGNAL_DMA_DAC1_CHANNEL		DMA_Channel_7
#define DAC_SIGNAL_DMA_DAC2_STREAM		DMA1_Stream6
#define DAC_SIGNAL_DMA_DAC2_CHANNEL		DMA_Channel_7

/**
 * Signal result enumeration
 * 
 * Parameters:
 * 	- TM_DAC_SIGNAL_Result_Ok:
 * 		Everything OK
 * 	- TM_DAC_SIGNAL_Result_Error:
 * 		An error occured
 * 	- TM_DAC_SIGNAL_Result_TimerNotValid:
 * 		Used timer for DMA and DAC request is not valid
 */
typedef enum {
	TM_DAC_SIGNAL_Result_Ok = 0x00,
	TM_DAC_SIGNAL_Result_Error,
	TM_DAC_SIGNAL_Result_TimerNotValid
} TM_DAC_SIGNAL_Result_t;

/**
 * Signal enumeration
 * 
 * Parameters:
 * 	- TM_DAC_SIGNAL_Signal_Sinus:
 * 		Select sinus signal on DAC output
 * 	- TM_DAC_SIGNAL_Signal_Triangle:
 * 		Select triangle signal on DAC output
 * 	- TM_DAC_SIGNAL_Signal_Sawtooth:
 * 		Select sawtooth signal on DAC output
 * 	- TM_DAC_SIGNAL_Signal_Square:
 * 		Use square signal on DAC output
 */
typedef enum {
	TM_DAC_SIGNAL_Signal_Sinus = 0x00,
	TM_DAC_SIGNAL_Signal_Triangle = 0x01,
	TM_DAC_SIGNAL_Signal_Sawtooth = 0x02,
	TM_DAC_SIGNAL_Signal_Square = 0x03
} TM_DAC_SIGNAL_Signal_t;

/**
 * DAC used for signal
 * 
 * Parameters:
 * 	- TM_DAC1:
 * 		Use DAC1 for specific settings
 * 	- TM_DAC2:
 * 		Use DAC2 for specific settings
 */
typedef enum {
	TM_DAC1 = 0x00,
	TM_DAC2 = 0x01
} TM_DAC_SIGNAL_Channel_t;

/**
 * Initialize DAC output pin and prepare to work
 * 
 * Parameters:
 * 	- TM_DAC_SIGNAL_Channel_t DACx:
 * 		DAC channel you will use. Parameter is member of TM_DAC_SIGNAL_Channel_t enumeration
 * 	- TIM_TypeDef* TIMx:
 * 		Timer used for DMA and DAC output generation.
 *		TIM2, TIM4, TIM5, TIM6, TIM7 and TIM8 can be used.
 * 		
 * Member of TM_DAC_SIGNAL_Result_t is returned
 */
extern TM_DAC_SIGNAL_Result_t TM_DAC_SIGNAL_Init(TM_DAC_SIGNAL_Channel_t DACx, TIM_TypeDef* TIMx);

/**
 * Set output signal type with specific frequency
 * 
 * Parameters:
 * 	- TM_DAC_SIGNAL_Channel_t DACx:
 * 		DAC channel you will use. Parameter is member of TM_DAC_SIGNAL_Channel_t enumeration
 * 	- TM_DAC_SIGNAL_Signal_t signal_type:
 * 		Signal type you will set for specific dac channel. Parameter is member of TM_DAC_SIGNAL_Signal_t enumeration
 * 	- uint32_t frequency:
 * 		Signal's frequency
 * 		
 * Member of TM_DAC_SIGNAL_Result_t is returned
 */
extern TM_DAC_SIGNAL_Result_t TM_DAC_SIGNAL_SetSignal(TM_DAC_SIGNAL_Channel_t DACx, TM_DAC_SIGNAL_Signal_t signal_type, double frequency);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
