/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2015
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
#include "tm_stm32f4_lcd.h"

/* Private structures, variables and functions */
static void TM_LCD_INT_InitLTDC(void);
static void TM_LCD_INT_InitLayers(void);
static void TM_LCD_INT_InitLCD(void);
static void TM_LCD_INT_InitPins(void);

/* Private structure */
typedef struct _TM_LCD_INT_t {
	uint16_t Width;
	uint16_t Height;
	uint32_t CurrentFrameBuffer;
	uint32_t FrameStart;
	uint32_t FrameOffset;
	uint8_t CurrentLayer;
	TM_FontDef_t* CurrentFont;
	uint32_t ForegroundColor;
	uint32_t BackgroundColor;
	uint16_t CurrentX;
	uint16_t CurrentY;
} TM_LCD_INT_t;
static TM_LCD_INT_t LCD;

TM_LCD_Result_t TM_LCD_Init(void) {
	TM_DMA2DGRAPHIC_INT_Conf_t DMA2DConf;
	
	/* Init SDRAM */
	if (!TM_SDRAM_Init()) {
		/* Return error */
		return TM_LCD_Result_SDRAM_Error;
	}
	
	/* Fill default structure */
	LCD.Width = LCD_PIXEL_WIDTH;
	LCD.Height = LCD_PIXEL_HEIGHT;
	LCD.CurrentFrameBuffer = LCD_FRAME_BUFFER;
	LCD.FrameStart = LCD_FRAME_BUFFER;
	LCD.FrameOffset = LCD_BUFFER_OFFSET;
	LCD.CurrentFont = &TM_Font_11x18;
	LCD.ForegroundColor = 0x0000;
	LCD.BackgroundColor = 0xFFFF;
	
	/* Set configrations for DMA2D */
	DMA2DConf.BufferStart = LCD_FRAME_BUFFER;
	DMA2DConf.BufferOffset = LCD_BUFFER_OFFSET;
	DMA2DConf.BytesPerPixel = 2;
	DMA2DConf.Height = LCD_PIXEL_HEIGHT;
	DMA2DConf.Width = LCD_PIXEL_WIDTH;
	DMA2DConf.Orientation = 1;
	
	/* Init LCD pins */
	TM_LCD_INT_InitPins();
	
	/* Init LTDC peripheral */
	TM_LCD_INT_InitLTDC();
	
	/* Init LTDC layers */
	TM_LCD_INT_InitLayers();
	
	/* Init LCD dependant settings */
	TM_LCD_INT_InitLCD();
	
	/* Init DMA2D graphics */
	TM_DMA2DGRAPHIC_Init();
	
	/* Set settings */
	TM_INT_DMA2DGRAPHIC_SetConf(&DMA2DConf);
	
	/* Enable LCD */
	TM_LCD_DisplayOn();
	
	TM_LCD_SetLayer1();
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_Fill(uint32_t color) {
	/* Erase memory */
	TM_DMA2DGRAPHIC_Fill(color);
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_DisplayOn(void) {
	/* Enable LTDC */
	LTDC->GCR |= LTDC_GCR_LTDCEN;
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_DisplayOff(void) {
	/* Disable LTDC */
	LTDC->GCR &= ~LTDC_GCR_LTDCEN;
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_SetXY(uint16_t X, uint16_t Y) {
	/* Check if we are inside LCD */
	if (X >= LCD.Width || Y >= LCD.Height) {
		return TM_LCD_Result_Error;
	}
	
	/* Set new values */
	LCD.CurrentX = X;
	LCD.CurrentY = Y;
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_SetFont(TM_FontDef_t* Font) {
	/* Set new font used for drawing */
	LCD.CurrentFont = Font;
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_SetColors(uint32_t Foreground, uint32_t Background) {
	/* Set new colors */
	LCD.ForegroundColor = Foreground;
	LCD.BackgroundColor = Background;
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_Putc(char c) {
	uint32_t i, b, j;
	
	/* Check current coordinates */
	if ((LCD.CurrentX + LCD.CurrentFont->FontWidth) >= LCD.Width) {
		/* If at the end of a line of display, go to new line and set x to 0 position */
		LCD.CurrentY += LCD.CurrentFont->FontHeight;
		LCD.CurrentY = 0;
		
		/* Check for Y position */
		if (LCD.CurrentY >= LCD.Height) {
			/* Return error */
			return TM_LCD_Result_Error;
		}
	}
	
	/* Draw all pixels */
	for (i = 0; i < LCD.CurrentFont->FontHeight; i++) {
		b = LCD.CurrentFont->data[(c - 32) * LCD.CurrentFont->FontHeight + i];
		for (j = 0; j < LCD.CurrentFont->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				TM_LCD_DrawPixel(LCD.CurrentX + j, (LCD.CurrentY + i), LCD.ForegroundColor);
			} else {
				TM_LCD_DrawPixel(LCD.CurrentX + j, (LCD.CurrentY + i), LCD.BackgroundColor);
			}
		}
	}
	
	/* Go to new X location */
	TM_LCD_SetXY(LCD.CurrentX + LCD.CurrentFont->FontWidth, LCD.CurrentY);
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_DrawPixel(uint16_t X, uint16_t Y, uint32_t color) {
	/* Draw pixel at desired location */
	*(__IO uint16_t *) (LCD.CurrentFrameBuffer + 2 * ((Y * LCD.Width) + X)) = color;
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

uint32_t TM_LCD_GetPixel(uint16_t X, uint16_t Y) {
	/* Get pixel at desired location */
	return *(__IO uint16_t *) (LCD.CurrentFrameBuffer + 2 * ((Y * LCD.Width) + X));
}

TM_LCD_Result_t TM_LCD_Puts(char* str) {
	/* Send till string ends or error returned */
	while (*str) {
		/* Check if string OK */
		if (TM_LCD_Putc(*str) != TM_LCD_Result_Ok) {
			/* Return error */
			return TM_LCD_Result_Error;
		}
		
		/* Increase pointer */
		str++;
	}
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_SetLayer1(void) {
	/* Fill settings */
	LCD.CurrentFrameBuffer = LCD.FrameStart;
	LCD.CurrentLayer = 0;
	
	/* Set layer for DMA2D also */
	//TM_DMA2DGRAPHIC_SetLayer(LCD.CurrentLayer + 1);
	
	/* Return OK */
	return TM_LCD_Result_Ok;

}

TM_LCD_Result_t TM_LCD_SetLayer2(void) {
	/* Fill settings */
	LCD.CurrentFrameBuffer = LCD.FrameStart + LCD.FrameOffset;
	LCD.CurrentLayer = 1;
	
	/* Set layer for DMA2D also */
	//TM_DMA2DGRAPHIC_SetLayer(LCD.CurrentLayer + 1);
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_SetLayer1Opacity(uint8_t opacity) {
	/* Set opacity */
	LTDC_Layer1->CACR = opacity;
	
	/* Reload */
	LTDC->SRCR = LTDC_SRCR_IMR;
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_SetLayer2Opacity(uint8_t opacity) {
	/* Set opacity */
	LTDC_Layer2->CACR = opacity;
	
	/* Reload */
	LTDC->SRCR = LTDC_SRCR_IMR;
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_ChangeLayers(void) {
	/* Change layers */
	if (LCD.CurrentLayer == 0) {
		TM_LCD_SetLayer2();
		TM_LCD_SetLayer1Opacity(0);
		TM_LCD_SetLayer2Opacity(255);
	} else {
		TM_LCD_SetLayer1();
		TM_LCD_SetLayer1Opacity(255);
		TM_LCD_SetLayer2Opacity(0);
	}
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_Layer2To1(void) {
	/* Copy buffer using DMA2D */
	TM_DMA2DGRAPHIC_CopyBuffer(
		(void *)(LCD.FrameStart),
		(void *)(LCD.FrameStart + LCD.FrameOffset),
		LCD.Height,
		LCD.Width,
		0,
		0
	);
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_Layer1To2(void) {
	/* Copy buffer using DMA2D */
	TM_DMA2DGRAPHIC_CopyBuffer(
		(void *)(LCD.FrameStart),
		(void *)(LCD.FrameStart + LCD.FrameOffset),
		LCD.Width,
		LCD.Height,
		0,
		0
	);
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color) {
	/* Draw line with DMA2D */
	TM_DMA2DGRAPHIC_DrawLine(x0, y0, x1, y1, color);
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}


TM_LCD_Result_t TM_LCD_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t Width, uint16_t Height, uint32_t color) {
	/* Use DMA2D for drawing */
	TM_DMA2DGRAPHIC_DrawRectangle(x0, y0, Width, Height, color);
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_DrawRoundedRectangle(uint16_t x0, uint16_t y0, uint16_t Width, uint16_t Height, uint16_t r, uint32_t color) {
	/* Check input parameters */
	if ((x0 + Width) > LCD.Width || (y0 + Height) > LCD.Height) {
		/* Return error */
		return TM_LCD_Result_Error;
	}
	
	/* Draw rectangle with DMA2D */
	TM_DMA2DGRAPHIC_DrawRoundedRectangle(x0, y0, Width, Height, r, color);

	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_DrawFilledRoundedRectangle(uint16_t x0, uint16_t y0, uint16_t Width, uint16_t Height, uint16_t r, uint32_t color) {
	/* Check input parameters */
	TM_DMA2DGRAPHIC_DrawFilledRoundedRectangle(x0, y0, Width, Height, r, color);
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t Width, uint16_t Height, uint32_t color) {
	/* Draw with DMA2D */
	TM_DMA2DGRAPHIC_DrawFilledRectangle(x0, y0, Width, Height, color);
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color) {
	/* Use DMA2D */
	TM_DMA2DGRAPHIC_DrawCircle(x0, y0, r, color);
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}

TM_LCD_Result_t TM_LCD_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color) {
	/* Use DMA2D */
	TM_DMA2DGRAPHIC_DrawFilledCircle(x0, y0, r, color);
	
	/* Return OK */
	return TM_LCD_Result_Ok;
}


uint16_t TM_LCD_GetWidth(void) {
	return LCD.Width;
}

uint16_t TM_LCD_GetHeight(void) {
	return LCD.Height;
}

uint32_t TM_LCD_GetFrameBuffer(void) {
	return LCD.FrameStart;
}

/* Private functions */
static void TM_LCD_INT_InitLTDC(void) {
	LTDC_InitTypeDef LTDC_InitStruct;
	
	/* Enable LTDC and DMA2D clocks */
	RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2DEN;
	
	/* Disable LTDC */
	LTDC->GCR &= ~LTDC_GCR_LTDCEN;
	
	/* Polarity configuration */
	/* Initialize the horizontal synchronization polarity as active low */
	LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;     
	/* Initialize the vertical synchronization polarity as active low */  
	LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;     
	/* Initialize the data enable polarity as active low */
	LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;     
	/* Initialize the pixel clock polarity as input pixel clock */ 
	LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

	/* Configure R,G,B component values for LCD background color */                   
	LTDC_InitStruct.LTDC_BackgroundRedValue = 0;            
	LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;          
	LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;
    
    /* Configure PLLSAI prescalers for LCD */
    /* Enable Pixel Clock */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/2 = 96 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 96/4 = 24 Mhz */
    RCC_PLLSAIConfig(192, 7, 2);
    RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div4);
    
    /* Enable PLLSAI Clock */
    RCC_PLLSAICmd(ENABLE);
	
    /* Wait for PLLSAI activation */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET);
    
    /* Timing configuration */     
    LTDC_InitStruct.LTDC_HorizontalSync = 29;
    LTDC_InitStruct.LTDC_VerticalSync = 2;
    LTDC_InitStruct.LTDC_AccumulatedHBP = 143;
    LTDC_InitStruct.LTDC_AccumulatedVBP = 34;  
    LTDC_InitStruct.LTDC_AccumulatedActiveW = 783;
    LTDC_InitStruct.LTDC_AccumulatedActiveH = 514;
    LTDC_InitStruct.LTDC_TotalWidth = 799; 
    LTDC_InitStruct.LTDC_TotalHeigh = 524;
    
	/* Init LTDC peripheral */
    LTDC_Init(&LTDC_InitStruct);
}

static void TM_LCD_INT_InitLayers(void) {
	LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;
	
    /* Windowing configuration */
    /* Horizontal start = horizontal synchronization + Horizontal back porch = 43 
    Vertical start   = vertical synchronization + vertical back porch     = 12
    Horizontal stop = Horizontal start + LCD_PIXEL_WIDTH -1 
    Vertical stop   = Vertical start + LCD_PIXEL_HEIGHT -1        
    */      
    LTDC_Layer_InitStruct.LTDC_HorizontalStart = 144;
    LTDC_Layer_InitStruct.LTDC_HorizontalStop = (LCD.Width + 144 - 1); 
    LTDC_Layer_InitStruct.LTDC_VerticalStart = 35;
    LTDC_Layer_InitStruct.LTDC_VerticalStop = (LCD.Height + 35 - 1);
    
    /* Pixel Format configuration*/
    LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
    /* Alpha constant (255 totally opaque) */
    LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255; 
    /* Default Color configuration (configure A,R,G,B component values) */          
    LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;        
    LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;       
    LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;         
    LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;
    /* Configure blending factors */       
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;
    
    /* the length of one line of pixels in bytes + 3 then :
    Line Lenth = Active high width x number of bytes per pixel + 3 
    Active high width         = LCD_PIXEL_WIDTH 
    number of bytes per pixel = 2    (pixel_format : RGB565) 
    */
    LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD.Width * 2) + 3);
    /* the pitch is the increment from the start of one line of pixels to the 
    start of the next line in bytes, then :
    Pitch = Active high width x number of bytes per pixel     
    */ 
    LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD.Width * 2);
    
    /* Configure the number of lines */  
    LTDC_Layer_InitStruct.LTDC_CFBLineNumber = LCD.Height;
    
    /* Start Address configuration : the LCD Frame buffer is defined on SDRAM */    
    LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD.FrameStart;
    
    LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);
    
    /* Configure Layer2 */
    /* Start Address configuration : the LCD Frame buffer is defined on SDRAM w/ Offset */     
    LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD.FrameStart + LCD.FrameOffset;
    
    /* Configure blending factors */   
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;    
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;  
    
    LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);
	
	LTDC_ReloadConfig(LTDC_IMReload);

	/* Enable foreground & background Layers */
	LTDC_LayerCmd(LTDC_Layer1, ENABLE);
	LTDC_LayerCmd(LTDC_Layer2, ENABLE);
	LTDC_ReloadConfig(LTDC_IMReload);
	
	/* Enable dithering */
	LTDC_DitherCmd(ENABLE);
	
	/* Set opacity */
	LTDC_LayerAlpha(LTDC_Layer1, 255);
	LTDC_LayerAlpha(LTDC_Layer2, 0);
	
	/* Immediate reload */
	LTDC_ReloadConfig(LTDC_IMReload);
}

static void TM_LCD_INT_InitLCD(void) {

}

static void TM_LCD_INT_InitPins(void) {
	/* LCD pins */
	TM_GPIO_InitAlternate(GPIOI, 0xF000, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Fast, 0x0E);
	TM_GPIO_InitAlternate(GPIOJ, 0xFFFF, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Fast, 0x0E);
	TM_GPIO_InitAlternate(GPIOK, 0x00FF, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Fast, 0x0E);
}
