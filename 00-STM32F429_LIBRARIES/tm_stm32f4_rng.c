#include "tm_stm32f4_rng.h"

void TM_RNG_Init(void) {
	//Enable RNG clock source
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);

	//RNG Peripheral enable
	RNG_Cmd(ENABLE);
}

uint32_t TM_RNG_Get(void) {
	//Wait until one RNG number is ready
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);

	//Get a 32bit Random number       
	return RNG_GetRandomNumber();
}