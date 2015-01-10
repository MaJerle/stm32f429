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
#include "tm_stm32f4_otp.h"
#include "tm_stm32f4_disco.h"

TM_OTP_Result_t TM_OTP_Write(uint8_t block, uint8_t byte, uint8_t data) {
	FLASH_Status status;
	
	/* Check input parameters */
	if (
		block >= OTP_BLOCKS ||
		byte >= OTP_BYTES_IN_BLOCK
	) {
		/* Invalid parameters */
		
		/* Return error */
		return TM_OTP_Result_Error;
	}
	
	/* Unlock FLASH */
	FLASH_Unlock();

	/* Clear pending flags (if any) */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
					FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	/* Wait for last operation */
	status = FLASH_WaitForLastOperation();
	
	/* If it is not success, return error */
	if (status != FLASH_COMPLETE) {
		/* Lock FLASH */
		FLASH_Lock();
		
		/* Return error */
		return TM_OTP_Result_Error;
	}
	
	/* Write byte */
	status = FLASH_ProgramByte(OTP_START_ADDR + block * OTP_BYTES_IN_BLOCK + byte, data);

	/* Lock FLASH */
	FLASH_Lock();
	
	/* Check status */
	if (status == FLASH_COMPLETE) {
		/* Return OK */
		return TM_OTP_Result_Ok;
	} else {
		/* Return error */
		return TM_OTP_Result_Error;
	}
}

uint8_t TM_OTP_Read(uint8_t block, uint8_t byte) {
	uint8_t data;
	
	/* Check input parameters */
	if (
		block >= OTP_BLOCKS ||
		byte >= OTP_BYTES_IN_BLOCK
	) {
		/* Invalid parameters */
		return 0;
	}
	
	/* Get value */
	data = *(__IO uint8_t *)(OTP_START_ADDR + block * OTP_BYTES_IN_BLOCK + byte);
	
	/* Return data */
	return data;
}

TM_OTP_Result_t TM_OTP_BlockLock(uint8_t block) {
	FLASH_Status status;
	
	/* Check input parameters */
	if (block >= OTP_BLOCKS) {
		/* Invalid parameters */
		
		/* Return error */
		return TM_OTP_Result_Error;
	}
	
	/* Unlock FLASH */
	FLASH_Unlock();

	/* Clear pending flags (if any) */  
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
					FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	/* Wait for last operation */
	status = FLASH_WaitForLastOperation();
	
	/* If it is not success, return error */
	if (status != FLASH_COMPLETE) {
		/* Lock FLASH */
		FLASH_Lock();
		
		/* Return error */
		return TM_OTP_Result_Error;
	}
	
	/* Write byte */
	status = FLASH_ProgramByte(OTP_LOCK_ADDR + block, 0x00);

	/* Lock FLASH */
	FLASH_Lock();
	
	/* Check status */
	if (status == FLASH_COMPLETE) {
		/* Return OK */
		return TM_OTP_Result_Ok;
	}
	
	/* Return error */
	return TM_OTP_Result_Error;
}

