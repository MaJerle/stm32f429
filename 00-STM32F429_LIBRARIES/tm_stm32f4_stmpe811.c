#include "tm_stm32f4_stmpe811.h"

TM_STMPE811_State_t TM_STMPE811_Init(void) {
	uint8_t bytes[2];
	/* Initialize Delay */
	TM_DELAY_Init();
	/* Initialize I2C */
	TM_I2C_Init(STMPE811_I2C, STMPE811_I2C_PINSPACK, 400000);

	/* Check for STMPE811 Connected */
	TM_I2C_ReadMulti(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_CHIP_ID, bytes, 2);
	if ((bytes[0] << 8 | bytes[1]) != STMPE811_CHIP_ID_VALUE) {
		return TM_STMPE811_State_Error;
	}
	
	/* Get the current register value */
	bytes[0] = TM_STMPE811_Read(STMPE811_SYS_CTRL2);

	/* Set the Functionalities to be Enabled */    
	bytes[0] &= ~(0x06);  

	/* Set the new register value */  
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_SYS_CTRL2, bytes[0]); 

	/* Select Sample Time, bit number and ADC Reference */
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_ADC_CTRL1, 0x49);

	/* Wait for 2 ms */
	Delayms(2); 

	/* Select the ADC clock speed: 3.25 MHz */
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_ADC_CTRL2, 0x01);

	/* Select TSC pins in non default mode */  
	bytes[0] = TM_STMPE811_Read(STMPE811_GPIO_AF);
	bytes[0] &= ~(uint8_t)(0x1E);
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_GPIO_AF, bytes[0]);

	/* Select 2 nF filter capacitor */
	/* Configuration: 
	- Touch average control    : 4 samples
	- Touch delay time         : 500 uS
	- Panel driver setting time: 500 uS 
	*/
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_TSC_CFG, 0x9A); 

	/* Configure the Touch FIFO threshold: single point reading */
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_FIFO_TH, 0x01);

	/* Clear the FIFO memory content. */
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_FIFO_STA, 0x01);

	/* Put the FIFO back into operation mode  */
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_FIFO_STA, 0x00);

	/* Set the range and accuracy pf the pressure measurement (Z) : 
	- Fractional part :7 
	- Whole part      :1 
	*/
	//TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_TSC_FRACT_XYZ, 0x01);

	/* Set the driving capability (limit) of the device for TSC pins: 50mA */
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_TSC_I_DRIVE, 0x01);

	/* Touch screen control configuration (enable TSC):
	- No window tracking index
	- XYZ acquisition mode
	*/
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_TSC_CTRL, 0x01);

	/*  Clear all the status pending bits if any */
	TM_I2C_Write(STMPE811_I2C, STMPE811_ADDRESS, STMPE811_INT_STA, 0xFF);

	/* Wait for 2 ms delay */
	Delayms(2); 
	
	return TM_STMPE811_State_Ok;
}

uint8_t TM_STMPE811_Read(uint8_t reg) {
	return TM_I2C_Read(STMPE811_I2C, STMPE811_ADDRESS, reg);
}

TM_STMPE811_State_t TM_STMPE811_ReadTouch(TM_STMPE811_TouchData *structdata) {
	uint8_t val;
	val = TM_STMPE811_Read(STMPE811_TSC_CTRL);
	
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


