/**
 *	DAC with DMA and TIM signal generator feature for STM32F4
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/09/library-36-dac-signal-generator-stm32f4
 *	@version 	v1.1
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
 * Version 1.1
 *	- March 12, 2015
 *	- Added support for my new GPIO library
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
/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/09/library-36-dac-signal-generator-stm32f4
 * @version v1.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   DAC with DMA and TIM signal generator feature for STM32F4
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
#ifndef TM_DAC_SIGNAL_H
#define TM_DAC_SIGNAL_H 110

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_DAC_SIGNAL
 * @brief    DAC with DMA and TIM signal generator feature for STM32F4 - http://stm32f4-discovery.com/2014/09/library-36-dac-signal-generator-stm32f4
 * @{
 *
 * \par Signals available
 *
 * Library supports 4 output signals:
 * 	- Square
 * 	- Sinus
 * 	- Triangle
 * 	- Sawtooth
 * 	
 * Signals work well to about 50kHz. You can use 2 DAC channel simultaneously.
 * 
 * Timers, available for signal generation are:
 * 	TIM2, TIM4, TIM5, TIM6, TIM7, TIM8
 * 
 * \par Pinout:
 * 	
@verbatim
DACx	STM32F4		Description
 	
DAC1	PA4			DAC Channel 1
DAC2	PA5			DAC Channel 2
@endverbatim
 * 	
 * Pins for DAC are fixed, analog and can not be changed
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
 - STM32F4xx DAC
 - STM32F4xx TIM
 - STM32F4xx DMA
 - defines.h
 - TM TIMER PROPERTIES
 - TM GPIO
@endverbatim
 */
 
#include "stm32f4xx.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dma.h"
#include "defines.h"
#include "tm_stm32f4_timer_properties.h"
#include "tm_stm32f4_gpio.h"

/**
 * @defgroup TM_DAC_SIGNAL_Macros
 * @brief    Library defines
 * @{
 */

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
 * @}
 */
 
/**
 * @defgroup TM_DAC_SIGNAL_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Signal result enumeration
 */
typedef enum {
	TM_DAC_SIGNAL_Result_Ok = 0x00,    /*!< Everything OK */
	TM_DAC_SIGNAL_Result_Error,        /*!< An error occurred */
	TM_DAC_SIGNAL_Result_TimerNotValid /*!< Used timer for DMA and DAC request is not valid */
} TM_DAC_SIGNAL_Result_t;

/**
 * @brief  Signals available 		
 */
typedef enum {
	TM_DAC_SIGNAL_Signal_Sinus = 0x00,    /*!< Select sinus signal on DAC output */
	TM_DAC_SIGNAL_Signal_Triangle = 0x01, /*!< Select triangle signal on DAC output */
	TM_DAC_SIGNAL_Signal_Sawtooth = 0x02, /*!< Select sawtooth signal on DAC output */
	TM_DAC_SIGNAL_Signal_Square = 0x03    /*!< Use square signal on DAC output */
} TM_DAC_SIGNAL_Signal_t;

/**
 * @brief  DAC used for signal
 */
typedef enum {
	TM_DAC1 = 0x00, /*!< Use DAC1 for specific settings */
	TM_DAC2 = 0x01  /*!< Use DAC2 for specific settings */
} TM_DAC_SIGNAL_Channel_t;

/**
 * @}
 */

/**
 * @defgroup TM_DAC_SIGNAL_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes DAC output pin and prepare to work
 * @param  DACx: DAC channel you will use. This parameter can be a value of @ref TM_DAC_SIGNAL_Channel_t enumeration
 * @param  *TIMx: Pointer to TIMER use for DMA treshold
 *            - TIM2, TIM4, TIM5, TIM6, TIM7, TIM8
 * @retval Member of @ref TM_DAC_SIGNAL_Result_t
 */
TM_DAC_SIGNAL_Result_t TM_DAC_SIGNAL_Init(TM_DAC_SIGNAL_Channel_t DACx, TIM_TypeDef* TIMx);

/**
 * @brief  Sets output signal type with specific frequency
 * @param  DACx: DAC channel you will use. This parameter can be a value of @ref TM_DAC_SIGNAL_Channel_t enumeration
 * @param  signal_type: Signal type you will set for specific dac channel. This parameter can be a value of @ref TM_DAC_SIGNAL_Signal_t enumeration
 * @param  frequency: Signal's frequency
 * @retval Member of @ref TM_DAC_SIGNAL_Result_t
 */
TM_DAC_SIGNAL_Result_t TM_DAC_SIGNAL_SetSignal(TM_DAC_SIGNAL_Channel_t DACx, TM_DAC_SIGNAL_Signal_t signal_type, double frequency);

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
