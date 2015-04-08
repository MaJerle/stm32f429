/**
 *	Keil project for I2C search devices feature
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@conf		PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 *	@packs		STM32F4xx Keil packs version 2.4.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.5.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_ili9341_ltdc.h"
#include "tm_stm32f4_i2c.h"

#include "stdio.h"

int main(void) {
	uint16_t i;
	uint8_t devices_found = 0;
	char buffer[20];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	
	/* Init LCD on STM32F429-Discovery board */
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);
	
	/* Set background color */
	TM_ILI9341_Fill(0x1234);
	
	/* Init I2C3 peripheral, PA8, PC9 */
	TM_I2C_Init(I2C3, TM_I2C_PinsPack_1, 100000);
	
	/* Make search and show on LCD */
	for (i = 2; i < 0xFF; i += 2) {
		/* Check if device detected */
		if (TM_I2C_IsDeviceConnected(I2C3, i)) {
			/* Format string */
			sprintf(buffer, "Device: %02X", i);
			/* Print to LCD */
			TM_ILI9341_Puts(10, 10 + 12 * devices_found++, buffer, &TM_Font_7x10, 0x0000, 0x1234);
		}
	}
	
	while (1) {
		
	}
}
