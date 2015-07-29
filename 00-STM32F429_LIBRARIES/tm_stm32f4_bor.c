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
#include "tm_stm32f4_bor.h"

TM_BOR_Result_t TM_BOR_Set(TM_BOR_Level_t BORValue) {
	FLASH_Status status;
	
	/* Check current BOR value */
	if (TM_BOR_Get() != BORValue) {
		/* Set new value */

		/* Unlocks the option bytes block access */
		FLASH_OB_Unlock();

		/* Select the desired V(BOR) Level */
		FLASH_OB_BORConfig((uint32_t)BORValue); 

		/* Launch the option byte loading */
		status = FLASH_OB_Launch();
		
		/* Lock access to registers */
		FLASH_OB_Lock();
		
		/* Check success */
		if (status != FLASH_COMPLETE) {
			/* Return error */
			return TM_BOR_Result_Error;
		}
	}
	
	/* Return OK */
	return TM_BOR_Result_Ok;
}

TM_BOR_Level_t TM_BOR_Get(void) {
	/* Get BOR value */
	switch (FLASH_OB_GetBOR()) {
		case OB_BOR_LEVEL3:
			return TM_BOR_Level_3;
		case OB_BOR_LEVEL2:
			return TM_BOR_Level_2;
		case OB_BOR_LEVEL1:
			return TM_BOR_Level_1;
		case OB_BOR_OFF:
		default:
			return TM_BOR_Level_None;
	}
}
