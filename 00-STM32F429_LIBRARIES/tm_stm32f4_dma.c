/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2015
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
#include "tm_stm32f4_dma.h"

/* Private defines for stream numbers */
#define GET_STREAM_NUMBER_DMA1(stream)    (((uint32_t)(stream) - (uint32_t)DMA1_Stream0) / (0x18))
#define GET_STREAM_NUMBER_DMA2(stream)    (((uint32_t)(stream) - (uint32_t)DMA2_Stream0) / (0x18))

/* Offsets for bits */
const static uint8_t DMA_Flags_Bit_Pos[4] = {
	0, 6, 16, 22
};
const static IRQn_Type DMA_IRQs[2][8] = {
	{
		DMA1_Stream0_IRQn, DMA1_Stream1_IRQn, DMA1_Stream2_IRQn, DMA1_Stream3_IRQn,
		DMA1_Stream4_IRQn, DMA1_Stream5_IRQn, DMA1_Stream6_IRQn, DMA1_Stream7_IRQn
	},
	{
		DMA2_Stream0_IRQn, DMA2_Stream1_IRQn, DMA2_Stream2_IRQn, DMA2_Stream3_IRQn,
		DMA2_Stream4_IRQn, DMA2_Stream5_IRQn, DMA2_Stream6_IRQn, DMA2_Stream7_IRQn
	}
};

void TM_DMA_ClearFlags(DMA_Stream_TypeDef* DMA_Stream) {
	/* Clear all flags */
	TM_DMA_ClearFlag(DMA_Stream, DMA_FLAG_ALL);
}

void TM_DMA_ClearFlag(DMA_Stream_TypeDef* DMA_Stream, uint32_t flag) {
	uint32_t location;
	uint32_t stream_number;

	/* Check stream value */
	if (DMA_Stream < DMA2_Stream0) {
		location = (uint32_t)&DMA1->LIFCR;
		stream_number = GET_STREAM_NUMBER_DMA1(DMA_Stream);
	} else {
		location = (uint32_t)&DMA2->LIFCR;
		stream_number = GET_STREAM_NUMBER_DMA2(DMA_Stream);
	}
	
	/* Get register offset */
	if (stream_number >= 4) {
		/* High registers for DMA clear */
		location += 4;
		
		/* Do offset for high DMA registers */
		stream_number -= 4;
	}
	
	/* Clear flags */
	*(__IO uint32_t *)location = (flag & DMA_FLAG_ALL) << DMA_Flags_Bit_Pos[stream_number];
}

uint32_t TM_DMA_GetFlags(DMA_Stream_TypeDef* DMA_Stream, uint32_t flag) {
	uint32_t stream_number = 0;
	uint32_t location = 0;
	uint32_t flags = 0;
	
	/* Check stream value */
	if (DMA_Stream < DMA2_Stream0) {
		location = (uint32_t)&DMA1->LISR;
		stream_number = GET_STREAM_NUMBER_DMA1(DMA_Stream);
	} else {
		location = (uint32_t)&DMA2->LISR;
		stream_number = GET_STREAM_NUMBER_DMA2(DMA_Stream);
	}
	
	/* Get register offset */
	if (stream_number >= 4) {
		/* High registers for DMA clear */
		location += 4;
		
		/* Do offset for high DMA registers */
		stream_number -= 4;
	}
	
	/* Get register value */
	flags =   *(__IO uint32_t *)location;
	flags >>= DMA_Flags_Bit_Pos[stream_number];
	flags &=  DMA_FLAG_ALL;
	
	/* Return value */
	return flags;
}

void TM_DMA_EnableInterrupts(DMA_Stream_TypeDef* DMA_Stream) {
	NVIC_InitTypeDef NVIC_InitStruct;
	uint32_t stream_number;
	
	/* Clear flags first */
	TM_DMA_ClearFlag(DMA_Stream, DMA_FLAG_ALL);

	/* Check stream value */
	if (DMA_Stream < DMA2_Stream0) {
		stream_number = GET_STREAM_NUMBER_DMA1(DMA_Stream);
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = DMA1_NVIC_PREEMPTION_PRIORITY;
		NVIC_InitStruct.NVIC_IRQChannel = DMA_IRQs[0][stream_number];
	} else {
		stream_number = GET_STREAM_NUMBER_DMA2(DMA_Stream);
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = DMA2_NVIC_PREEMPTION_PRIORITY;
		NVIC_InitStruct.NVIC_IRQChannel = DMA_IRQs[1][stream_number];
	}
	
	/* Fill NVIC */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = stream_number;
	
	/* Init NVIC */
	NVIC_Init(&NVIC_InitStruct);
	
	/* Enable DMA stream interrupts */
	DMA_Stream->CR |= DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE;
	DMA_Stream->FCR |= DMA_SxFCR_FEIE;
}

void TM_DMA_DisableInterrupts(DMA_Stream_TypeDef* DMA_Stream) {
	IRQn_Type IRQValue;
	
	/* Clear flags first */
	TM_DMA_ClearFlag(DMA_Stream, DMA_FLAG_ALL);

	/* Check stream value */
	if (DMA_Stream < DMA2_Stream0) {
		IRQValue = DMA_IRQs[0][GET_STREAM_NUMBER_DMA1(DMA_Stream)];
	} else {
		IRQValue = DMA_IRQs[0][GET_STREAM_NUMBER_DMA2(DMA_Stream)];
	}
	
	/* Disable NVIC */
	NVIC_DisableIRQ(IRQValue);
	
	/* Disable DMA stream interrupts */
	DMA_Stream->CR &= ~(DMA_SxCR_TCIE  | DMA_SxCR_HTIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE);
	DMA_Stream->FCR &= DMA_SxFCR_FEIE;
}


/*****************************************************************/
/*                 DMA INTERRUPT USER CALLBACKS                  */
/*****************************************************************/
__weak void TM_DMA_TransferCompleteHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* NOTE: This function should not be modified, when the callback is needed,
            the TM_DMA_TransferCompleteHandler could be implemented in the user file
	*/
}

__weak void TM_DMA_HalfTransferCompleteHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* NOTE: This function should not be modified, when the callback is needed,
            the TM_DMA_HalfTransferCompleteHandler could be implemented in the user file
	*/
}

__weak void TM_DMA_TransferErrorHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* NOTE: This function should not be modified, when the callback is needed,
            the TM_DMA_TransferErrorHandler could be implemented in the user file
	*/
}

__weak void TM_DMA_DirectModeErrorHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* NOTE: This function should not be modified, when the callback is needed,
            the TM_DMA_DirectModeErrorHandler could be implemented in the user file
	*/
}

__weak void TM_DMA_FIFOErrorHandler(DMA_Stream_TypeDef* DMA_Stream) {
	/* NOTE: This function should not be modified, when the callback is needed,
            the TM_DMA_FIFOErrorHandler could be implemented in the user file
	*/
}

/*****************************************************************/
/*                    DMA INTERNAL FUNCTIONS                     */
/*****************************************************************/
static void TM_DMA_INT_ProcessInterrupt(DMA_Stream_TypeDef* DMA_Stream) {
	/* Get DMA interrupt status flags */
	uint16_t flags = TM_DMA_GetFlags(DMA_Stream, DMA_FLAG_ALL);
	
	/* Clear flags */
	TM_DMA_ClearFlag(DMA_Stream, DMA_FLAG_ALL);
	
	/* Call user callback function */
	
	/* Check transfer complete flag */
	if ((flags & DMA_FLAG_TCIF) && (DMA_Stream->CR & DMA_SxCR_TCIE)) {
		TM_DMA_TransferCompleteHandler(DMA_Stream);
	}
	/* Check half-transfer complete flag */
	if ((flags & DMA_FLAG_HTIF) && (DMA_Stream->CR & DMA_SxCR_HTIE)) {
		TM_DMA_HalfTransferCompleteHandler(DMA_Stream);
	}
	/* Check transfer error flag */
	if ((flags & DMA_FLAG_TEIF) && (DMA_Stream->CR & DMA_SxCR_TEIE)) {
		TM_DMA_TransferErrorHandler(DMA_Stream);
	}
	/* Check direct error flag */
	if ((flags & DMA_FLAG_DMEIF) && (DMA_Stream->CR & DMA_SxCR_DMEIE)) {
		TM_DMA_DirectModeErrorHandler(DMA_Stream);
	}
	/* Check FIFO error flag */
	if ((flags & DMA_FLAG_FEIF) && (DMA_Stream->FCR & DMA_SxFCR_FEIE)) {
		TM_DMA_FIFOErrorHandler(DMA_Stream);
	}
}

/* Handle all DMA interrupt handlers possible */
#ifndef DMA1_STREAM0_DISABLE_IRQHANDLER
void DMA1_Stream0_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA1_Stream0);
}
#endif
#ifndef DMA1_STREAM1_DISABLE_IRQHANDLER
void DMA1_Stream1_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA1_Stream1);
}
#endif
#ifndef DMA1_STREAM2_DISABLE_IRQHANDLER
void DMA1_Stream2_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA1_Stream2);
}
#endif
#ifndef DMA1_STREAM3_DISABLE_IRQHANDLER
void DMA1_Stream3_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA1_Stream3);
}
#endif
#ifndef DMA1_STREAM4_DISABLE_IRQHANDLER
void DMA1_Stream4_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA1_Stream4);
}
#endif
#ifndef DMA1_STREAM5_DISABLE_IRQHANDLER
void DMA1_Stream5_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA1_Stream5);
}
#endif
#ifndef DMA1_STREAM6_DISABLE_IRQHANDLER
void DMA1_Stream6_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA1_Stream6);
}
#endif
#ifndef DMA1_STREAM7_DISABLE_IRQHANDLER
void DMA1_Stream7_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA1_Stream7);
}
#endif
#ifndef DMA2_STREAM0_DISABLE_IRQHANDLER
void DMA2_Stream0_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA2_Stream0);
}
#endif
#ifndef DMA2_STREAM1_DISABLE_IRQHANDLER
void DMA2_Stream1_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA2_Stream1);
}
#endif
#ifndef DMA2_STREAM2_DISABLE_IRQHANDLER
void DMA2_Stream2_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA2_Stream2);
}
#endif
#ifndef DMA2_STREAM3_DISABLE_IRQHANDLER
void DMA2_Stream3_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA2_Stream3);
}
#endif
#ifndef DMA2_STREAM4_DISABLE_IRQHANDLER
void DMA2_Stream4_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA2_Stream4);
}
#endif
#ifndef DMA2_STREAM5_DISABLE_IRQHANDLER
void DMA2_Stream5_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA2_Stream5);
}
#endif
#ifndef DMA2_STREAM6_DISABLE_IRQHANDLER
void DMA2_Stream6_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA2_Stream6);
}
#endif
#ifndef DMA2_STREAM7_DISABLE_IRQHANDLER
void DMA2_Stream7_IRQHandler(void) {
	/* Call user function */
	TM_DMA_INT_ProcessInterrupt(DMA2_Stream7);
}
#endif
