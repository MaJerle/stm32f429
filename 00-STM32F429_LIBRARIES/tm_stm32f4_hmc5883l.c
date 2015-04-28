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
#include "stdio.h"

/* Private functions */
static uint8_t HMC5883L_DataReady = 0;
static float HMC5883L_mGPerDigit;

TM_HMC5883L_Result_t TM_HMC5883L_Init(TM_HMC5883L_t* HMC5883L, TM_HMC5883L_Gain_t Gain, TM_HMC5883L_OutputRate_t OutputRate) {
	uint8_t data[3];
	
	/* Init I2C */
	TM_I2C_Init(HMC5883L_I2C, HMC5883L_I2C_PINSPACK, HMC5883L_I2C_CLOCK);
	
	/* Check if device is connected */
	if (!TM_I2C_IsDeviceConnected(HMC5883L_I2C, HMC5883L_I2C_ADDR)) {
		/* Return error */
		return TM_HMC5883L_Result_DeviceNotConnected;
	}
	
	/* Attach data ready pin to interrupt */
	if (TM_EXTI_Attach(HMC5883L_DRDY_PORT, HMC5883L_DRDY_PIN, TM_EXTI_Trigger_Falling) != TM_EXTI_Result_Ok) {
		/* Return error */
		return TM_HMC5883L_Result_Error;
	}
	
	/* Configure device */
	
	/* Register A */
	data[0] = 0x30 | (uint8_t) OutputRate;
	
	/* Register B */
	data[1] = (uint8_t) Gain;
	
	/* Mode register */
	data[2] = 0;
	
	/* Write data to device */
	TM_I2C_WriteMulti(HMC5883L_I2C, HMC5883L_I2C_ADDR, HMC5883L_CONFA, data, 3);
	
	/* Read identification register */
	TM_I2C_ReadMulti(HMC5883L_I2C, HMC5883L_I2C_ADDR, HMC5883L_IDA, HMC5883L->ID, 3);
	
	/* Store LSB value */
	switch (Gain) {
		case TM_HMC5883L_Gain_0_88:
			HMC5883L_mGPerDigit = 0.073;
			break;
		case TM_HMC5883L_Gain_1_3:
			HMC5883L_mGPerDigit = 0.92;
			break;
		case TM_HMC5883L_Gain_1_9:
			HMC5883L_mGPerDigit = 1.22;
			break;
		case TM_HMC5883L_Gain_2_5:
			HMC5883L_mGPerDigit = 1.52;
			break;
		case TM_HMC5883L_Gain_4_0:
			HMC5883L_mGPerDigit = 2.27;
			break;
		case TM_HMC5883L_Gain_4_7:
			HMC5883L_mGPerDigit = 2.56;
			break;
		case TM_HMC5883L_Gain_5_6:
			HMC5883L_mGPerDigit = 3.03;
			break;
		case TM_HMC5883L_Gain_8_1:
			HMC5883L_mGPerDigit = 4.35;
			break;
		default:
			HMC5883L_mGPerDigit = 1;
	}
	
	/* Return OK */
	return TM_HMC5883L_Result_Ok;
}

TM_HMC5883L_Result_t TM_HMC5883L_DataReady(TM_HMC5883L_t* HMC5883L) {
	/* Check data ready flag */
	if (HMC5883L_DataReady) {
		/* Data ready */
		return TM_HMC5883L_Result_Ok;
	}
	
	/* Data not ready */
	return TM_HMC5883L_Result_Error;
}

TM_HMC5883L_Result_t TM_HMC5883L_Read(TM_HMC5883L_t* HMC5883L) {
	uint8_t data[6];
	
	/* Check data ready flag */
	if (!HMC5883L_DataReady) {
		/* Data not ready */
		return TM_HMC5883L_Result_Error;
	}
	
	/* Read multi registers */
	TM_I2C_ReadMulti(HMC5883L_I2C, HMC5883L_I2C_ADDR, HMC5883L_DATAXM, data, 6);
	
	/* Format data */
	HMC5883L->X = (int16_t)((uint8_t)data[0] << 8 | (uint8_t)data[1]);
	HMC5883L->Z = (int16_t)((uint8_t)data[2] << 8 | (uint8_t)data[3]);
	HMC5883L->Y = (int16_t)((uint8_t)data[4] << 8 | (uint8_t)data[5]);
	
	/* Calculate */
	HMC5883L->X = HMC5883L->X * ((float)HMC5883L_mGPerDigit);
	HMC5883L->Z = HMC5883L->Z * ((float)HMC5883L_mGPerDigit);
	HMC5883L->Y = HMC5883L->Y * ((float)HMC5883L_mGPerDigit);
	
	/* Clear data ready flag */
	HMC5883L_DataReady = 0;
	
	/* Return OK */
	return TM_HMC5883L_Result_Ok;
}

TM_HMC5883L_Result_t TM_HMC5883L_ProcessInterrupt(TM_HMC5883L_t* HMC5883L) {
	/* Set data ready flag */
	HMC5883L_DataReady = 1;
	
	/* Return OK */
	return TM_HMC5883L_Result_Ok;
}
