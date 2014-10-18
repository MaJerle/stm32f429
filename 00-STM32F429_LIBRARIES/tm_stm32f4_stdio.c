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
#include "tm_stm32f4_stdio.h"

/* Create main __stdout for printf */
/* Set funcPointer to 0 */
FILE __stdout = {
	0
};

/* stdio.h related function */
int fputc(int ch, FILE* f) {
	/* Check if it was called from printf or fprintf */
	
	if (f->outputFuncPointer != 0) {
		/* Call user custom function */
		return f->outputFuncPointer(ch, f);
	}
	
	/* Printf was call */
	return TM_STDIO_PrintfHandler(ch, f);
}

void TM_STDIO_SetOutputFunction(FILE* f, int (*funcPointer)(int, FILE *)) {
	/* Set pointer to output function for specific file pointer */
	f->outputFuncPointer = funcPointer;
}
