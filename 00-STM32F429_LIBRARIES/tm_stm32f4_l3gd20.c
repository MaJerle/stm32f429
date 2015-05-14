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
#include "tm_stm32f4_l3gd20.h"

/* Private variables */
TM_L3GD20_Scale_t TM_L3GD20_INT_Scale;

/* Private functions */
extern void TM_L3GD20_INT_InitPins(void);
extern uint8_t TM_L3GD20_INT_ReadSPI(uint8_t address);
extern void TM_L3GD20_INT_WriteSPI(uint8_t address, uint8_t data);

/* Public */
TM_L3GD20_Result_t TM_L3GD20_Init(TM_L3GD20_Scale_t scale) {
	/* Init CS pin */
	TM_L3GD20_INT_InitPins();
	/* Init SPI */
	TM_SPI_Init(L3GD20_SPI, L3GD20_SPI_PINSPACK);
	/* Check if sensor is L3GD20 */
	if (TM_L3GD20_INT_ReadSPI(L3GD20_REG_WHO_AM_I) != L3GD20_WHO_AM_I) {
		/* Sensor connected is not L3GD20 */
		return TM_L3GD20_Result_Error;
	}

	/* Enable L3GD20 Power bit */
	TM_L3GD20_INT_WriteSPI(L3GD20_REG_CTRL_REG1, 0xFF);

	/* Set L3GD20 scale */
	if (scale == TM_L3GD20_Scale_250) {
		TM_L3GD20_INT_WriteSPI(L3GD20_REG_CTRL_REG4, 0x00);
	} else if (scale == TM_L3GD20_Scale_500) {
		TM_L3GD20_INT_WriteSPI(L3GD20_REG_CTRL_REG4, 0x10);
	} else if (scale == TM_L3GD20_Scale_2000) {
		TM_L3GD20_INT_WriteSPI(L3GD20_REG_CTRL_REG4, 0x20);
	}
	
	/* Save scale */
	TM_L3GD20_INT_Scale = scale;

	/* Set high-pass filter settings */
	TM_L3GD20_INT_WriteSPI(L3GD20_REG_CTRL_REG2, 0x00);

	/* Enable high-pass filter */
	TM_L3GD20_INT_WriteSPI(L3GD20_REG_CTRL_REG5, 0x10);
	
	/* Everything OK */
	return TM_L3GD20_Result_Ok;
}

TM_L3GD20_Result_t TM_L3GD20_Read(TM_L3GD20_t* L3DG20_Data) {
	float temp, s;
	
	/* Read X axis */
	L3DG20_Data->X = TM_L3GD20_INT_ReadSPI(L3GD20_REG_OUT_X_L);
	L3DG20_Data->X |= TM_L3GD20_INT_ReadSPI(L3GD20_REG_OUT_X_H) << 8;

	/* Read Y axis */
	L3DG20_Data->Y = TM_L3GD20_INT_ReadSPI(L3GD20_REG_OUT_Y_L);
	L3DG20_Data->Y |= TM_L3GD20_INT_ReadSPI(L3GD20_REG_OUT_Y_H) << 8;
	
	/* Read Z axis */
	L3DG20_Data->Z = TM_L3GD20_INT_ReadSPI(L3GD20_REG_OUT_Z_L);
	L3DG20_Data->Z |= TM_L3GD20_INT_ReadSPI(L3GD20_REG_OUT_Z_H) << 8;
	
	/* Set sensitivity scale correction */
	if (TM_L3GD20_INT_Scale == TM_L3GD20_Scale_250) {
		/* Sensitivity at 250 range = 8.75 mdps/digit */
		s = L3GD20_SENSITIVITY_250 * 0.001;
	} else if (TM_L3GD20_INT_Scale == TM_L3GD20_Scale_500) {
		/* Sensitivity at 500 range = 17.5 mdps/digit */
		s = L3GD20_SENSITIVITY_500 * 0.001;
	} else {
		/* Sensitivity at 2000 range = 70 mdps/digit */
		s = L3GD20_SENSITIVITY_2000 * 0.001;
	}
	
	temp = (float)L3DG20_Data->X * s;
	L3DG20_Data->X = (int16_t) temp;
	temp = (float)L3DG20_Data->Y * s;
	L3DG20_Data->Y = (int16_t) temp;
	temp = (float)L3DG20_Data->Z * s;
	L3DG20_Data->Z = (int16_t) temp;
	
	/* Return OK */
	return TM_L3GD20_Result_Ok;
}

/* Private functions */
void TM_L3GD20_INT_InitPins(void) {
	/* Init CS pin for SPI */
	TM_GPIO_Init(L3GD20_CS_PORT, L3GD20_CS_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Low);
	/* Set CS high */
	L3GD20_CS_HIGH;
}

uint8_t TM_L3GD20_INT_ReadSPI(uint8_t address) {
	uint8_t data;
	/* CS low */
	L3GD20_CS_LOW;
	/* Send address with read command */
	TM_SPI_Send(L3GD20_SPI, address | 0x80);
	/* Read data */
	data = TM_SPI_Send(L3GD20_SPI, 0xFF);
	/* CS high */
	L3GD20_CS_HIGH;
	/* Return data */
	return data;
}

void TM_L3GD20_INT_WriteSPI(uint8_t address, uint8_t data) {
	/* CS low */
	L3GD20_CS_LOW;
	/* Send address with write command */
	TM_SPI_Send(L3GD20_SPI, address);
	/* Write data */
	TM_SPI_Send(L3GD20_SPI, data);
	/* CS high */
	L3GD20_CS_HIGH;
}

