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
#include "tm_stm32f4_hcsr04.h"

uint8_t TM_HCSR04_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Init Delay functions */
	TM_DELAY_Init();
	
	/* Initialize pins */
	/* Enable clock */
	RCC_AHB1PeriphClockCmd(HCSR04_TRIGGER_RCC | HCSR04_ECHO_RCC, ENABLE);
	
	/* Set common settings */
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	/* Trigger pin */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = HCSR04_TRIGGER_PIN;
	GPIO_Init(HCSR04_TRIGGER_PORT, &GPIO_InitStruct);
	
	/* Echo pin */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = HCSR04_ECHO_PIN;
	GPIO_Init(HCSR04_ECHO_PORT, &GPIO_InitStruct);
	
	/* Trigger set to low */
	HCSR04_TRIGGER_LOW;
	
	/* Start measurement, check if sensor is working */
	if (TM_HCSR04_Read() >= 0) {
		/* Sensor OK */
		return 1;
	}
	/* Sensor not ok */
	return 0;
}

float TM_HCSR04_Read(void) {
	uint32_t time, timeout;
	float dis;
	/* Trigger low */
	HCSR04_TRIGGER_LOW;
	/* Delay 2 us */
	Delay(2);
	/* Trigger high for 10us */
	HCSR04_TRIGGER_HIGH;
	/* Delay 10 us */
	Delay(10);
	/* Trigger low */
	HCSR04_TRIGGER_LOW;
	
	/* Give some time for response */
	timeout = HCSR04_TIMEOUT;
	while (HCSR04_ECHO_CHECK == Bit_RESET) {
		if (timeout-- == 0x00) {
			return -1;
		}
	}
	
	/* Start time */
	time = 0;
	/* Wait till signal is low */
	while (HCSR04_ECHO_CHECK == Bit_SET) {
		/* Increase time */
		time++;
		/* Delay 1us */
		Delay(1);
	}
	
	/* Convert us to cm */
	dis = (float)time * HCSR04_NUMBER;
	/* Return distance */
	return dis;
}
