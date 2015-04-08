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

void TM_LCD_InitLTDC(void) {
	LTDC_InitTypeDef LTDC_InitStruct;
	
	/* Enable the LTDC Clock */
	RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;

	/* Enable the DMA2D Clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2DEN;
	  
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
	while (RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET) {
	
	}
	
    /* Timing configuration */  
    /* Configure horizontal synchronization width */
    LTDC_InitStruct.LTDC_HorizontalSync = 29;
    /* Configure vertical synchronization height */
    LTDC_InitStruct.LTDC_VerticalSync = 2;
    /* Configure accumulated horizontal back porch */
    LTDC_InitStruct.LTDC_AccumulatedHBP = 143;
    /* Configure accumulated vertical back porch */
    LTDC_InitStruct.LTDC_AccumulatedVBP = 34;
    /* Configure accumulated active width */
    LTDC_InitStruct.LTDC_AccumulatedActiveW = 783;
    /* Configure accumulated active height */
    LTDC_InitStruct.LTDC_AccumulatedActiveH = 514;
    /* Configure total width */
    LTDC_InitStruct.LTDC_TotalWidth = 799;
    /* Configure total height */
    LTDC_InitStruct.LTDC_TotalHeigh = 524;

	/* Init LTDC */
	LTDC_Init(&LTDC_InitStruct);
}

void TM_LCD_InitLayers(void) {
	LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;

    /* Windowing configuration */
    /* Horizontal start = horizontal synchronization + Horizontal back porch = 43 
    Vertical start   = vertical synchronization + vertical back porch     = 12
    Horizontal stop = Horizontal start + LCD_PIXEL_WIDTH -1 
    Vertical stop   = Vertical start + LCD_PIXEL_HEIGHT -1        
    */      
    LTDC_Layer_InitStruct.LTDC_HorizontalStart = 144;
    LTDC_Layer_InitStruct.LTDC_HorizontalStop = (LCD_PIXEL_WIDTH + 144 - 1); 
    LTDC_Layer_InitStruct.LTDC_VerticalStart = 35;
    LTDC_Layer_InitStruct.LTDC_VerticalStop = (LCD_PIXEL_HEIGHT + 35 - 1);
    
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
    LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_PIXEL_WIDTH * 2) + 3);
    /* the pitch is the increment from the start of one line of pixels to the 
    start of the next line in bytes, then :
    Pitch = Active high width x number of bytes per pixel     
    */ 
    LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_PIXEL_WIDTH * 2);
    
    /* Configure the number of lines */  
    LTDC_Layer_InitStruct.LTDC_CFBLineNumber = LCD_PIXEL_HEIGHT;
    
    /* Start Address configuration : the LCD Frame buffer is defined on SDRAM */    
    LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER;
    
	/* Init layer 1 */
    LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);
    
    /* Configure Layer2 */
    /* Start Address configuration : the LCD Frame buffer is defined on SDRAM w/ Offset */     
    LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER + LCD_FRAME_OFFSET;
    
    /* Configure blending factors */   
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;    
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;  
    
	/* Init layer 2 */
    LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);

	/* Reload immediate */
	LTDC_ReloadConfig(LTDC_IMReload);
  
	/* Enable foreground & background Layers */
	LTDC_LayerCmd(LTDC_Layer1, ENABLE);
	LTDC_LayerCmd(LTDC_Layer2, ENABLE);	
	
	/* Reload immediate */
	LTDC_ReloadConfig(LTDC_IMReload);
	
	/* Set opacity */
	LTDC_LayerAlpha(LTDC_Layer1, 255);
	LTDC_LayerAlpha(LTDC_Layer2, 0);
	
	/* Enable dither */
	LTDC_DitherCmd(ENABLE);
	
	/* Display On */
	LTDC_Cmd(ENABLE);
	
	/* Immediate reload */
	//LTDC_ReloadConfig(LTDC_IMReload);
}

void TM_LCD_InitPins(void) {
	TM_GPIO_InitAlternate(GPIOI, 0xF000, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_LTDC);
	TM_GPIO_InitAlternate(GPIOJ, 0xFFFF, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_LTDC);
	TM_GPIO_InitAlternate(GPIOK, 0x00FF, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_High, GPIO_AF_LTDC);
}

void TM_LCD_DisplayOn(void) {
	/* Enable LTDC peripheral */
	LTDC_Cmd(ENABLE);
}

void TM_LCD_DisplayOff(void) {
	/* Disable LTDC peripheral */
	LTDC_Cmd(DISABLE);
}
 
void TM_LCD_Init(void) {
	/* Initialize pins used */
	TM_LCD_InitPins();
	/* Init SDRAM */
	TM_SDRAM_Init();
	/* Initialize LTDC */
	TM_LCD_InitLTDC();
	/* Initialize LTDC layers */
	TM_LCD_InitLayers();
	/* Enable LCD */
	TM_LCD_DisplayOn();
}

