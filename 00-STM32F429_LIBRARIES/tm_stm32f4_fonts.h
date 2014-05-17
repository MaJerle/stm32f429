/**
 * Fonts library
 */
#ifndef TM_FONTS_H
#define TM_FONTS_H

#include "stm32f4xx.h"
#include "defines.h"
/**
 * Font struct
 */
typedef struct {
	uint8_t FontWidth;
	uint8_t FontHeight;
	const uint16_t *data;
} TM_FontDef_t;

#endif

