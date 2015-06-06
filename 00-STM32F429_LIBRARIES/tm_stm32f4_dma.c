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

#define GET_STREAM_NUMBER_DMA1(stream)    (((stream) - DMA1_Stream0) / (0x18))
#define GET_STREAM_NUMBER_DMA2(stream)    (((stream) - DMA2_Stream0) / (0x18))

void TM_DMA_ClearFlags(DMA_Stream_TypeDef* DMA_Stream) {
/*	uint32_t value;
	uint32_t* location;
	uint32_t stream_number;
	*/
	/* Check stream value */
/*	if (DMA_Stream < DMA2_Stream0) {
		location = (uint32_t *)&DMA1->LIFCR;
		stream_number = GET_STREAM_NUMBER_DMA1(DMA_Stream);
	} else {
		location = (uint32_t *)&DMA2->LIFCR;
		stream_number = GET_STREAM_NUMBER_DMA2(DMA_Stream);
	}*/
	
	/* Clear flags */
	DMA_DeInit(DMA_Stream);
}
