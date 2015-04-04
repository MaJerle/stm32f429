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
#include "tm_stm32f4_am2301.h"

/* Private functions */
TM_AM2301_t TM_AM2301_INT_Read(TM_AM2301_Data_t* data);
TM_AM2301_t TM_AM2301_INT_InitPins(void);

TM_AM2301_t TM_AM2301_Init(void) {
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize pin */
	TM_GPIO_Init(AM2301_PORT, AM2301_PIN, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	
	/* Return OK */
	return TM_AM2301_OK;
}

TM_AM2301_t TM_AM2301_Read(TM_AM2301_Data_t* data) {
	/* Read data */
	return TM_AM2301_INT_Read(data);
}

/* Internal function */
TM_AM2301_t TM_AM2301_INT_Read(TM_AM2301_Data_t* data) {
	volatile uint32_t time;
	uint8_t i, j, d[5];
	
	/* Pin output */
	AM2301_PIN_OUT;
	/* Set pin low for ~800-1000 us */
	AM2301_PIN_LOW;
	Delay(1000);
	/* Set pin high to ~30 us */
	AM2301_PIN_HIGH;
	Delay(30);
	
	/* Read mode */
	AM2301_PIN_IN;
	
	time = 0;
	/* Wait 20us for acknowledge, low signal */
	while (AM2301_PIN_READ) {
		if (time > 20) {
			return TM_AM2301_CONNECTION_ERROR;
		}
		/* Increase time */
		time++;
		/* Wait 1 us */
		Delay(1);
	}
	
	time = 0;
	/* Wait high signal, about 80-85us long (measured with logic analyzer) */
	while (!AM2301_PIN_READ) {
		if (time > 85) {
			return TM_AM2301_WAITHIGH_ERROR;
		}
		/* Increase time */
		time++;
		/* Wait 1 us */
		Delay(1);
	}
	
	time = 0;
	/* Wait low signal, about 80-85us long (measured with logic analyzer) */
	while (AM2301_PIN_READ) {
		if (time > 85) {
			return TM_AM2301_WAITLOW_ERROR;
		}
		/* Increase time */
		time++;
		/* Wait 1 us */
		Delay(1);
	}
	
	/* Read 5 bytes */
	for (j = 0; j < 5; j++) {
		d[j] = 0;
		for (i = 8; i > 0; i--) {
			/* We are in low signal now, wait for high signal and measure time */
			time = 0;
			/* Wait high signal, about 57-63us long (measured with logic analyzer) */
			while (!AM2301_PIN_READ) {
				if (time > 75) {
					return TM_AM2301_WAITHIGH_LOOP_ERROR;
				}
				/* Increase time */
				time++;
				/* Wait 1 us */
				Delay(1);
			}
			/* High signal detected, start measure high signal, it can be 26us for 0 or 70us for 1 */
			time = 0;
			/* Wait low signal, between 26 and 70us long (measured with logic analyzer) */
			while (AM2301_PIN_READ) {
				if (time > 90) {
					return TM_AM2301_WAITLOW_LOOP_ERROR;
				}
				/* Increase time */
				time++;
				/* Wait 1 us */
				Delay(1);
			}
			
			if (time > 18 && time < 37) {
				/* We read 0 */
			} else {
				/* We read 1 */
				d[j] |= 1 << (i - 1);
			}
		}
	}
	
	/* Check for parity */
	if (((d[0] + d[1] + d[2] + d[3]) & 0xFF) != d[4]) {
		/* Parity error, data not valid */
		return TM_AM2301_PARITY_ERROR;
	}
	
	/* Set humidity */
	data->Hum = d[0] << 8 | d[1];
	/* Negative temperature */
	if (d[2] & 0x80) {
		data->Temp = -((d[2] & 0x7F) << 8 | d[3]);
	} else {
		data->Temp = (d[2]) << 8 | d[3];
	}
	
	/* Data OK */
	return TM_AM2301_OK;
}

