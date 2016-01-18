/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/04/library-56-extend-spi-with-dma-for-stm32f4xx
 * @version v1.1.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   DMA functionality for TM SPI library
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
#ifndef TM_SPI_DMA_H
#define TM_SPI_DMA_H 111

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_SPI_DMA
 * @brief    DMA functionality for @ref TM_SPI library - http://stm32f4-discovery.com/2015/04/library-56-extend-spi-with-dma-for-stm32f4xx
 * @{
 *
 * This library allows you to use DMA with SPI peripheral.
 *
 * It can send (TX only), receive (RX only) or transmit (TX and RX) data over SPI with DMA feature.
 *
 * \par 
 *
 * @note All possible DMA Streams and Channels for SPI DMA can be found in STM32F4xx Reference manual.
 *
 * Default DMA streams and channels:
 *
@verbatim
SPIx     | DMA  | DMA TX Stream | DMA TX Channel | DMA RX Stream | DMA RX Channel
                                                   
SPI1     | DMA2 | DMA Stream 3  | DMA Channel 3  | DMA Stream 2  | DMA Channel 3 
SPI2     | DMA1 | DMA Stream 4  | DMA Channel 0  | DMA Stream 3  | DMA Channel 0
SPI3     | DMA1 | DMA Stream 5  | DMA Channel 0  | DMA Stream 0  | DMA Channel 0 
SPI4     | DMA2 | DMA Stream 1  | DMA Channel 4  | DMA Stream 0  | DMA Channel 4 
SPI5     | DMA2 | DMA Stream 6  | DMA Channel 7  | DMA Stream 5  | DMA Channel 7 
SPI6     | DMA2 | DMA Stream 5  | DMA Channel 1  | DMA Stream 6  | DMA Channel 0 
@endverbatim
 *
 * \par Changelog
 *
@verbatim
 Version 1.1.1
  - August 11, 2015
  - Fixed bug with default TX Stream value for SPI4

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
 - TM DMA
 - TM SPI
 - stdlib.h
@endverbatim
 */
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "defines.h"
#include "tm_stm32f4_dma.h"
#include "tm_stm32f4_spi.h"
#include "stdlib.h"

/**
 * @defgroup TM_SPI_DMA_Macros
 * @brief    Library defines
 * @{
 */

/* SPI1 TX and RX default settings */
#ifndef SPI1_DMA_TX_STREAM
#define SPI1_DMA_TX_STREAM    DMA2_Stream3
#define SPI1_DMA_TX_CHANNEL   DMA_Channel_3
#endif
#ifndef SPI1_DMA_RX_STREAM
#define SPI1_DMA_RX_STREAM    DMA2_Stream2
#define SPI1_DMA_RX_CHANNEL   DMA_Channel_3
#endif

/* SPI2 TX and RX default settings */
#ifndef SPI2_DMA_TX_STREAM
#define SPI2_DMA_TX_STREAM    DMA1_Stream4
#define SPI2_DMA_TX_CHANNEL   DMA_Channel_0
#endif
#ifndef SPI2_DMA_RX_STREAM
#define SPI2_DMA_RX_STREAM    DMA1_Stream3
#define SPI2_DMA_RX_CHANNEL   DMA_Channel_0
#endif

/* SPI3 TX and RX default settings */
#ifndef SPI3_DMA_TX_STREAM
#define SPI3_DMA_TX_STREAM    DMA1_Stream5
#define SPI3_DMA_TX_CHANNEL   DMA_Channel_0
#endif
#ifndef SPI3_DMA_RX_STREAM
#define SPI3_DMA_RX_STREAM    DMA1_Stream0
#define SPI3_DMA_RX_CHANNEL   DMA_Channel_0
#endif

/* SPI4 TX and RX default settings */
#ifndef SPI4_DMA_TX_STREAM
#define SPI4_DMA_TX_STREAM    DMA2_Stream1
#define SPI4_DMA_TX_CHANNEL   DMA_Channel_4
#endif
#ifndef SPI4_DMA_RX_STREAM
#define SPI4_DMA_RX_STREAM    DMA2_Stream0
#define SPI4_DMA_RX_CHANNEL   DMA_Channel_4
#endif

/* SPI5 TX and RX default settings */
#ifndef SPI5_DMA_TX_STREAM
#define SPI5_DMA_TX_STREAM    DMA2_Stream6
#define SPI5_DMA_TX_CHANNEL   DMA_Channel_7
#endif
#ifndef SPI5_DMA_RX_STREAM
#define SPI5_DMA_RX_STREAM    DMA2_Stream5
#define SPI5_DMA_RX_CHANNEL   DMA_Channel_7
#endif

/* SPI6 TX and RX default settings */
#ifndef SPI6_DMA_TX_STREAM
#define SPI6_DMA_TX_STREAM    DMA2_Stream5
#define SPI6_DMA_TX_CHANNEL   DMA_Channel_1
#endif
#ifndef SPI6_DMA_RX_STREAM
#define SPI6_DMA_RX_STREAM    DMA2_Stream6
#define SPI6_DMA_RX_CHANNEL   DMA_Channel_1
#endif

/**
 * @}
 */
 
/**
 * @defgroup TM_SPI_DMA_Typedefs
 * @brief    Library Typedefs
 * @{
 */
/* Typedefs here */
/**
 * @}
 */

/**
 * @defgroup TM_SPI_DMA_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes SPI peripheral DMA
 * @note   This function initializes TX and RX DMA streams for SPI
 *
 * @note   SPI HAVE TO be previously initialized using @ref TM_SPI library
 * @param  *SPIx: Pointer to SPI peripheral where you want to enable DMA
 * @retlva None 
 */
void TM_SPI_DMA_Init(SPI_TypeDef* SPIx);

/**
 * @brief  Initializes SPI DMA functionality with custom DMA stream and channel options
 * @note   SPI HAVE TO be previously initialized using @ref TM_USART library
 *
 * @note   Use this function only in case default Stream and Channel settings are not good for you
 * @param  *SPIx: Pointer to SPIx where you want to set custom DMA streams and channels
 * @param  *TX_Stream: Pointer to DMAy_Streamx, where y is DMA (1 or 2) and x is Stream (0 to 7)
 * @param  TX_Channel: Select DMA TX channel for your SPI in specific DMA Stream
 * @param  *RX_Stream: Pointer to DMAy_Streamx, where y is DMA (1 or 2) and x is Stream (0 to 7)
 * @param  RX_Channel: Select DMA RX channel for your SPI in specific DMA Stream
 * @retval None
 */
void TM_SPI_DMA_InitWithStreamAndChannel(SPI_TypeDef* SPIx, DMA_Stream_TypeDef* TX_Stream, uint32_t TX_Channel, DMA_Stream_TypeDef* RX_Stream, uint32_t RX_Channel);

/**
 * @brief  Deinitializes SPI DMA functionality
 * @param  *SPIx: Pointer to SPIx where you want to disable DMA mode
 * @retval None
 */
void TM_SPI_DMA_Deinit(SPI_TypeDef* SPIx);

/**
 * @brief  Transmits (exchanges) data over SPI with DMA
 * @note   Try not to use local variables pointers for DMA memory as TX and RX Buffers
 * @param  *SPIx: Pointer to SPIx where DMA transmission will happen
 * @param  *TX_Buffer: Pointer to TX_Buffer where DMA will take data to sent over SPI.
 *            Set this parameter to NULL, if you want to sent "0x00" and only receive data into *RX_Buffer pointer
 * @param  *RX_Buffer: Pointer to RX_Buffer where DMA will save data from SPI.
 *            Set this parameter to NULL, if you don't want to receive any data, only sent from TX_Buffer
 * @param  count: Number of bytes to be send/received over SPI with DMA
 * @retval Transmission started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 */
uint8_t TM_SPI_DMA_Transmit(SPI_TypeDef* SPIx, uint8_t* TX_Buffer, uint8_t* RX_Buffer, uint16_t count);

/**
 * @brief  Sends data over SPI without receiving data back using DMA
 * @note   Try not to use local variables pointers for DMA memory as TX and RX Buffers
 * @param  *SPIx: Pointer to SPIx where DMA transmission will happen
 * @param  *TX_Buffer: Pointer to TX_Buffer where DMA will take data to sent over SPI
 * @param  count: Number of bytes to be send/received over SPI with DMA
 * @retval Sending started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 * @note   Defined as macro for faster execution
 */
#define TM_SPI_DMA_Send(SPIx, TX_Buffer, count)   (TM_SPI_DMA_Transmit(SPIx, TX_Buffer, NULL, count))

/**
 * @brief  Sends dummy byte (0x00) over SPI to receive data back from slave over DMA
 * @note   Try not to use local variables pointers for DMA memory as TX and RX Buffers
 * @param  SPIx: Pointer to SPIx where DMA transmission will happen
 * @param  RX_Buffer: Pointer to RX_Buffer where DMA will save data from SPI
 * @param  count: Number of bytes to be received over SPI with DMA
 * @retval Receiving started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 * @note   Defined as macro for faster execution
 */
#define TM_SPI_DMA_Receive(SPIx, RX_Buffer, count)   (TM_SPI_DMA_Transmit(SPIx, NULL, RX_Buffer, count))

/**
 * @brief  Sends one byte value multiple times over SPI with DMA
 * @param  SPIx: Pointer to SPIx where DMA transmission will happen
 * @param  value: Byte value to be sent
 * @param  count: Number of bytes with value of @arg value will be sent
 * @retval Transmission started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 */
uint8_t TM_SPI_DMA_SendByte(SPI_TypeDef* SPIx, uint8_t value, uint16_t count);

/**
 * @brief  Sends one half word value multiple times over SPI with DMA
 * @param  SPIx: Pointer to SPIx where DMA transmission will happen
 * @param  value: Byte value to be sent
 * @param  count: Number of half words with value of @arg value will be sent
 * @retval Transmission started status:
 *            - 0: DMA has not started with sending data
 *            - > 0: DMA has started with sending data
 */
uint8_t TM_SPI_DMA_SendHalfWord(SPI_TypeDef* SPIx, uint16_t value, uint16_t count);

/**
 * @brief  Checks if SPI DMA is still sending/receiving data
 * @param  *SPIx: Pointer to SPIx where you want to enable DMA TX mode
 * @retval Sending status:
 *            - 0: SPI DMA does not sending any more
 *            - > 0: SPI DMA is still sending data 
 */
uint8_t TM_SPI_DMA_Working(SPI_TypeDef* SPIx);

/**
 * @brief  Gets TX DMA stream for specific SPI 
 * @param  *SPIx: Pointer to SPIx peripheral where you want to get TX stream
 * @retval Pointer to SPI DMA TX stream
 */
DMA_Stream_TypeDef* TM_SPI_DMA_GetStreamTX(SPI_TypeDef* SPIx);

/**
 * @brief  Gets RX DMA stream for specific SPI 
 * @param  *SPIx: Pointer to SPIx peripheral where you want to get RX stream
 * @retval Pointer to SPI DMA RX stream
 */
DMA_Stream_TypeDef* TM_SPI_DMA_GetStreamRX(SPI_TypeDef* SPIx);

/**
 * @brief  Enables DMA stream interrupts for specific SPI
 * @param  *SPIx: Pointer to SPIx peripheral where you want to enable DMA stream interrupts
 * @retval None
 */
void TM_SPI_DMA_EnableInterrupts(SPI_TypeDef* SPIx);

/**
 * @brief  Disables DMA stream interrupts for specific SPI
 * @param  *SPIx: Pointer to SPIx peripheral where you want to disable DMA stream interrupts
 * @retval None
 */
void TM_SPI_DMA_DisableInterrupts(SPI_TypeDef* SPIx);

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
