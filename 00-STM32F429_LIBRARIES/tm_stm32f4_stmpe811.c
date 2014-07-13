#include "tm_stm32f4_stmpe811.h"

uint8_t TM_STMPE811_Init(void) {
	uint8_t test[2];
	//Initialize Delay
	TM_DELAY_Init();
	//Initialize I2C
	TM_I2C_Init(STMPE811_I2C, STMPE811_I2C_PINSPACK, 400000);
	
	test[0] = TM_STMPE811_Read(STMPE811_TSC_CTRL);
	test[0] &= ~STMPE811_TSC_CTRL_EN;
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_TSC_CTRL, test[0]);
	test[1] = TM_STMPE811_Read(STMPE811_TSC_CTRL);
	// Set operating mode	
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_TSC_CTRL, STMPE811_TSC_CTRL_EN | STMPE811_TSC_CTRL_XY);
	//if (test[0] == test[1]) return test[0];
	// Reset
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_SYS_CTRL1, STMPE811_SYS_CTRL1_SOFTRESET);
    // Wait minumum of 10ms.
    Delayms(20);
    // Enable TSC and ADC
    TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_SYS_CTRL2, STMPE811_CTRL2_TS_OFF | STMPE811_CTRL2_GPIO_OFF);
    // Enable Touch detect, FIFO interrupt. 
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_INT_EN, STMPE811_INT_EN_TOUCHDET | STMPE811_INT_EN_TH | STMPE811_INT_EN_FIFOOFLOW);
	// To select sample time, bit number and ADC reference.
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_ADC_CTRL1, ((0x6 & STMPE811_ADC_CTRL1_SAMPLE_M) << STMPE811_ADC_CTRL1_SAMPLE_S ) | STMPE811_ADC_CTRL1_MOD);
	// Wait minumum of 2ms.
	Delayms(4);
	// To select the ADC clock speed is 3.25 MHz.
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_ADC_CTRL2, STMPE811_ADC_CTRL2_CLK1);
	// To set Average, Touch Detect Delay and Setting Time setting.
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_TSC_CFG, STMPE811_TSC_CFG_AVE_8 | ((0x6 & STMPE811_TSC_CFG_DELAY_M) << STMPE811_TSC_CFG_DELAY_S)| ((0x5 & STMPE811_TSC_CFG_SETTING_M) << STMPE811_TSC_CFG_SETTING_S));
	// Set FIFO threshold level.
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_FIFO_TH, 0x01);
	// Reset FIFO.
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_FIFO_STA, STMPE811_FIFO_STA_RESET);
	// Write 0x00 to put the FIFO back into operation mode.
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_FIFO_STA, 0x00);
	// Set the data format for Z value.
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_TSC_FRACTION_Z, (0x07 * STMPE811_FRACTION_Z_M) << STMPE811_FRACTION_Z_S);
	// Set the driving capability of the device for touchscreen controller pins.
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_TSC_I_DRIVE, STMPE811_TSC_I_DRIVE_50);
	// Set tracking index, set touchscreen controller operation mode and enable 
	// the TSC. STMPE811_TSC_CTRL
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_TSC_CTRL, STMPE811_TSC_CTRL_EN | STMPE811_TSC_CTRL_XYZ);
	// Clear all the interrupt status.
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_INT_STA, 0xFF);
	// Set interrupt mode and enable interrupt.
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_INT_CTRL, STMPE811_INT_TYPE | STMPE811_INT_GLOBAL);
	
	return 0;
}

uint8_t TM_STMPE811_Read(uint8_t reg) {
	return TM_I2C_Read(STMPE811_I2C, STMPE811_ADDRESS, reg);
}

TM_STMPE811_State_t TM_STMPE811_ReadTouch(TM_STMPE811_TouchData *structdata) {
	uint8_t val;
	//volatile uint8_t i;
	val = TM_STMPE811_Read(STMPE811_TSC_CTRL);
	Delayms(10);
	if ((val & 0x80) == 0) {
		//Not pressed
		structdata->x = 0;
		structdata->y = 0;
		structdata->pressed = TM_STMPE811_State_Released;
		
		return TM_STMPE811_State_Released;
	}
	//Pressed
	if (structdata->orientation == TM_STMPE811_Orientation_Portrait_1) {
		structdata->x = 239 - TM_STMPE811_ReadX(structdata->x);
		structdata->y = 319 - TM_STMPE811_ReadY(structdata->y);
	} else if (structdata->orientation == TM_STMPE811_Orientation_Portrait_2) {
		structdata->x = TM_STMPE811_ReadX(structdata->x);
		structdata->y = TM_STMPE811_ReadY(structdata->y);
	} else if (structdata->orientation == TM_STMPE811_Orientation_Landscape_1) {
		structdata->y = TM_STMPE811_ReadX(structdata->y);
		structdata->x = 319 - TM_STMPE811_ReadY(structdata->x);
	} else if (structdata->orientation == TM_STMPE811_Orientation_Landscape_2) {
		structdata->y = 239 - TM_STMPE811_ReadX(structdata->x);
		structdata->x = TM_STMPE811_ReadY(structdata->x);
	}
	
	//Reset Fifo
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_FIFO_STA, 0x01);
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_FIFO_STA, 0x00);
	
	//Check for valid data
	structdata->pressed = TM_STMPE811_State_Pressed;
	if (structdata->orientation == TM_STMPE811_Orientation_Portrait_1 || structdata->orientation == TM_STMPE811_Orientation_Portrait_2) {
		//Portrait
		if (structdata->x > 0 && structdata->x < 239 && structdata->y > 0 && structdata->y < 319) {
			return TM_STMPE811_State_Pressed;
		}
	} else {
		//Landscape
		if (structdata->x > 0 && structdata->x < 319 && structdata->y > 0 && structdata->y < 239) {
			return TM_STMPE811_State_Pressed;
		}
	}
	return TM_STMPE811_State_Released;
}

uint16_t TM_STMPE811_ReadX(uint16_t x) {
	uint8_t data[2];
	int16_t val, dx;
	data[1] = TM_STMPE811_Read(STMPE811_TSC_DATA_X);
	data[0] = TM_STMPE811_Read(STMPE811_TSC_DATA_X + 1);
	val = (data[1] << 8 | (data[0] & 0xFF));
	
	if (val <= 3000) {
		val = 3900 - val;
	} else {
		val = 3800 - val;
	}
	
	val /= 15;
	
	if (val > 239) {
		val = 239;
	} else if (val < 0) {
		val = 0;
	}
	
	dx = (val > x) ? (val - x) : (x - val);
	if (dx > 4) {
		return val;
	}
	return x;
}

uint16_t TM_STMPE811_ReadY(uint16_t y) {
	uint8_t data[2];
	int16_t val, dy;
	data[1] = TM_STMPE811_Read(STMPE811_TSC_DATA_Y);
	data[0] = TM_STMPE811_Read(STMPE811_TSC_DATA_Y + 1);
	val = (data[1] << 8 | (data[0] & 0xFF));

	val -= 360;
	val = val / 11;

	if (val <= 0) {
		val = 0;
	} else if (val >= 320) {
		val = 319;
	}
	
	dy = (val > y) ? (val - y) : (y - val);
	if (dy > 4) {
		return val;
	}
	return y;
}	


