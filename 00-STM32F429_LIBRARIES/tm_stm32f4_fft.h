/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/05/library-62-fast-fourier-transform-fft-for-stm32f4xx
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   FFT library for float 32 and Cortex-M4 little endian
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
#ifndef TM_FFT_H
#define TM_FFT_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_FFT
 * @brief    FFT library for STM32F4xx devices - http://stm32f4-discovery.com/2015/05/library-62-fast-fourier-transform-fft-for-stm32f4xx
 * @{
 *
 * This library allows you to calculate FFT in your signal.
 * 
 * For more info about FFT and how it works on Cortex-M4, you should take a look at ARM DSP documentation
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
 - defines.h
 - ARM MATH
 - ARM CONST STRUCTS
@endverbatim
 */

#include "stm32f4xx.h"
#include "defines.h"

#include "arm_math.h"
#include "arm_const_structs.h"

/**
 * @defgroup TM_FFT_Macros
 * @brief    Library defines
 * @{
 */
 
/* Memory allocation function */
#ifndef LIB_ALLOC_FUNC
#define LIB_ALLOC_FUNC    malloc
#endif

/* Memory free function */
#ifndef LIB_FREE_FUNC
#define LIB_FREE_FUNC     free
#endif

/**
 * @}
 */
 
/**
 * @defgroup TM_FFT_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  FFT main structure for 32-bit float
 */
typedef struct {
	float32_t* Input;               /*!< Pointer to data input buffer. Its length must be 2 * FFT_Size */
	float32_t* Output;              /*!< Pointer to data output buffer. Its length must be FFT_Size */
	uint16_t FFT_Size;              /*!< FFT size in units of samples. This parameter can be a value of 2^n where n is between 4 and 12 */
	uint8_t UseMalloc;              /*!< Set to 1 when malloc is used for memory allocation for buffers. Meant for private use */
	uint16_t Count;                 /*!< Number of samples in buffer when using @ref TM_FFT_AddToBuffer function. Meant for private use */
	const arm_cfft_instance_f32* S; /*!< Pointer to @ref arm_cfft_instance_f32 structure. Meant for private use */
	float32_t MaxValue;             /*!< Max value in FTT result after calculation */
	uint32_t MaxIndex;              /*!< Index in output array where max value happened */
} TM_FFT_F32_t;

/**
 * @}
 */

/**
 * @defgroup TM_FFT_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes and prepares FFT structure for signal operations
 * @param  *FFT: Pointer to empty @ref TM_FFT_F32_t structure for FFT
 * @param  FFT_Size: Number of samples to be used for FFT calculation
 *            This parameter can be a value of 2^n where n is between 4 and 12, so any power of 2 between 16 and 4096
 * @param  use_malloc: Set parameter to 1, if you want to use HEAP memory and @ref malloc to allocate input and output buffers
 * @note   It is recommended to use malloc for allocation, because FFT input and output buffers differs in length
 * @retval Initialization status:
 *            - 0: Initialized OK, ready to use
 *            - 1: Input FFT SIZE is not valid
 *            - 2: Malloc failed with allocating input data buffer
 *            - 3: Malloc failed with allocating output data buffer. If input data buffer is allocated, it will be free if this is returned.
 */
uint8_t TM_FFT_Init_F32(TM_FFT_F32_t* FFT, uint16_t FFT_Size, uint8_t use_malloc);

/**
 * @brief  Sets input and output buffers for FFT calculations
 * @note   Use this function only if you set @arg use_malloc parameter to zero in @ref TM_FFT_Init_F32 function
 * @param  *FFT: Pointer to @ref TM_FFT_F32_t structure where buffers will be set
 * @param  *InputBuffer: Pointer to buffer of type float32_t with FFT_Size * 2 length
 * @param  *OutputBuffer: Pointer to buffer of type float32_t with FFT_Size length
 * @retval None
 */
void TM_FFT_SetBuffers_F32(TM_FFT_F32_t* FFT, float32_t* InputBuffer, float32_t* OutputBuffer);

/**
 * @brief  Adds new sample to input buffer in FFT array 
 * @param  *FFT: Pointer to @ref TM_FFT_F32_t structure where new sample will be added
 * @param  sampleValue: A new sample to be added to buffer, real part. Imaginary part will be set to 0
 * @retval FFT calculation status:
 *            - 0: Input buffer is not full yet
 *            - > 0: Input buffer is full and samples are ready to be calculated
 */
uint8_t TM_FFT_AddToBuffer(TM_FFT_F32_t* FFT, float32_t sampleValue);

/**
 * @brief  Processes and calculates FFT from InputBuffer and saves data to Output buffer
 * @note   This function also calculates max value and max index in array where max value happens
 * @param  *FFT: Pointer to @ref TM_FFT_F32_t where FFT calculation will happen
 * @retval None
 */
void TM_FFT_Process_F32(TM_FFT_F32_t* FFT);

/**
 * @brief  Free input and output buffers
 * @note   This function has sense only, if you used @ref malloc for memory allocation when you called @ref TM_FFT_Init_F32 function
 * @param  *FFT: Pointer to @ref TM_FFT_F32_t structure where buffers will be free
 * @retval None
 */
void TM_FFT_Free_F32(TM_FFT_F32_t* FFT);

/**
 * @brief  Gets max value from already calculated FFT result
 * @param  FFT: Pointer to @ref TM_FFT_F32_t structure where max value should be checked
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_FFT_GetMaxValue(FFT)            ((FFT)->MaxValue)

/**
 * @brief  Gets index value where max value happens from already calculated FFT result
 * @param  FFT: Pointer to @ref TM_FFT_F32_t structure where max index at max value should be checked
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_FFT_GetMaxIndex(FFT)            ((FFT)->MaxIndex)

/**
 * @brief  Gets FFT size in units of samples length
 * @param  FFT: Pointer to @ref TM_FFT_F32_t structure where FFT size will be checked
 * @retval FFT size in units of elements for calculation
 * @note   Defined as macro for faster execution
 */
#define TM_FFT_GetFFTSize(FFT)             ((FFT)->FFT_Size)

/**
 * @brief  Gets FFT result value from output buffer at given index
 * @param  FFT: Pointer to @ref TM_FFT_F32_t structure where FFT output sample will be returned
 * @param  index: Index in buffer where result will be returned. Valid input is between 0 and FFT_Size - 1
 * @retval Value at given index
 * @note   Defined as macro for faster execution
 */
#define TM_FFT_GetFromBuffer(FFT, index)   ((FFT)->Output[(uint16_t)(index)])

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
