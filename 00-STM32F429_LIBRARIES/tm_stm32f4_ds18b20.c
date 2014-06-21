#include "tm_stm32f4_ds18b20.h"

uint8_t TM_DS18B20_Start(uint8_t *ROM) {
	if (!TM_DS18B20_Is(ROM)) {
		return 0;
	}
	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Start temperature conversion
	TM_OneWire_WriteByte(TM_DS18B20_CMD_CONVERTTEMP);
	
	return 1;
}

void TM_DS18B20_StartAll(void) {
	//Reset pulse
	TM_OneWire_Reset();
	//Skip rom
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_SKIPROM);
	//Start conversion on all connected devices
	TM_OneWire_WriteByte(TM_DS18B20_CMD_CONVERTTEMP);
}

uint8_t TM_DS18B20_Read(uint8_t *ROM, float *destination) {
	uint16_t temperature;
	uint8_t resolution;
	int8_t digit, minus = 0;
	float decimal;
	
	if (!TM_DS18B20_Is(ROM)) {
		return 0;
	}
	
	//Check if line is released, if it is, then conversion is complete
	if (!TM_OneWire_ReadBit()) {
		return 0; //Conversion is not finished yet
	}

	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Read scratchpad command by onewire protocol
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_RSCRATCHPAD);
	
	//First two bytes of scratchpad are temperature values
	temperature = TM_OneWire_ReadByte() | (TM_OneWire_ReadByte() << 8);

	//Reset line
	TM_OneWire_Reset();
	
	if (((temperature >> 15)) == 1) {
		//Two's complement, temperature is negative
		temperature = ~temperature + 1;
		minus = 1;
	}

	//Get sensor resolution
	resolution = TM_DS18B20_GetResolution(ROM);
	
	//Store temperature integer digits and decimal digits
	digit = temperature >> 4;
	digit |= ((temperature >> 8) & 0x7) << 4;
	//Store decimal digits
	switch (resolution) {
		case 9: {
			decimal = (temperature >> 3) & 0x01;
			decimal *= (float)TM_DS18B20_DECIMAL_STEPS_9BIT;
		} break;
		case 10: {
			decimal = (temperature >> 2) & 0x03;
			decimal *= (float)TM_DS18B20_DECIMAL_STEPS_10BIT;
		} break;
		case 11: {
			decimal = (temperature >> 1) & 0x07;
			decimal *= (float)TM_DS18B20_DECIMAL_STEPS_11BIT;
		} break;
		case 12: {
			decimal = temperature & 0x0F;
			decimal *= (float)TM_DS18B20_DECIMAL_STEPS_12BIT;
		} break;
		default: {
			decimal = 0xFF;
			digit = 0;
		}
	}
	
	decimal = digit + decimal;
	if (minus) {
		decimal = 0 - decimal;
	}
	
	*destination = decimal;
	
	return 1;
}

uint8_t TM_DS18B20_GetResolution(uint8_t *ROM) {
	uint8_t conf;
	if (!TM_DS18B20_Is(ROM)) {
		return 0;
	}
	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Read scratchpad command by onewire protocol
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_RSCRATCHPAD);
	
	TM_OneWire_ReadByte();
	TM_OneWire_ReadByte();
	TM_OneWire_ReadByte();
	TM_OneWire_ReadByte();
	//5th byte of scratchpad is configuration register
	conf = TM_OneWire_ReadByte();
	
	return ((conf & 0x60) >> 5) + 9;
}

uint8_t TM_DS18B20_SetResolution(uint8_t *ROM, TM_DS18B20_Resolution_t resolution) {
	uint8_t th, tl, conf;
	if (!TM_DS18B20_Is(ROM)) {
		return 0;
	}
	
	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Read scratchpad command by onewire protocol
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_RSCRATCHPAD);
	
	TM_OneWire_ReadByte();
	TM_OneWire_ReadByte();
	
	th = TM_OneWire_ReadByte();
	tl = TM_OneWire_ReadByte();
	conf = TM_OneWire_ReadByte();
	
	if (resolution == TM_DS18B20_Resolution_9bits) {
		conf &= ~(1 << TM_DS18B20_RESOLUTION_R1);
		conf &= ~(1 << TM_DS18B20_RESOLUTION_R0);
	} else if (resolution == TM_DS18B20_Resolution_10bits) {
		conf &= ~(1 << TM_DS18B20_RESOLUTION_R1);
		conf |= 1 << TM_DS18B20_RESOLUTION_R0;
	} else if (resolution == TM_DS18B20_Resolution_11bits) {
		conf |= 1 << TM_DS18B20_RESOLUTION_R1;
		conf &= ~(1 << TM_DS18B20_RESOLUTION_R0);
	} else if (resolution == TM_DS18B20_Resolution_12bits) {
		conf |= 1 << TM_DS18B20_RESOLUTION_R1;
		conf |= 1 << TM_DS18B20_RESOLUTION_R0;
	}
	
	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Write scratchpad command by onewire protocol, only th, tl and conf register can be written
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_WSCRATCHPAD);
	
	//Write bytes
	TM_OneWire_WriteByte(th);
	TM_OneWire_WriteByte(tl);
	TM_OneWire_WriteByte(conf);
	
	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Copy scratchpad to EEPROM of DS18B20
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_CPYSCRATCHPAD);
	
	return 1;
}

uint8_t TM_DS18B20_Is(uint8_t *ROM) {
	//Checks if first byte is equal to DS18B20's family code (0x28)
	if (*ROM == TM_DS18B20_FAMILY_CODE) {
		return 1;
	}
	return 0;
}

uint8_t TM_DS18B20_SetAlarmLowTemperature(uint8_t *ROM, int8_t temp) {
	uint8_t tl, th, conf;
	if (!TM_DS18B20_Is(ROM)) {
		return 0;
	}
	if (temp > 125) {
		temp = 125;
	} 
	if (temp < -55) {
		temp = -55;
	}
	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Read scratchpad command by onewire protocol
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_RSCRATCHPAD);
	
	TM_OneWire_ReadByte();
	TM_OneWire_ReadByte();
	
	th = TM_OneWire_ReadByte();
	tl = TM_OneWire_ReadByte();
	conf = TM_OneWire_ReadByte();
	
	tl = (uint8_t)temp; 

	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Write scratchpad command by onewire protocol, only th, tl and conf register can be written
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_WSCRATCHPAD);
	
	//Write bytes
	TM_OneWire_WriteByte(th);
	TM_OneWire_WriteByte(tl);
	TM_OneWire_WriteByte(conf);
	
	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Copy scratchpad to EEPROM of DS18B20
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_CPYSCRATCHPAD);
	
	return 1;
}

uint8_t TM_DS18B20_SetAlarmHighTemperature(uint8_t *ROM, int8_t temp) {
	uint8_t tl, th, conf;
	if (!TM_DS18B20_Is(ROM)) {
		return 0;
	}
	if (temp > 125) {
		temp = 125;
	} 
	if (temp < -55) {
		temp = -55;
	}
	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Read scratchpad command by onewire protocol
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_RSCRATCHPAD);
	
	TM_OneWire_ReadByte();
	TM_OneWire_ReadByte();
	
	th = TM_OneWire_ReadByte();
	tl = TM_OneWire_ReadByte();
	conf = TM_OneWire_ReadByte();
	
	th = (uint8_t)temp; 

	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Write scratchpad command by onewire protocol, only th, tl and conf register can be written
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_WSCRATCHPAD);
	
	//Write bytes
	TM_OneWire_WriteByte(th);
	TM_OneWire_WriteByte(tl);
	TM_OneWire_WriteByte(conf);
	
	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Copy scratchpad to EEPROM of DS18B20
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_CPYSCRATCHPAD);
	
	return 1;
}

uint8_t TM_DS18B20_DisableAlarmTemperature(uint8_t *ROM) {
	uint8_t tl, th, conf;
	if (!TM_DS18B20_Is(ROM)) {
		return 0;
	}
	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Read scratchpad command by onewire protocol
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_RSCRATCHPAD);
	
	TM_OneWire_ReadByte();
	TM_OneWire_ReadByte();
	
	th = TM_OneWire_ReadByte();
	tl = TM_OneWire_ReadByte();
	conf = TM_OneWire_ReadByte();
	
	th = 125;
	tl = (uint8_t)-55;

	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Write scratchpad command by onewire protocol, only th, tl and conf register can be written
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_WSCRATCHPAD);
	
	//Write bytes
	TM_OneWire_WriteByte(th);
	TM_OneWire_WriteByte(tl);
	TM_OneWire_WriteByte(conf);
	
	//Reset line
	TM_OneWire_Reset();
	//Select ROM number
	TM_OneWire_SelectWithPointer(ROM);
	//Copy scratchpad to EEPROM of DS18B20
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_CPYSCRATCHPAD);
	
	return 1;
}

uint8_t TM_DS18B20_AlarmSearch(void) {
	return TM_OneWire_Search(TM_DS18B20_CMD_ALARMSEARCH);
}

uint8_t TM_DS18B20_AllDone(void) {
	return TM_OneWire_ReadBit();
}


