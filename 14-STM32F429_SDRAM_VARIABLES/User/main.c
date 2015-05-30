/**
 *	Keil project for external SDRAM with data arrays
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
 *
 * In this project, compiler will set location for "BigArray" variable at external memory on STM32F429-Discovery board.
 * Variable is too big to be saved into internal RAM of device.
 *
 * To tell compiler, that we have external RAM memory, open "Options for Target" and in "Target" tab use
 * IRAM2 value and type location where it starts and how big it is, in our case is set:
 *
 * Start address: 0xD0000000
 * Length:        0x800000
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"

/* Create small array and force linker to put it to external RAM at location 0xD0001000 which is external RAM */
uint32_t SmallArray[10] __attribute__((at(0xD0001000)));

/* Create very big array, compiler will use external RAM because its size is too big for internal RAM on STM32F429 */
uint32_t BigArray[0x100000];

int main(void) {
	uint32_t i, c;
	
	/* Initialize system */
	/* SystemInit function is called from startup file, so no need to call it again here. */
	/* Also, SDRAM must be initialized before main function starts. */
	/* For that case, at the end of SystemInit() function, TM_SDRAM_Init() function is called which prepares external RAM */
	//SystemInit();
	
	/* Get size of big array */
	c = sizeof(BigArray) / sizeof(uint32_t);
	
	/* Write something to external memory */
	for (i = 0; i < c; i++) {
		BigArray[i] = i;
	}
	
	/* Get size of small array */
	c = sizeof(BigArray) / sizeof(uint32_t);
	
	/* Write something to external memory */
	for (i = 0; i < c; i++) {
		SmallArray[i] = i;
	}

	/* While loop */
	while (1) {

	}
}
