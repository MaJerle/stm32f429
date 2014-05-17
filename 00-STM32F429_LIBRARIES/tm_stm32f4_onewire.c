#include "tm_stm32f4_onewire.h"

GPIO_InitTypeDef TM_OneWire_GPIO_InitDef;

// global search state
uint8_t TM_OneWire_ROM_NO[8];
uint8_t TM_OneWire_LastDiscrepancy;
uint8_t TM_OneWire_LastFamilyDiscrepancy;
uint8_t TM_OneWire_LastDeviceFlag;

void TM_OneWire_Init(void) {
	TM_DELAY_Init();
	
	RCC_AHB1PeriphClockCmd(TM_ONEWIRE_RCC, ENABLE);
	
	TM_OneWire_GPIO_InitDef.GPIO_Pin = TM_ONEWIRE_PIN;
	TM_OneWire_GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	TM_OneWire_GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	TM_OneWire_GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
	TM_OneWire_GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(TM_ONEWIRE_PORT, &TM_OneWire_GPIO_InitDef);
}

uint8_t TM_OneWire_Reset(void) {
	uint8_t i;
	
	//Line low, and wait 480us
	TM_ONEWIRE_LOW;
	TM_ONEWIRE_OUTPUT;
	TM_ONEWIRE_DELAY(480);
	
	//Release line and wait for 80us
	TM_ONEWIRE_INPUT;
	TM_ONEWIRE_DELAY(60);
	
	//Check bit value
	i = GPIO_ReadInputDataBit(TM_ONEWIRE_PORT, TM_ONEWIRE_PIN);
	TM_ONEWIRE_DELAY(420);
	//Return value of presence pulse, 0 = OK, 1 = ERROR
	return i;
}

uint8_t TM_OneWire_ReadBit(void) {
	uint8_t bit = 0;
	//Line low
	TM_ONEWIRE_LOW;
	TM_ONEWIRE_OUTPUT;
	TM_ONEWIRE_DELAY(1);
	
	//Release line
	TM_ONEWIRE_INPUT;
	TM_ONEWIRE_DELAY(15);
	
	//Read line value
	if (GPIO_ReadInputDataBit(TM_ONEWIRE_PORT, TM_ONEWIRE_PIN)) {
		bit = 1;
	}
	
	//Wait 45us to complete 60us period
	TM_ONEWIRE_DELAY(45);
	
	return bit;
}

uint8_t TM_OneWire_ReadByte(void) {
	uint8_t i = 8, byte = 0;
	while (i--) {
		byte >>= 1;
		byte |= (TM_OneWire_ReadBit() << 7);
	}
	
	return byte;
}

void TM_OneWire_WriteBit(uint8_t bit) {
	//Line low
	TM_ONEWIRE_LOW;
	TM_ONEWIRE_OUTPUT;
	TM_ONEWIRE_DELAY(1);
	
	if (bit) {
		TM_ONEWIRE_INPUT;
	}
	
	//Wait for 60 us and release the line
	TM_ONEWIRE_DELAY(60);
	TM_ONEWIRE_INPUT;
}

void TM_OneWire_WriteByte(uint8_t byte) {
	uint8_t i = 8;
	while (i--) {
		//LSB bit is first
		TM_OneWire_WriteBit(byte & 0x01);
		byte >>= 1;
	}
}


uint8_t TM_OneWire_First(void) {
	TM_OneWire_ResetSearch();

	return TM_OneWire_Search(TM_ONEWIRE_CMD_SEARCHROM);
}


uint8_t TM_OneWire_Next(void) {
   // leave the search state alone
   return TM_OneWire_Search(TM_ONEWIRE_CMD_SEARCHROM);
}

void TM_OneWire_ResetSearch(void) {
	// reset the search state
	TM_OneWire_LastDiscrepancy = 0;
	TM_OneWire_LastDeviceFlag = 0;
	TM_OneWire_LastFamilyDiscrepancy = 0;
}

uint8_t TM_OneWire_Search(uint8_t command) {
	uint8_t id_bit_number;
	uint8_t last_zero, rom_byte_number, search_result;
	uint8_t id_bit, cmp_id_bit;
	uint8_t rom_byte_mask, search_direction;

	// initialize for search
	id_bit_number = 1;
	last_zero = 0;
	rom_byte_number = 0;
	rom_byte_mask = 1;
	search_result = 0;

	// if the last call was not the last one
	if (!TM_OneWire_LastDeviceFlag) {
		// 1-Wire reset
		if (TM_OneWire_Reset()) {
			// reset the search
			TM_OneWire_LastDiscrepancy = 0;
			TM_OneWire_LastDeviceFlag = 0;
			TM_OneWire_LastFamilyDiscrepancy = 0;
			return 0;
		}

		// issue the search command 
		TM_OneWire_WriteByte(command);  

		// loop to do the search
		do {
			// read a bit and its complement
			id_bit = TM_OneWire_ReadBit();
			cmp_id_bit = TM_OneWire_ReadBit();

			// check for no devices on 1-wire
			if ((id_bit == 1) && (cmp_id_bit == 1)) {
				break;
			} else {
				// all devices coupled have 0 or 1
				if (id_bit != cmp_id_bit) {
					search_direction = id_bit;  // bit write value for search
				} else {
					// if this discrepancy if before the Last Discrepancy
					// on a previous next then pick the same as last time
					if (id_bit_number < TM_OneWire_LastDiscrepancy) {
						search_direction = ((TM_OneWire_ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
					} else {
						// if equal to last pick 1, if not then pick 0
						search_direction = (id_bit_number == TM_OneWire_LastDiscrepancy);
					}
					
					// if 0 was picked then record its position in LastZero
					if (search_direction == 0) {
						last_zero = id_bit_number;

						// check for Last discrepancy in family
						if (last_zero < 9) {
							TM_OneWire_LastFamilyDiscrepancy = last_zero;
						}
					}
				}

				// set or clear the bit in the ROM byte rom_byte_number
				// with mask rom_byte_mask
				if (search_direction == 1) {
					TM_OneWire_ROM_NO[rom_byte_number] |= rom_byte_mask;
				} else {
					TM_OneWire_ROM_NO[rom_byte_number] &= ~rom_byte_mask;
				}
				
				// serial number search direction write bit
				TM_OneWire_WriteBit(search_direction);

				// increment the byte counter id_bit_number
				// and shift the mask rom_byte_mask
				id_bit_number++;
				rom_byte_mask <<= 1;

				// if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
				if (rom_byte_mask == 0) {
					//docrc8(ROM_NO[rom_byte_number]);  // accumulate the CRC
					rom_byte_number++;
					rom_byte_mask = 1;
				}
			}
		} while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

		// if the search was successful then
		if (!(id_bit_number < 65)) {
			// search successful so set LastDiscrepancy,LastDeviceFlag,search_result
			TM_OneWire_LastDiscrepancy = last_zero;

			// check for last device
			if (TM_OneWire_LastDiscrepancy == 0) {
				TM_OneWire_LastDeviceFlag = 1;
			}

			search_result = 1;
		}
	}

	// if no device found then reset counters so next 'search' will be like a first
	if (!search_result || !TM_OneWire_ROM_NO[0]) {
		TM_OneWire_LastDiscrepancy = 0;
		TM_OneWire_LastDeviceFlag = 0;
		TM_OneWire_LastFamilyDiscrepancy = 0;
		search_result = 0;
	}

	return search_result;
}

int TM_OneWire_Verify() {
	unsigned char rom_backup[8];
	int i,rslt,ld_backup,ldf_backup,lfd_backup;

	// keep a backup copy of the current state
	for (i = 0; i < 8; i++)
	rom_backup[i] = TM_OneWire_ROM_NO[i];
	ld_backup = TM_OneWire_LastDiscrepancy;
	ldf_backup = TM_OneWire_LastDeviceFlag;
	lfd_backup = TM_OneWire_LastFamilyDiscrepancy;

	// set search to find the same device
	TM_OneWire_LastDiscrepancy = 64;
	TM_OneWire_LastDeviceFlag = 0;

	if (TM_OneWire_Search(TM_ONEWIRE_CMD_SEARCHROM)) {
		// check if same device found
		rslt = 1;
		for (i = 0; i < 8; i++) {
			if (rom_backup[i] != TM_OneWire_ROM_NO[i]) {
				rslt = 1;
				break;
			}
		}
	} else {
		rslt = 0;
	}

	// restore the search state 
	for (i = 0; i < 8; i++) {
		TM_OneWire_ROM_NO[i] = rom_backup[i];
	}
	TM_OneWire_LastDiscrepancy = ld_backup;
	TM_OneWire_LastDeviceFlag = ldf_backup;
	TM_OneWire_LastFamilyDiscrepancy = lfd_backup;

	// return the result of the verify
	return rslt;
}

void TM_OneWire_TargetSetup(uint8_t family_code) {
   uint8_t i;

	// set the search state to find SearchFamily type devices
	TM_OneWire_ROM_NO[0] = family_code;
	for (i = 1; i < 8; i++) {
		TM_OneWire_ROM_NO[i] = 0;
	}
	TM_OneWire_LastDiscrepancy = 64;
	TM_OneWire_LastFamilyDiscrepancy = 0;
	TM_OneWire_LastDeviceFlag = 0;
}

void TM_OneWire_FamilySkipSetup() {
	// set the Last discrepancy to last family discrepancy
	TM_OneWire_LastDiscrepancy = TM_OneWire_LastFamilyDiscrepancy;
	TM_OneWire_LastFamilyDiscrepancy = 0;

	// check for end of list
	if (TM_OneWire_LastDiscrepancy == 0) {
		TM_OneWire_LastDeviceFlag = 1;
	}
}

uint8_t TM_OneWire_GetROM(uint8_t index) {
	return TM_OneWire_ROM_NO[index];
}

void TM_OneWire_Select(uint8_t addr[]) {
	uint8_t i;
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_MATCHROM);
	
	for (i = 0; i < 8; i++) {
		TM_OneWire_WriteByte(*(addr + i));
	}
}

void TM_OneWire_SelectWithPointer(uint8_t *ROM) {
	uint8_t i;
	TM_OneWire_WriteByte(TM_ONEWIRE_CMD_MATCHROM);
	
	for (i = 0; i < 8; i++) {
		TM_OneWire_WriteByte(*(ROM + i));
	}	
}

void TM_OneWire_GetFullROM(uint8_t *firstIndex) {
	uint8_t i;
	for (i = 0; i < 8; i++) {
		*(firstIndex + i) = TM_OneWire_ROM_NO[i];
	}
}
