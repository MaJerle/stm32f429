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
#include "tm_stm32f4_general.h"

/* System speed in MHz */
uint16_t GENERAL_SystemSpeedInMHz = 0;

/* Private functions */
static uint32_t x_na_y(uint32_t x, uint8_t y) {
	uint32_t output = 1;
	
	/* Make a "power" multiply */
	while (y--) {
		output *= x;
	}
	
	/* Return output value */
	return output;
}

void TM_GENERAL_SystemReset(void) {
	/* Call use function */
	TM_GENERAL_SoftwareResetCallback();
	
	/* Perform a system software reset */
	NVIC_SystemReset();
}

uint32_t TM_GENERAL_GetClockSpeed(TM_GENERAL_Clock_t clock) {
	static RCC_ClocksTypeDef RCC_Clocks;
	uint32_t c = 0;
	
	/* Check RCC clocks */
	if (RCC_Clocks.HCLK_Frequency == 0) {
		/* Read clocks */
		RCC_GetClocksFreq(&RCC_Clocks);
	}
	
	/* Return clock speed */
	switch (clock) {
		case TM_GENERAL_Clock_HSI:
			c = HSI_VALUE;
			break;
		case TM_GENERAL_Clock_HSE:
			c = HSE_VALUE;
			break;
		case TM_GENERAL_Clock_HCLK:
			c = RCC_Clocks.HCLK_Frequency;
			break;
		case TM_GENERAL_Clock_PCLK1:
			c = RCC_Clocks.PCLK1_Frequency;
			break;
		case TM_GENERAL_Clock_PCLK2:
			c = RCC_Clocks.PCLK2_Frequency;
			break;
		case TM_GENERAL_Clock_SYSCLK:
			c = RCC_Clocks.SYSCLK_Frequency;
			break;
		default:
			break;
	}
	
	/* Return clock */
	return c;
}

TM_GENERAL_ResetSource_t TM_GENERAL_GetResetSource(uint8_t reset_flags) {
	TM_GENERAL_ResetSource_t source = TM_GENERAL_ResetSource_None;

	/* Check bits */
	if (RCC->CSR & RCC_CSR_LPWRRSTF) {
		source = TM_GENERAL_ResetSource_LowPower;
	} else if (RCC->CSR & RCC_CSR_WWDGRSTF) {
		source = TM_GENERAL_ResetSource_WWDG;
	} else if (RCC->CSR & RCC_CSR_WDGRSTF) {
		source = TM_GENERAL_ResetSource_IWDG;
	} else if (RCC->CSR & RCC_CSR_SFTRSTF) {
		source = TM_GENERAL_ResetSource_Software;
	} else if (RCC->CSR & RCC_CSR_PORRSTF) {
		source = TM_GENERAL_ResetSource_POR;
	} else if (RCC->CSR & RCC_CSR_BORRSTF) {
		source = TM_GENERAL_ResetSource_BOR;
	} else if (RCC->CSR & RCC_CSR_PADRSTF) {
		source = TM_GENERAL_ResetSource_PIN;
	}
	
	/* Check for clearing flags */
	if (reset_flags) {
		RCC->CSR = RCC_CSR_RMVF;
	}
	
	/* Return source */
	return source;
}

uint8_t TM_GENERAL_DWTCounterEnable(void) {
	uint32_t c;
	
	/* Set clock speed if not already */
	if (GENERAL_SystemSpeedInMHz == 0) {
		/* Get clock speed in MHz */
		GENERAL_SystemSpeedInMHz = TM_GENERAL_GetClockSpeed(TM_GENERAL_Clock_SYSCLK) / 1000000;
	}
	
    /* Enable TRC */
    CoreDebug->DEMCR &= ~0x01000000;
    CoreDebug->DEMCR |=  0x01000000;
	
    /* Enable counter */
    DWT->CTRL &= ~0x00000001;
    DWT->CTRL |=  0x00000001;
	
    /* Reset counter */
    DWT->CYCCNT = 0;
	
	/* Check if DWT has started */
	c = DWT->CYCCNT;
	
	/* 2 dummys */
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	
	/* Return difference, if result is zero, DWT has not started */
	return (DWT->CYCCNT - c);
}

void TM_GENERAL_ConvertFloat(TM_GENERAL_Float_t* Float_Struct, float Number, uint8_t decimals) {
	/* Check decimals */
	if (decimals > 9) {
		decimals = 9;
	}
	
	/* Get integer part */
	Float_Struct->Integer = (int32_t)Number;
	
	/* Get decimal part */
	if (Number < 0) {
		Float_Struct->Decimal = (int32_t)((float)(Float_Struct->Integer - Number) * x_na_y(10, decimals));
	} else {
		Float_Struct->Decimal = (int32_t)((float)(Number - Float_Struct->Integer) * x_na_y(10, decimals));
	}
}

float TM_GENERAL_RoundFloat(float Number, uint8_t decimals) {
	float x;
		
	/* Check decimals */
	if (decimals > 9) {
		decimals = 9;
	}
	
	x = x_na_y(10, decimals);
	
	/* Make truncating */
	if (Number > 0) {
		return (float)(Number * x + (float)0.5) / x;
	} else if (Number < 0) {
		return (float)(Number * x - (float)0.5) / x;
	}
	
	/* Return number */
	return 0;
}
