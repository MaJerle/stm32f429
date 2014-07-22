/**
 * Random number generator library for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/07/library-22-true-random-number-generator-stm32f4xx/
 *	@version 	v1.0
 *	@ide		Keil uVision
 *	
 *	This peripheral provides you a true random 32bit number
 */
#ifndef TM_RNG_
#define TM_RNG_ 100
/**
 * Dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx RNG
 * - defines.h
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_rng.h"
#include "defines.h"

/**
 * Initialize RNG
 *
 *
 */
extern void TM_RNG_Init(void);

/**
 * Get 32bit random number
 *
 * Returns 32bit randon number
 */
extern uint32_t TM_RNG_Get(void);

#endif

