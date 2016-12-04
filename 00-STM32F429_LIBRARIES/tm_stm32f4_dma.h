/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/06/library-63-dma-for-stm32f4xx
 * @version v1.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   DMA library for STM32F4xx for several purposes
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
#ifndef TM_DMA_H
#define TM_DMA_H 110

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_DMA
 * @brief    DMA library for STM32F4xx for several purposes - http://stm32f4-discovery.com/2015/06/library-63-dma-for-stm32f4xx
 * @{
 *
 * This is a generic DMA library for STM32F4xx devices.
 * 
 * It allows you to easily enable interrupts for DMA streams, checking flags and more.
 * 
 * It is also designed in a ways that all interrupts that happens are handled by this library. No worries for correct stream handler anymore!
 *
 * \par Customization
 * 
 * Library implements ALL stream handlers (DMA1,2, streams 0 to 7, together is this 14 stream handlers) but some of my libraries uses default stream handler, like FATFS.
 * FATFS uses DMA2_Stream3 for interrupts for SDIO and in my FATFS library is written function name DMA2_Stream3_IRQHandler(). 
 *
 * DMA library also features this function and now if you use DMA library and FATFS library, you will get error for "Multiple declarations...".
 *
 * To prevent this link errors, you can open defines.h configuration file and add defines like below:
 *
@verbatim
//Disable DMA2 Stream3 IRQ handler for TM DMA library
#define DMA2_STREAM3_DISABLE_IRQHANDLER

//For all other DMAs and STREAMS, syntax is similar:
#define DMAx_STREAMy_DISABLE_IRQHANDLER
//Where X is 1 or 2 for DMA1 or DMA2 and y is 0 to 7 for STREAM0 to STREAM7 on specific DMA
@endverbatim
 *
 * \par Handling interrupts
 * 
 * Every stream on DMA can make 5 interrupts. My library is designed in a way that specific callback is called for each interrupt type.
 * Check functions section for more informations
 *
 * \par Changelog
 *
@verbatim
 Version 1.1
  - June 13, 2015
  - Added support for clearing DMA interrupt flags 
	
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx DMA
 - MISC
 - defines.h
 - attributes.h
@endverbatim
 */

#include "stm32f4xx.h"
#include "tm_stm32f4_dma.h"
#include "misc.h"
#include "defines.h"
#include "attributes.h"

/**
 * @defgroup TM_DMA_Macros
 * @brief    Library defines
 * @{
 */

/**
 * @brief  DMA macros for interrupt flags
 */
#define DMA_FLAG_TCIF    ((uint32_t)0x00000020) /*!< DMA stream transfer complete */
#define DMA_FLAG_HTIF    ((uint32_t)0x00000010) /*!< DMA stream half transfer complete */
#define DMA_FLAG_TEIF    ((uint32_t)0x00000008) /*!< DMA stream transfer error */
#define DMA_FLAG_DMEIF   ((uint32_t)0x00000004) /*!< DMA stream direct mode error */
#define DMA_FLAG_FEIF    ((uint32_t)0x00000001) /*!< DMA stream FIFO error */
#define DMA_FLAG_ALL     ((uint32_t)0x0000003D) /*!< DMA stream all flags */

/* DMA1 preemption priority */
#ifndef DMA1_NVIC_PREEMPTION_PRIORITY
#define DMA1_NVIC_PREEMPTION_PRIORITY   0x01
#endif

/* DMA2 preemption priority */
#ifndef DMA2_NVIC_PREEMPTION_PRIORITY
#define DMA2_NVIC_PREEMPTION_PRIORITY   0x01
#endif

/**
 * @}
 */
 
/**
 * @defgroup TM_DMA_Typedefs
 * @brief    Library Typedefs
 * @{
 */
/* Typedefs here */
/**
 * @}
 */

/**
 * @defgroup TM_DMA_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Clears all flags for selected DMA stream
 * @param  *DMA_Stream: Pointer to @ref DMA_Stream_TypeDef DMA stream where you want to clear flags 
 * @retval None
 */
void TM_DMA_ClearFlags(DMA_Stream_TypeDef* DMA_Stream);

/** 
 * @brief  Clears selected DMA interrupt flag
 * @param  *DMA_Stream: Pointer to @ref DMA_Stream_TypeDef DMA stream where you want to clear flags
 * @param  flags: Flag(s) which you wanna clear:
 *           - DMA_FLAG_TCIF: Transfer complete interrupt flag
 *           - DMA_FLAG HTIF: Half transfer complete interrupt flag
 *           - DMA_FLAG_TEIF: Transfer error interrupt flag
 *           - DMA_FLAG_DMEIF: Direct mode error interrupt flag
 *           - DMA_FLAG_FEIF: FIFO error interrupt flag
 *           - DMA_FLAG_ALL: All flags
 * @retval None
 */
void TM_DMA_ClearFlag(DMA_Stream_TypeDef* DMA_Stream, uint32_t flags);

/**
 * @brief  Enables interrupts for DMA stream
 * @note   It adds IRQ to NVIC and enables all possible DMA STREAM interrupts
 * @param  *DMA_Stream: Pointer to DMA stream where to enable interrupts
 * @retval None
 */
void TM_DMA_EnableInterrupts(DMA_Stream_TypeDef* DMA_Stream);

/**
 * @brief  Disables interrupts for DMA stream
 * @note   It adds IRQ to NVIC and enables all possible DMA STREAM interrupts
 * @param  *DMA_Stream: Pointer to DMA stream where to disable interrupts
 * @retval None
 */
void TM_DMA_DisableInterrupts(DMA_Stream_TypeDef* DMA_Stream);

/**
 * @brief  Transfer complete callback
 * @note   This function is called when interrupt for specific stream happens
 * @param  *DMA_Stream: Pointer to DMA stream where interrupt happens
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_DMA_TransferCompleteHandler(DMA_Stream_TypeDef* DMA_Stream);

/**
 * @brief  Half transfer complete callback
 * @note   This function is called when interrupt for specific stream happens
 * @param  *DMA_Stream: Pointer to DMA stream where interrupt happens
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_DMA_HalfTransferCompleteHandler(DMA_Stream_TypeDef* DMA_Stream);

/**
 * @brief  Transfer error callback
 * @note   This function is called when interrupt for specific stream happens
 * @param  *DMA_Stream: Pointer to DMA stream where interrupt happens
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_DMA_TransferErrorHandler(DMA_Stream_TypeDef* DMA_Stream);

/**
 * @brief  Direct mode error callback
 * @note   This function is called when interrupt for specific stream happens
 * @param  *DMA_Stream: Pointer to DMA stream where interrupt happens
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_DMA_DirectModeErrorHandler(DMA_Stream_TypeDef* DMA_Stream);

/**
 * @brief  FIFO error error callback
 * @note   This function is called when interrupt for specific stream happens
 * @param  *DMA_Stream: Pointer to DMA stream where interrupt happens
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void TM_DMA_FIFOErrorHandler(DMA_Stream_TypeDef* DMA_Stream);

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
