#include "global_includes.h"
#include "GUIDRV_Lin.h"
#include "tm_stm32f4_disco.h"
#include "stdio.h"
#include "tm_stm32f4_delay.h"

/* Function to fill rectangle with single color */
static void _LCD_FillRect(int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex) {
	int16_t Width = GUI_ABS(x1 - x0);
	int16_t Height = GUI_ABS(y1 - y0);
	
	//printf("X0: %d; Y0: %d; X1: %d; Y1: %d\n", x0, y0, x1, y1);
	TM_DISCO_LedToggle(LED_GREEN);
	
	if (Width == 0 && Height == 0) {
		/* Draw single pixel */
		*(__IO uint16_t *)(SDRAM_START_ADR + 2 * (640 * y0 + x0)) = PixelIndex;
	} else if (Width == 0) {
		TM_DMA2DGRAPHIC_DrawVerticalLine(x0, y0, Height, PixelIndex);
	} else if (Height == 0) {
		TM_DMA2DGRAPHIC_DrawHorizontalLine(x0, y0, Width, PixelIndex);
	} else {
		/* Draw filled rectangle using DMA2D */
		TM_DMA2DGRAPHIC_DrawFilledRectangle(x0, y0, Width + 1, Height + 1, PixelIndex);
	}
}

static void _LCD_CopyBuffer(int LayerIndex, void* pSrc, void* pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLineSrc, uint32_t OffLineDst) {
	TM_DISCO_LedToggle(LED_RED);
	/* Make a copy */
	TM_DMA2DGRAPHIC_CopyBuffer(pSrc, pDst, xSize, ySize, OffLineSrc, OffLineDst);
}

static void _LCD_CopyRect(int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize) {
	TM_DISCO_LedToggle(LED_BLUE);
	/* Make a copy */
	//TM_DMA2DGRAPHIC_CopyBuffer((void *)AddrSrc, (void *)AddrDst, xSize, ySize, 640 - xSize, 0);
}

void LCD_X_Config(void) {
	/* Link driver type for LCD */
	GUI_DEVICE_CreateAndLink(GUIDRV_LIN_16, GUICC_565, 0, 0);

	/* Set layer width and height */
	LCD_SetSizeEx(0, TM_LCD_GetWidth(), TM_LCD_GetHeight());
	LCD_SetVSizeEx(0, TM_LCD_GetWidth(), TM_LCD_GetHeight());
	
	/* Set custom functions */
	LCD_SetDevFunc(0, LCD_DEVFUNC_COPYBUFFER, (void(*)(void))_LCD_CopyBuffer);
	LCD_SetDevFunc(0, LCD_DEVFUNC_COPYRECT,   (void(*)(void))_LCD_CopyRect);
	LCD_SetDevFunc(0, LCD_DEVFUNC_FILLRECT,   (void(*)(void))_LCD_FillRect);
	
	/* Set LCD RAM */
	LCD_SetVRAMAddrEx(0, (void *)TM_LCD_GetFrameBuffer());
}

int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
	return 0;
}
