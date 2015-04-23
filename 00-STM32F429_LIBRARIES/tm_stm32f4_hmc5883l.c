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
#include "tm_stm32f4_hmc5883l.h"

TM_HMC5883L_Result_t TM_HMC5883L_Init(void) {
	uint8_t data[3];
	/* Init I2C */
	TM_I2C_Init(HMC5883L_I2C, HMC5883L_I2C_PINSPACK, HMC5883L_I2C_CLOCK);
	
	/* Check if device is connected */
	if (!TM_I2C_IsDeviceConnected(HMC5883L_I2C, HMC5883L_I2C_ADDR)) {
		/* Return error */
		return TM_HMC5883L_Result_DeviceNotConnected;
	}
	
	/* Configure device */
	
	/* Read identification register */
	TM_I2C_ReadMulti(HMC5883L_I2C, HMC5883L_I2C_ADDR, 0x0A, data, 3);
	
	printf("Ident: 0x%02X 0x%02X 0x%02X\n", data[0], data[1], data[2]);
	
	/* Return OK */
	return TM_HMC5883L_Result_Ok;
}
