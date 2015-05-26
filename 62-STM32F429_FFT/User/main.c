/**
 *	Keil project example for FFT on STM32F4 device.
 *
 *	Works on STM32F429-Discovery board because it has LCD
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 *
 *	Notes: 
 *		- Under "Options for target" > "C/C++" > "Define" you must add 2 defines (I've already add them):
 *			- ARM_MATH_CM4
 *			- __FPU_PRESENT=1
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_ili9341_ltdc.h"
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_sdram.h"
#include "tm_stm32f4_dac_signal.h"
#include "tm_stm32f4_fft.h"

#include <stdio.h>

/* Include arm_math.h mathematic functions */
#include "arm_math.h"

/* FFT settings */
#define SAMPLES					(512)         /* 256 real party and 256 imaginary parts */
#define FFT_SIZE				(SAMPLES / 2) /* FFT size is always the same size as we have samples, so 256 in our case */

#define FFT_BAR_MAX_HEIGHT		120           /* 120 px on the LCD */

/* Global variables */
float32_t Input[SAMPLES];   /*!< Input buffer is always 2 * FFT_SIZE */
float32_t Output[FFT_SIZE]; /*!< Output buffer is always FFT_SIZE */

/* Draw bar for LCD */
/* Simple library to draw bars */
void DrawBar(uint16_t bottomX, uint16_t bottomY, uint16_t maxHeight, uint16_t maxValue, float32_t value, uint16_t foreground, uint16_t background) {
	uint16_t height;
	height = (uint16_t)((float32_t)value / (float32_t)maxValue * (float32_t)maxHeight);
	if (height == maxHeight) {
		TM_ILI9341_DrawLine(bottomX, bottomY, bottomX, bottomY - height, foreground);
	} else if (height < maxHeight) {
		TM_ILI9341_DrawLine(bottomX, bottomY, bottomX, bottomY - height, foreground);
		TM_ILI9341_DrawLine(bottomX, bottomY - height, bottomX, bottomY - maxHeight, background);
	}
}

int main(void) {
	TM_FFT_F32_t FFT;    /*!< FFT structure */
	uint16_t i;
	uint32_t frequency = 10000;
	
	/* Initialize system */
	SystemInit();
	
	/* Delay init */
	TM_DELAY_Init();
	
	/* Initialize LED's on board */
	TM_DISCO_LedInit();
	
	/* Initialize LCD */
	TM_ILI9341_Init();
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_1);
	
	/* Initialize DAC2, PA5 for fake sinus, use TIM4 to generate DMA */
	TM_DAC_SIGNAL_Init(TM_DAC2, TIM4);
	
	/* Set sinus with 10kHz */
	TM_DAC_SIGNAL_SetSignal(TM_DAC2, TM_DAC_SIGNAL_Signal_Sinus, frequency);

	/* Initialize ADC, PA0 is used */
	TM_ADC_Init(ADC1, ADC_Channel_0);
	
	/* Print something on LCD */
	TM_ILI9341_Puts(10, 10, "FFT example STM32F4xx\nstm32f4-discovery.com", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_GREEN2);
	
	/* Init FFT, FFT_SIZE define is used for FFT_SIZE, samples count is FFT_SIZE * 2, don't use malloc for memory allocation */
	TM_FFT_Init_F32(&FFT, FFT_SIZE, 0);
	
	/* We didn't used malloc for allocation, so we have to set pointers ourself */
	/* Input buffer must be 2 * FFT_SIZE in length because of real and imaginary part */
	/* Output buffer must be FFT_SIZE in length */
	TM_FFT_SetBuffers_F32(&FFT, Input, Output);
	
	while (1) {
		/* This part should be done with DMA and timer for ADC treshold */
		/* Actually, best solution is double buffered DMA with timer for ADC treshold */
		/* But this is only for show principle on how FFT works */
		/* Fill buffer until function returns 1 = Buffer full and samples ready to be calculated */
		while (!TM_FFT_AddToBuffer(&FFT, (TM_ADC_Read(ADC1, ADC_Channel_0) - (float32_t)2048.0))) {
			/* Little delay, fake 45kHz sample rate for audio */
			Delay(21);
		}
			
		/* Do FFT on signal, values at each bin and calculate max value and index where max value happened */
		TM_FFT_Process_F32(&FFT);

		/* Display data on LCD */
		for (i = 0; i < TM_FFT_GetFFTSize(&FFT); i++) {
			/* Draw FFT results */
			DrawBar(30 + 2 * i,
					220,
					FFT_BAR_MAX_HEIGHT,
					TM_FFT_GetMaxValue(&FFT),
					TM_FFT_GetFromBuffer(&FFT, i),
					0x1234,
					0xFFFF
			);
		}
	}
}
