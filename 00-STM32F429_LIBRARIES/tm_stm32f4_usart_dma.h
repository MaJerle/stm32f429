/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/04/library-55-extend-usart-with-tx-dma
 * @version v1.2.3
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   DMA TX functionality for TM USART library
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
#ifndef TM_USART_DMA_H
#define TM_USART_DMA_H 123

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_USART_DMA
 * @brief    DMA TX functionality for TM USART library - http://stm32f4-discovery.com/2015/04/library-55-extend-usart-with-tx-dma
 * @{
 *
 * This library allows you to send data over USART with DMA feature.
 *
 * It is great feature because you can do other stuff while DMA sends data to USART.
 *
 * It is designed only for TX data from MCU to other world. For RX, @ref TM_USART library already
 * uses RXNE (RX Not Empty) interrupts when data is available and there is no need for RX DMA too right now.
 *
 * \par Default stream and channel settings
 *
 * Library uses default settings for DMA TX Stream and Channel settings.
 * But it may happen that your USART DMA is on the same stream and DMA as something other you need.
 * You are able to use custom Stream and Channel using @ref TM_USART_DMA_InitWithStreamAndChannel() function.
 *
 * @note All possible DMA Streams and Channels for USART TX DMA can be found in STM32F4xx Reference manual.
 *
 * Default DMA streams and channels:
 *
@verbatim
USARTx     | DMA  | DMA Stream   | DMA Channel

USART1     | DMA2 | DMA Stream 7 | DMA Channel 4
USART2     | DMA1 | DMA Stream 6 | DMA Channel 4
USART3     | DMA1 | DMA Stream 3 | DMA Channel 4
UART4      | DMA1 | DMA Stream 4 | DMA Channel 4
UART5      | DMA1 | DMA Stream 7 | DMA Channel 4
USART6     | DMA2 | DMA Stream 6 | DMA Channel 5
UART7      | DMA1 | DMA Stream 1 | DMA Channel 5
UART8      | DMA1 | DMA Stream 0 | DMA Channel 5
@endverbatim
 *
 * \par Changelog
 *
@verbatim
 Version 1.3
  - TM_USART_DMA_Working() function now returns > 0 also when USART works, not only when DMA works.
     Requires updated USART library  

 Version 1.2.1
  - Added separate function for enabling/disabling USART DMA interrupts
  
 Version 1.2
  - June 13, 2015
  - Added support for DMA interrupts for USART
  
 Version 1.1
  - June 06, 2015
  - Added TM DMA library support for future purpose
  
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx DMA
 - defines.h
 - TM USART
 - TM DMA
 - string.h
@endverbatim
 */

#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "defines.h"
#include "tm_stm32f4_dma.h"
#include "tm_stm32f4_usart.h"
#include "string.h"

/* Check USART library version */
#if TM_USART_H < 250
#error "TM USART library version must be greater or equal to 2.5.0. Please redownload TM USART library!"
#endif

/**
 * @defgroup TM_USART_DMA_Macros
 * @brief    Library defines
 * @{
 */

/* Default DMA Stream and Channel for USART1 */
#ifndef USART1_DMA_TX_STREAM
#define USART1_DMA_TX_STREAM      DMA2_Stream7
#define USART1_DMA_TX_CHANNEL     DMA_Channel_4
#endif

/* Default DMA Stream and Channel for USART2 */
#ifndef USART2_DMA_TX_STREAM
#define USART2_DMA_TX_STREAM      DMA1_Stream6
#define USART2_DMA_TX_CHANNEL     DMA_Channel_4
#endif

/* Default DMA Stream and Channel for USART3 */
#ifndef USART3_DMA_TX_STREAM
#define USART3_DMA_TX_STREAM      DMA1_Stream3
#define USART3_DMA_TX_CHANNEL     DMA_Channel_4
#endif

/* Default DMA Stream and Channel for UART4 */
#ifndef UART4_DMA_TX_STREAM
#define UART4_DMA_TX_STREAM       DMA1_Stream4
#define UART4_DMA_TX_CHANNEL      DMA_Channel_4
#endif

/* Default DMA Stream and Channel for UART5 */
#ifndef UART5_DMA_TX_STREAM
#define UART5_DMA_TX_STREAM       DMA1_Stream7
#define UART5_DMA_TX_CHANNEL      DMA_Channel_4
#endif

/* Default DMA Stream and Channel for USART6 */
#ifndef USART6_DMA_TX_STREAM
#define USART6_DMA_TX_STREAM      DMA2_Stream6
#define USART6_DMA_TX_CHANNEL     DMA_Channel_5
#endif

/* Default DMA Stream and Channel for UART7 */
#ifndef UART7_DMA_TX_STREAM
#define UART7_DMA_TX_STREAM       DMA1_Stream1
#define UART7_DMA_TX_CHANNEL      DMA_Channel_5
#endif

/* Default DMA Stream and Channel for UART8 */
#ifndef UART8_DMA_TX_STREAM
#define UART8_DMA_TX_STREAM       DMA1_Stream0
#define UART8_DMA_TX_CHANNEL      DMA_Channel_5
#endif

/**
 * @}
 */
 
/**
 * @defgroup TM_USART_DMA_Typedefs
 * @brief    Library Typedefs
 * @{
 */
/* Typedefs here */
/**
 * @}
 */

/**
 * @defgroup TM_USART_DMA_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes USART DMA TX functionality
 * @note   USART HAVE TO be previously initialized using @ref TM_USART library
 * @param  *USARTx: Pointer to USARTx where you want to enable DMA TX mode
 * @retval None
 */
void TM_USART_DMA_Init(USART_TypeDef* USARTx);

/**
 * @brief  Initializes USART DMA TX functionality with custom DMA stream and Channel options
 * @note   USART HAVE TO be previously initialized using @ref TM_USART library
 *
 * @note   Use this function only in case default Stream and Channel settings are not good for you
 * @param  *USARTx: Pointer to USARTx where you want to enable DMA TX mode
 * @param  *DMA_Stream: Pointer to DMAy_Streamx, where y is DMA (1 or 2) and x is Stream (0 to 7)
 * @param  DMA_Channel: Select DMA channel for your USART in specific DMA Stream
 * @retval None
 */
void TM_USART_DMA_InitWithStreamAndChannel(USART_TypeDef* USARTx, DMA_Stream_TypeDef* DMA_Stream, uint32_t DMA_Channel);

/**
 * @brief  Deinitializes USART DMA TX functionality
 * @param  *USARTx: Pointer to USARTx where you want to disable DMA TX mode
 * @retval None
 */
void TM_USART_DMA_Deinit(USART_TypeDef* USARTx);

/**
 * @breif  Enables interrupts for DMA for USART streams
 * @note   USART DMA must be initialized first using @ref TM_USART_DMA_Init() or @ref TM_USART_DMA_InitWithStreamAndChannel() functions
 * @param  *USARTx: Pointer to USARTx where DMA interrupts will be enabled
 * @retval None
 */
void TM_USART_DMA_EnableInterrupts(USART_TypeDef* USARTx);

/**
 * @breif  Disables interrupts for DMA for USART streams
 * @param  *USARTx: Pointer to USARTx where DMA interrupts will be disabled
 * @retval None
 */
void TM_USART_DMA_DisableInterrupts(USART_TypeDef* USARTx);

/**
 * @brief  Gets poitner to DMA stream for desired USART 
 * @param  *USARTx: Pointer to USART where you wanna get its stream pointer
 * @retval Pointer to DMA stream for desired USART
 */
DMA_Stream_TypeDef* TM_USART_DMA_GetStream(USART_TypeDef* USARTx);

/**
 * @brief  Puts string to USART port with DMA
 * @note   Try not to use local variables pointers for DMA memory as parameter *str
 * @param  *USARTx: Pointer to USARTx peripheral you will use
 * @param  *str: Pointer to string to send over USART with DMA
 * @retval Sending started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 */
uint8_t TM_USART_DMA_Puts(USART_TypeDef* USARTx, char* str);

/**
 * @brief  Sends data over USART with DMA TX functionality
 * @note   Try not to use local variables pointers for DMA memory as parameter *str
 * @param  *USARTx: Pointer to USARTx to use for send
 * @param  *DataArray: Pointer to array of data to be sent over USART
 * @param  count: Number of data bytes to be sent over USART with DMA
 * @retval Sending started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 */
uint8_t TM_USART_DMA_Send(USART_TypeDef* USARTx, uint8_t* DataArray, uint16_t count);

/**
 * @brief  Checks if USART DMA TX is still sending data
 * @param  *USARTx: Pointer to USARTx where you want to check if DMA is still working
 * @retval Sending status:
 *            - 0: USART does not sending anymore
 *            - > 0: USART DMA is still sending data
 */
uint16_t TM_USART_DMA_Sending(USART_TypeDef* USARTx);

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
