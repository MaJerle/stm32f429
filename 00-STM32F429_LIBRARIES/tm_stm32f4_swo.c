/**	
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
#include "tm_stm32f4_swo.h"

/* If SWO debug is enabled */
#if SWO_DEBUG_ENABLED == 1

/* File struct for SWO output */
FILE TM_SWO_File;

int TM_SWO_fputc(int ch, FILE *f) {
	/* Check parameters */
	if (
		(ITM->TCR & ITM_TCR_ITMENA_Msk) &&      /* ITM enabled */
		(ITM->TER & (1UL << 0))					/* ITM Port #0 enabled */
	) {
		while (ITM->PORT[0].u32 == 0);			/* Wait for available */
		ITM->PORT[0].u8 = (uint8_t) ch;			/* Send character */
		
		/* Return written character */
		return ch;
	}
	
	/* Return EOF to stop writing */
	return -1;
}

void TM_SWO_Init(void) {
	/* Enable SWO output */
	DBGMCU->CR = 0x00000020;
	
	/* Link output function for output stream functionality */
	TM_STDIO_SetOutputFunction(&TM_SWO_File, TM_SWO_fputc);
}

#endif
