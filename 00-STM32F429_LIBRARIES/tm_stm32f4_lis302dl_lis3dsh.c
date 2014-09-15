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
#include "tm_stm32f4_lis302dl_lis3dsh.h"

/* Private */
extern void TM_LIS302DL_LIS3DSH_INT_WriteSPI(uint8_t* data, uint8_t addr, uint8_t count);
extern void TM_LIS302DL_LIS3DSH_INT_ReadSPI(uint8_t* data, uint8_t addr, uint8_t count);
extern void TM_LIS302DL_LIS3DSH_INT_InitPins(void);
extern void TM_LIS302DL_LIS3DSH_INT_InitLIS3DSH(TM_LIS302DL_LIS3DSH_Sensitivity_t Sensitivity, TM_LIS302DL_LIS3DSH_Filter_t Filter);
extern void TM_LIS302DL_LIS3DSH_INT_InitLIS302DL(TM_LIS302DL_LIS3DSH_Sensitivity_t Sensitivity, TM_LIS302DL_LIS3DSH_Filter_t Filter);
extern void TM_LIS3DSH_INT_ReadAxes(TM_LIS302DL_LIS3DSH_t* Axes_Data);
extern void TM_LIS302DL_INT_ReadAxes(TM_LIS302DL_LIS3DSH_t* Axes_Data);
extern void TM_LIS302DL_LIS3DSH_INT_Delay(void);

TM_LIS302DL_LIS3DSH_Device_t TM_LIS302DL_LIS3DSH_INT_Device = TM_LIS302DL_LIS3DSH_Device_Error;
float TM_LIS3DSH_INT_Sensitivity;

/* Public */
TM_LIS302DL_LIS3DSH_Device_t TM_LIS302DL_LIS3DSH_Detect(void) {
	uint8_t id;
	/* Delay on power up */
	TM_LIS302DL_LIS3DSH_INT_Delay();
	/* Init SPI */
	TM_LIS302DL_LIS3DSH_INT_InitPins();
	/* Get ID */
	TM_LIS302DL_LIS3DSH_INT_ReadSPI(&id, LIS302DL_LIS3DSH_REG_WHO_I_AM, 1);
	/* Check device */
	if (id == LIS302DL_ID) {
		/* Set device */
		TM_LIS302DL_LIS3DSH_INT_Device = TM_LIS302DL_LIS3DSH_Device_LIS302DL;
		/* Return device */
		return TM_LIS302DL_LIS3DSH_Device_LIS302DL;
	} else if (id == LIS3DSH_ID) {
		/* Set device */
		TM_LIS302DL_LIS3DSH_INT_Device = TM_LIS302DL_LIS3DSH_Device_LIS3DSH;
		/* Return device */;
		return TM_LIS302DL_LIS3DSH_Device_LIS3DSH;
	}
	
	/* Return Error */
	return TM_LIS302DL_LIS3DSH_Device_Error;
}

TM_LIS302DL_LIS3DSH_Device_t TM_LIS302DL_LIS3DSH_Init(TM_LIS302DL_LIS3DSH_Sensitivity_t Sensitivity, TM_LIS302DL_LIS3DSH_Filter_t Filter) {
	/* Init pinout and SPI */
	TM_LIS302DL_LIS3DSH_INT_InitPins();
	/* Some delay */
	TM_LIS302DL_LIS3DSH_INT_Delay();
	/* Detect proper device and init it */
	if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS302DL) {
		/* Init sequence for LIS302DL */
		TM_LIS302DL_LIS3DSH_INT_InitLIS302DL(Sensitivity, Filter);
		/* Return device */
		return TM_LIS302DL_LIS3DSH_Device_LIS302DL;
	} else if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS3DSH) {
		/* Init sequence for LIS3DSH */
		TM_LIS302DL_LIS3DSH_INT_InitLIS3DSH(Sensitivity, Filter);
		/* Return device */
		return TM_LIS302DL_LIS3DSH_Device_LIS3DSH;
	}
	
	/* Error detection */
	TM_LIS302DL_LIS3DSH_INT_Device = TM_LIS302DL_LIS3DSH_Device_Error;
	/* Return Error */
	return TM_LIS302DL_LIS3DSH_Device_Error;
}

TM_LIS302DL_LIS3DSH_Device_t TM_LIS302DL_LIS3DSH_ReadAxes(TM_LIS302DL_LIS3DSH_t* Axes_Data) {
	if (TM_LIS302DL_LIS3DSH_INT_Device == TM_LIS302DL_LIS3DSH_Device_LIS302DL) {
		/* Init sequence for LIS302DL */
		TM_LIS302DL_INT_ReadAxes(Axes_Data);
		/* Return device */
		return TM_LIS302DL_LIS3DSH_Device_LIS302DL;
	} else if (TM_LIS302DL_LIS3DSH_INT_Device == TM_LIS302DL_LIS3DSH_Device_LIS3DSH) {
		/* Init sequence for LIS3DSH */
		TM_LIS3DSH_INT_ReadAxes(Axes_Data);
		/* Return device */
		return TM_LIS302DL_LIS3DSH_Device_LIS3DSH;
	}
	/* Return Error */
	return TM_LIS302DL_LIS3DSH_Device_Error;
}

/* Private */
void TM_LIS302DL_LIS3DSH_INT_InitPins(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	static uint8_t initialized = 0;
	if (initialized) {
		return;
	}
	
	/* Initialize SPI */
	TM_SPI_Init(LIS302DL_LIS3DSH_SPI, LIS302DL_LIS3DSH_SPI_PINSPACK);
	
	/* Enable clock for CS port */
	RCC_AHB1PeriphClockCmd(LIS302DL_LIS3DSH_CS_RCC, ENABLE);
	
	/* Configure CS pin */
	GPIO_InitStruct.GPIO_Pin = LIS302DL_LIS3DSH_CS_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	/* GPIO Init */
	GPIO_Init(LIS302DL_LIS3DSH_CS_PORT, &GPIO_InitStruct);
	
	/* CS HIGH */
	LIS302DL_LIS3DSH_CS_HIGH;
	
	initialized = 1;
}

void TM_LIS302DL_LIS3DSH_INT_WriteSPI(uint8_t* data, uint8_t addr, uint8_t count) {
	/* Start SPI transmission */
	LIS302DL_LIS3DSH_CS_LOW;
	
	if (count > 1 && TM_LIS302DL_LIS3DSH_INT_Device == TM_LIS302DL_LIS3DSH_Device_LIS302DL) {
		/* Add autoincrement bit */
		/* Only LIS302DL device */
		addr |= 0x40;
	}
	
	/* Send address */
	TM_SPI_Send(LIS302DL_LIS3DSH_SPI, addr);
	/* Send data */
	TM_SPI_WriteMulti(LIS302DL_LIS3DSH_SPI, data, count);
	
	/* Stop SPI transmission */
	LIS302DL_LIS3DSH_CS_HIGH;
}

void TM_LIS302DL_LIS3DSH_INT_ReadSPI(uint8_t* data, uint8_t addr, uint8_t count) {
	/* Start SPI transmission */
	LIS302DL_LIS3DSH_CS_LOW;
	
	/* Add read bit */
	addr |= 0x80;
	
	if (count > 1) {
		/* Add autoincrement bit */
		addr |= 0x40;
	}
	
	/* Send address */
	TM_SPI_Send(LIS302DL_LIS3DSH_SPI, addr);
	/* Receive data */
	TM_SPI_ReadMulti(LIS302DL_LIS3DSH_SPI, data, 0x00, count);
	
	/* Stop SPI transmission */
	LIS302DL_LIS3DSH_CS_HIGH;
}

void TM_LIS302DL_LIS3DSH_INT_InitLIS3DSH(TM_LIS302DL_LIS3DSH_Sensitivity_t Sensitivity, TM_LIS302DL_LIS3DSH_Filter_t Filter) {
	uint8_t tmpreg;
	uint16_t temp;

	/* Set data */
	temp = (uint16_t) (LIS3DSH_DATARATE_100 | LIS3DSH_XYZ_ENABLE);
	temp |= (uint16_t) (LIS3DSH_SERIALINTERFACE_4WIRE | LIS3DSH_SELFTEST_NORMAL);
	
	/* Set sensitivity */
	if (Sensitivity == TM_LIS3DSH_Sensitivity_2G) {
		temp |= (uint16_t) (LIS3DSH_FULLSCALE_2);
		TM_LIS3DSH_INT_Sensitivity = LIS3DSH_SENSITIVITY_0_06G;
	} else if (Sensitivity == TM_LIS3DSH_Sensitivity_4G) {
		temp |= (uint16_t) (LIS3DSH_FULLSCALE_4);
		TM_LIS3DSH_INT_Sensitivity = LIS3DSH_SENSITIVITY_0_12G;
	} else if (Sensitivity == TM_LIS3DSH_Sensitivity_6G) {
		temp |= (uint16_t) (LIS3DSH_FULLSCALE_6);
		TM_LIS3DSH_INT_Sensitivity = LIS3DSH_SENSITIVITY_0_18G;
	} else if (Sensitivity == TM_LIS3DSH_Sensitivity_8G) {
		temp |= (uint16_t) (LIS3DSH_FULLSCALE_8);
		TM_LIS3DSH_INT_Sensitivity = LIS3DSH_SENSITIVITY_0_24G;
	} else if (Sensitivity == TM_LIS3DSH_Sensitivity_16G) {
		temp |= (uint16_t) (LIS3DSH_FULLSCALE_16);
		TM_LIS3DSH_INT_Sensitivity = LIS3DSH_SENSITIVITY_0_73G;
	} else {
		return;
	}
	
	/* Set filter */
	if (Filter == TM_LIS3DSH_Filter_800Hz) {
		temp |= (uint16_t) (LIS3DSH_FILTER_BW_800 << 8);
	} else if (Filter == TM_LIS3DSH_Filter_400Hz) {
		temp |= (uint16_t) (LIS3DSH_FILTER_BW_400 << 8);
	} else if (Filter == TM_LIS3DSH_Filter_200Hz) {
		temp |= (uint16_t) (LIS3DSH_FILTER_BW_200 << 8);
	} else if (Filter == TM_LIS3DSH_Filter_50Hz) {
		temp |= (uint16_t) (LIS3DSH_FILTER_BW_50 << 8);
	} else {
		return;
	}
	
	/* Configure MEMS: power mode(ODR) and axes enable */
	tmpreg = (uint8_t) (temp);

	/* Write value to MEMS CTRL_REG4 register */
	TM_LIS302DL_LIS3DSH_INT_WriteSPI(&tmpreg, LIS3DSH_CTRL_REG4_ADDR, 1);

	/* Configure MEMS: full scale and self test */
	tmpreg = (uint8_t) (temp >> 8);

	/* Write value to MEMS CTRL_REG5 register */
	TM_LIS302DL_LIS3DSH_INT_WriteSPI(&tmpreg, LIS3DSH_CTRL_REG5_ADDR, 1);
}

void TM_LIS302DL_LIS3DSH_INT_InitLIS302DL(TM_LIS302DL_LIS3DSH_Sensitivity_t Sensitivity, TM_LIS302DL_LIS3DSH_Filter_t Filter) {
	uint16_t ctrl;
	
	/* Reboot */
	TM_LIS302DL_LIS3DSH_INT_ReadSPI((uint8_t *)&ctrl, LIS302DL_CTRL_REG2_ADDR, 1);
	ctrl |= LIS302DL_BOOT_REBOOTMEMORY;
	TM_LIS302DL_LIS3DSH_INT_WriteSPI((uint8_t *)&ctrl, LIS302DL_CTRL_REG2_ADDR, 1);
	
	/* Init settings */
	ctrl = (uint16_t) (LIS302DL_DATARATE_100 | LIS302DL_LOWPOWERMODE_ACTIVE | LIS302DL_SELFTEST_NORMAL | LIS302DL_XYZ_ENABLE);
	if (Sensitivity == TM_LIS302DL_Sensitivity_2_3G) {
		ctrl |= (uint16_t) LIS302DL_FULLSCALE_2_3;
		TM_LIS3DSH_INT_Sensitivity = LIS302DL_SENSITIVITY_2_3G;
	} else if (Sensitivity == TM_LIS302DL_Sensitivity_9_2G) {
		ctrl |= (uint16_t) LIS302DL_FULLSCALE_9_2;
		TM_LIS3DSH_INT_Sensitivity = LIS302DL_SENSITIVITY_9_2G;
	} else {
		return;
	}
	/* Write settings */
	TM_LIS302DL_LIS3DSH_INT_WriteSPI((uint8_t *)&ctrl, LIS302DL_CTRL_REG1_ADDR, 1);
	
	/* Read filter */
	TM_LIS302DL_LIS3DSH_INT_WriteSPI((uint8_t *)&ctrl, LIS302DL_CTRL_REG2_ADDR, 1);
	ctrl &= (uint8_t) ~(LIS302DL_FILTEREDDATASELECTION_OUTPUTREGISTER | LIS302DL_HIGHPASSFILTER_LEVEL_3 | LIS302DL_HIGHPASSFILTERINTERRUPT_1_2);
	/* Set filter */
    ctrl |= (uint8_t) (LIS302DL_HIGHPASSFILTERINTERRUPT_1_2 | LIS302DL_FILTEREDDATASELECTION_OUTPUTREGISTER);
	/* Set filter value */
	if (Filter == TM_LIS302DL_Filter_2Hz) {
		ctrl |= (uint8_t) LIS302DL_HIGHPASSFILTER_LEVEL_0;
	} else if (Filter == TM_LIS302DL_Filter_1Hz) {
		ctrl |= (uint8_t) LIS302DL_HIGHPASSFILTER_LEVEL_1;
	} else if (Filter == TM_LIS302DL_Filter_500mHz) {
		ctrl |= (uint8_t) LIS302DL_HIGHPASSFILTER_LEVEL_2;
	} else if (Filter == TM_LIS302DL_Filter_250mHz) {
		ctrl |= (uint8_t) LIS302DL_HIGHPASSFILTER_LEVEL_3;
	} else {
		return;
	}
	/* Write settings */
	TM_LIS302DL_LIS3DSH_INT_WriteSPI((uint8_t *)&ctrl, LIS302DL_CTRL_REG2_ADDR, 1);
}

void TM_LIS3DSH_INT_ReadAxes(TM_LIS302DL_LIS3DSH_t *Axes_Data) {
	int8_t buffer[6];

	TM_LIS302DL_LIS3DSH_INT_ReadSPI((uint8_t*)&buffer[0], LIS3DSH_OUT_X_L_ADDR, 1);
	TM_LIS302DL_LIS3DSH_INT_ReadSPI((uint8_t*)&buffer[1], LIS3DSH_OUT_X_H_ADDR, 1);
	TM_LIS302DL_LIS3DSH_INT_ReadSPI((uint8_t*)&buffer[2], LIS3DSH_OUT_Y_L_ADDR, 1);
	TM_LIS302DL_LIS3DSH_INT_ReadSPI((uint8_t*)&buffer[3], LIS3DSH_OUT_Y_H_ADDR, 1);
	TM_LIS302DL_LIS3DSH_INT_ReadSPI((uint8_t*)&buffer[4], LIS3DSH_OUT_Z_L_ADDR, 1);
	TM_LIS302DL_LIS3DSH_INT_ReadSPI((uint8_t*)&buffer[5], LIS3DSH_OUT_Z_H_ADDR, 1);
	
	/* Set axes */
	Axes_Data->X = (int16_t)((buffer[1] << 8) + buffer[0]) * TM_LIS3DSH_INT_Sensitivity;
	Axes_Data->Y = (int16_t)((buffer[3] << 8) + buffer[2]) * TM_LIS3DSH_INT_Sensitivity;
	Axes_Data->Z = (int16_t)((buffer[5] << 8) + buffer[4]) * TM_LIS3DSH_INT_Sensitivity;
}

void TM_LIS302DL_INT_ReadAxes(TM_LIS302DL_LIS3DSH_t* Axes_Data) {
	int8_t buffer[3];
	int16_t SwitchXY;

	TM_LIS302DL_LIS3DSH_INT_ReadSPI((uint8_t*)&buffer[0], LIS302DL_OUT_X_ADDR, 1);
	TM_LIS302DL_LIS3DSH_INT_ReadSPI((uint8_t*)&buffer[1], LIS302DL_OUT_Y_ADDR, 1);
	TM_LIS302DL_LIS3DSH_INT_ReadSPI((uint8_t*)&buffer[2], LIS302DL_OUT_Z_ADDR, 1);
	
	/* Set axes */
	Axes_Data->X = (int16_t) (buffer[0]) * TM_LIS3DSH_INT_Sensitivity;
	Axes_Data->Y = (int16_t) (buffer[1]) * TM_LIS3DSH_INT_Sensitivity;
	Axes_Data->Z = (int16_t) (buffer[2]) * TM_LIS3DSH_INT_Sensitivity;	
	/* Switch axes */
	SwitchXY  = Axes_Data->X;
	Axes_Data->X = Axes_Data->Y;
	Axes_Data->X = -SwitchXY;
}

void TM_LIS302DL_LIS3DSH_INT_Delay(void) {
	uint32_t delay = 1000000;
	while (delay--);
}

