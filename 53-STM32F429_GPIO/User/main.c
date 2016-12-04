/**
 *	Keil project for GPIO pins
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@conf		PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_gpio.h"

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize pins for LEDS on discovery boards */
#if defined(TM_DISCO_STM32F4_DISCOVERY)
	/* Init pins PD12, 13, 14, 15 */
	TM_GPIO_Init(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
#elif defined(TM_DISCO_STM32F429_DISCOVERY)
	/* Init pins PG13, 14 */
	TM_GPIO_Init(GPIOG, GPIO_Pin_13 | GPIO_Pin_14, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
#elif defined(TM_DISCO_NUCLEO_F401) || defined(TM_DISCO_NUCLEO_F411)
	/* Init pin PA5 */
	TM_GPIO_Init(GPIOA, GPIO_Pin_5, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High);
#endif

	while (1) {
		/* Toggle leds */
#if defined(TM_DISCO_STM32F4_DISCOVERY)
		/* Init pins PD12, 13, 14, 15 */
		TM_GPIO_TogglePinValue(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
#elif defined(TM_DISCO_STM32F429_DISCOVERY)
		/* Init pins PG13, 14 */
		TM_GPIO_TogglePinValue(GPIOG, GPIO_Pin_13 | GPIO_Pin_14);
#elif defined(TM_DISCO_NUCLEO_F401) || defined(TM_DISCO_NUCLEO_F411)
		/* Init pin PA5 */
		TM_GPIO_TogglePinValue(GPIOA, GPIO_Pin_5);
#endif
		
		/* Delay */
		Delayms(500);
	}
}
