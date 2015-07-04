/*********************************************************************
*          Portions COPYRIGHT 2013 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.22 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIDRV_stm32f429i_discovery.c
Purpose     : Driver for STM32429I-Discovery board RevB

The part between 'DISPLAY CONFIGURATION START' and 'DISPLAY CONFIGURA-
TION END' can be used to configure the following for each layer:

- Color mode
- Layer size
- Layer orientation

Further the background color used on positions without a valid layer
can be set here.

---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "GUI.h"
#include "GUI_Private.h"
#include "GUIDRV_Lin.h"
#include "global_includes.h"

/* External variable, declared in tm_stm32f4_emwin.c file */
extern uint32_t EMWIN_LCD_DRIVER_CB_CALLED;

/*********************************************************************
*
*       Display configuration (to be modified)
*
**********************************************************************
*/
/*********************************************************************
*
*       Common
*/

#undef  LCD_SWAP_XY
#undef  LCD_MIRROR_Y

#define LCD_SWAP_XY  0 
#define LCD_MIRROR_Y 1

//
// Physical display size
//
#define XSIZE_PHYS 240
#define YSIZE_PHYS 320

//
// Buffers / VScreens
//
#define NUM_BUFFERS  1 // Number of multiple buffers to be used
#define NUM_VSCREENS 1 // Number of virtual screens to be used

//
// BkColor shown if no layer is active
//
#define BK_COLOR GUI_DARKBLUE

//
// Redefine number of layers for this configuration file. Must be equal or less than in GUIConf.h!
//
#undef  GUI_NUM_LAYERS
#define GUI_NUM_LAYERS 1

/*********************************************************************
*
*       Color mode definitions
*/
#define _CM_ARGB8888 1
#define _CM_RGB888   2
#define _CM_RGB565   3
#define _CM_ARGB1555 4
#define _CM_ARGB4444 5
#define _CM_L8       6
#define _CM_AL44     7
#define _CM_AL88     8

/*********************************************************************
*
*       Layer 0
*/
//
// Color mode layer 0
//
#define COLOR_MODE_0 _CM_RGB565
//
// Layer size
//
#define XSIZE_0 240
#define YSIZE_0 320

/*********************************************************************
*
*       Layer 1
*/
#define COLOR_MODE_1 _CM_RGB565
//
// Layer size
//
#define XSIZE_1 240
#define YSIZE_1 320

/*********************************************************************
*
*       Automatic selection of driver and color conversion
*/
#if   (COLOR_MODE_0 == _CM_ARGB8888)
  #define COLOR_CONVERSION_0 GUICC_M8888I
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_32
#elif (COLOR_MODE_0 == _CM_RGB888)
  #define COLOR_CONVERSION_0 GUICC_M888
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_24
#elif (COLOR_MODE_0 == _CM_RGB565)
  #define COLOR_CONVERSION_0 GUICC_M565
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_16
#elif (COLOR_MODE_0 == _CM_ARGB1555)
  #define COLOR_CONVERSION_0 GUICC_M1555I
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_16
#elif (COLOR_MODE_0 == _CM_ARGB4444)
  #define COLOR_CONVERSION_0 GUICC_M4444I
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_16
#elif (COLOR_MODE_0 == _CM_L8)
  #define COLOR_CONVERSION_0 GUICC_8666
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_8
#elif (COLOR_MODE_0 == _CM_AL44)
  #define COLOR_CONVERSION_0 GUICC_1616I
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_8
#elif (COLOR_MODE_0 == _CM_AL88)
  #define COLOR_CONVERSION_0 GUICC_88666I
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_16
#else
  #error Illegal color mode 0!
#endif

#if (GUI_NUM_LAYERS > 1)

#if   (COLOR_MODE_1 == _CM_ARGB8888)
  #define COLOR_CONVERSION_1 GUICC_M8888I
  #define DISPLAY_DRIVER_1   GUIDRV_LIN_32
#elif (COLOR_MODE_1 == _CM_RGB888)
  #define COLOR_CONVERSION_1 GUICC_M888
  #define DISPLAY_DRIVER_1   GUIDRV_LIN_24
#elif (COLOR_MODE_1 == _CM_RGB565)
  #define COLOR_CONVERSION_1 GUICC_M565
  #define DISPLAY_DRIVER_1   GUIDRV_LIN_16
#elif (COLOR_MODE_1 == _CM_ARGB1555)
  #define COLOR_CONVERSION_1 GUICC_M1555I
  #define DISPLAY_DRIVER_1   GUIDRV_LIN_16
#elif (COLOR_MODE_1 == _CM_ARGB4444)
  #define COLOR_CONVERSION_1 GUICC_M4444I
  #define DISPLAY_DRIVER_1   GUIDRV_LIN_16
#elif (COLOR_MODE_1 == _CM_L8)
  #define COLOR_CONVERSION_1 GUICC_8666
  #define DISPLAY_DRIVER_1   GUIDRV_LIN_8
#elif (COLOR_MODE_1 == _CM_AL44)
  #define COLOR_CONVERSION_1 GUICC_1616I
  #define DISPLAY_DRIVER_1   GUIDRV_LIN_8
#elif (COLOR_MODE_1 == _CM_AL88)
  #define COLOR_CONVERSION_1 GUICC_88666I
  #define DISPLAY_DRIVER_1   GUIDRV_LIN_16
#else
  #error Illegal color mode 1!
#endif

#else

/*********************************************************************
*
*       Use complete display automatically in case of only one layer
*/
#undef XSIZE_0
#undef YSIZE_0
#define XSIZE_0 XSIZE_PHYS
#define YSIZE_0 YSIZE_PHYS

#endif

/*********************************************************************
*
*       H/V front/backporch and synchronization width/height
*/
#define HBP  29
#define VBP   3

#define HSW  9
#define VSW  1

#define HFP  10
#define VFP  4

/*********************************************************************
*
*       Configuration checking
*/
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   NUM_VSCREENS
  #define NUM_VSCREENS 1
#else
  #if (NUM_VSCREENS <= 0)
    #error At least one screeen needs to be defined!
  #endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
  #error Virtual screens and multiple buffers are not allowed!
#endif

/*********************************************************************
*
*       Redirect bulk conversion to DMA2D routines
*/
#define DEFINE_DMA2D_COLORCONVERSION(PFIX, PIXELFORMAT)                                                        \
static void _Color2IndexBulk_##PFIX##_DMA2D(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Color2IndexBulk(pColor, pIndex, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}                                                                                                              \
static void _Index2ColorBulk_##PFIX##_DMA2D(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Index2ColorBulk(pColor, pIndex, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static LTDC_Layer_TypeDef       * _apLayer[]        = { LTDC_Layer1, LTDC_Layer2 };
static const U32                  _aAddr[]          = { 0xD0000000, 0xD0000000 + XSIZE_PHYS * YSIZE_PHYS * sizeof(U32) * NUM_VSCREENS * NUM_BUFFERS };
static int                        _aPendingBuffer[GUI_NUM_LAYERS];
static int                        _aBufferIndex[GUI_NUM_LAYERS];
static int                        _axSize[GUI_NUM_LAYERS];
static int                        _aySize[GUI_NUM_LAYERS];
static int                        _aBytesPerPixels[GUI_NUM_LAYERS];


//
// Prototypes of DMA2D color conversion routines
//
static void _DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat);
static void _DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat);

//
// Color conversion routines using DMA2D
//
DEFINE_DMA2D_COLORCONVERSION(M8888I, LTDC_Pixelformat_ARGB8888)
DEFINE_DMA2D_COLORCONVERSION(M888,   LTDC_Pixelformat_ARGB8888) // Internal pixel format of emWin is 32 bit, because of that ARGB8888
DEFINE_DMA2D_COLORCONVERSION(M565,   LTDC_Pixelformat_RGB565)
DEFINE_DMA2D_COLORCONVERSION(M1555I, LTDC_Pixelformat_ARGB1555)
DEFINE_DMA2D_COLORCONVERSION(M4444I, LTDC_Pixelformat_ARGB4444)

static U32 _aBuffer_DMA2D[XSIZE_PHYS * sizeof(U32)];
static U32 _aBuffer_FG   [XSIZE_PHYS * sizeof(U32)];
static U32 _aBuffer_BG   [XSIZE_PHYS * sizeof(U32)];
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static const LCD_API_COLOR_CONV * _apColorConvAPI[] = {
  COLOR_CONVERSION_0,
#if GUI_NUM_LAYERS > 1
  COLOR_CONVERSION_1,
#endif
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetPixelformat
*/
static U32 _GetPixelformat(int LayerIndex) {
  const LCD_API_COLOR_CONV * pColorConvAPI;	
	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  if (LayerIndex >= GUI_COUNTOF(_apColorConvAPI)) {
    return 0;
  }
  pColorConvAPI = _apColorConvAPI[LayerIndex];
  if        (pColorConvAPI == GUICC_M8888I) {
    return LTDC_Pixelformat_ARGB8888;
  } else if (pColorConvAPI == GUICC_M888) {
    return LTDC_Pixelformat_RGB888;
  } else if (pColorConvAPI == GUICC_M565) {
    return LTDC_Pixelformat_RGB565;
  } else if (pColorConvAPI == GUICC_M1555I) {
    return LTDC_Pixelformat_ARGB1555;
  } else if (pColorConvAPI == GUICC_M4444I) {
    return LTDC_Pixelformat_ARGB4444;
  } else if (pColorConvAPI == GUICC_8666) {
    return LTDC_Pixelformat_L8;
  } else if (pColorConvAPI == GUICC_1616I) {
    return LTDC_Pixelformat_AL44;
  } else if (pColorConvAPI == GUICC_88666I) {
    return LTDC_Pixelformat_AL88;
  }
  while (1); // Error
}

/*********************************************************************
*
*       _GetBytesPerLine
*/
static int _GetBytesPerLine(int LayerIndex, int xSize) {
  int BitsPerPixel, BytesPerLine;	
	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  BitsPerPixel  = LCD_GetBitsPerPixelEx(LayerIndex);
  BytesPerLine = (BitsPerPixel * xSize + 7) / 8;
  return BytesPerLine;
}

/*********************************************************************
*
*       _DMA_LoadLUT
*/
static void _DMA_LoadLUT(LCD_COLOR * pColor, U32 NumItems) {	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;
	
  DMA2D->FGCMAR  = (U32)pColor;                     // Foreground CLUT Memory Address Register
  //
  // Foreground PFC Control Register
  //
  DMA2D->FGPFCCR  = LTDC_Pixelformat_RGB888         // Pixel format
                  | ((NumItems - 1) & 0xFF) << 8;   // Number of items to load
  DMA2D->FGPFCCR |= (1 << 5);                       // Start loading
  //
  // Waiting not required here...
  //
}

/*********************************************************************
*
*       _InvertAlpha_SwapRB
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   swaps red and blue and inverts alpha that it is compatible to emWin
*   and vice versa.
*/
static void _InvertAlpha_SwapRB(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, U32 NumItems) {
  U32 Color;
		
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;
	
  do {
    Color = *pColorSrc++;
    *pColorDst++ = ((Color & 0x000000FF) << 16)         // Swap red <-> blue
                 |  (Color & 0x0000FF00)                // Green
                 | ((Color & 0x00FF0000) >> 16)         // Swap red <-> blue
                 | ((Color & 0xFF000000) ^ 0xFF000000); // Invert alpha
  } while (--NumItems);
}

/*********************************************************************
*
*       _InvertAlpha
*
* Purpose:
*   Color format of DMA2D is different to emWin color format. This routine
*   inverts alpha that it is compatible to emWin and vice versa.
*   Changes are done in the destination memory location.
*/
static void _InvertAlpha(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, U32 NumItems) {
  U32 Color;
		
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  do {
    Color = *pColorSrc++;
    *pColorDst++ = Color ^ 0xFF000000; // Invert alpha
  } while (--NumItems);
}

/*********************************************************************
*
*       _DMA_AlphaBlendingBulk
*/
static void _DMA_AlphaBlendingBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems) {	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000UL | (1 << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pColorFG;                   // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)pColorBG;                   // Background Memory Address Register
  DMA2D->OMAR    = (U32)pColorDst;                  // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = 0;                               // Foreground Offset Register
  DMA2D->BGOR    = 0;                               // Background Offset Register
  DMA2D->OOR     = 0;                               // Output Offset Register
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_Pixelformat_ARGB8888;       // Foreground PFC Control Register (Defines the FG pixel format)
  DMA2D->BGPFCCR = LTDC_Pixelformat_ARGB8888;       // Background PFC Control Register (Defines the BG pixel format)
  DMA2D->OPFCCR  = LTDC_Pixelformat_ARGB8888;       // Output     PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;       // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  
  DMA2D->CR     |= 1;  
    
  while (DMA2D->CR & DMA2D_CR_START) {
    __WFI();                                        // Sleep until next interrupt
  }
}

/*********************************************************************
*
*       _DMA_MixColors
*
* Purpose:
*   Function for mixing up 2 colors with the given intensity.
*   If the background color is completely transparent the
*   foreground color should be used unchanged.
*/
static LCD_COLOR _DMA_MixColors(LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens) {
  U32 ColorFG, ColorBG, ColorDst;	
	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  if ((BkColor & 0xFF000000) == 0xFF000000) {
    return Color;
  }
  ColorFG = Color   ^ 0xFF000000;
  ColorBG = BkColor ^ 0xFF000000;
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000UL | (1 << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)&ColorFG;                   // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)&ColorBG;                   // Background Memory Address Register
  DMA2D->OMAR    = (U32)&ColorDst;                  // Output Memory Address Register (Destination address)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (1UL << 16)
                 | ((U32)Intens << 24);
  DMA2D->BGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (0UL << 16)
                 | ((U32)(255 - Intens) << 24);
  DMA2D->OPFCCR  = LTDC_Pixelformat_ARGB8888;
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(1 << 16) | 1;              // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //

  //_DMA_ExecOperation();
  DMA2D->CR     |= 1;                               // Control Register (Start operation)
  //
  // Wait until transfer is done
  //
  while (DMA2D->CR & DMA2D_CR_START) {
    __WFI();                                        // Sleep until next interrupt
  }

  return ColorDst ^ 0xFF000000;
}

/*********************************************************************
*
*       _DMA_ConvertColor
*/
static void _DMA_ConvertColor(void * pSrc, void * pDst,  U32 PixelFormatSrc, U32 PixelFormatDst, U32 NumItems) {	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00010000UL | (1 << 9);         // Control Register (Memory to memory with pixel format conversion and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = 0;                               // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = 0;                               // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = PixelFormatSrc;                  // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->OPFCCR  = PixelFormatDst;                  // Output PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;       // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  DMA2D->CR     |= 1;  
    
  while (DMA2D->CR & DMA2D_CR_START) {
    //__WFI();                                        // Sleep until next interrupt
  }
}

/*********************************************************************
*
*       _LCD_GetpPalConvTable
*
* Purpose:
*   The emWin function LCD_GetpPalConvTable() normally translates the given colors into
*   index values for the display controller. In case of index based bitmaps without
*   transparent pixels we load the palette only to the DMA2D LUT registers to be
*   translated (converted) during the process of drawing via DMA2D.
*/
static LCD_PIXELINDEX * _LCD_GetpPalConvTable(const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, const GUI_BITMAP GUI_UNI_PTR * pBitmap, int LayerIndex) {
  void (* pFunc)(void);
  int DoDefault = 0;	
	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  //
  // Check if we have a non transparent device independent bitmap
  //
  if (pBitmap->BitsPerPixel == 8) {
    pFunc = LCD_GetDevFunc(LayerIndex, LCD_DEVFUNC_DRAWBMP_8BPP);
    if (pFunc) {
      if (pBitmap->pPal) {
        if (pBitmap->pPal->HasTrans) {
          DoDefault = 1;
        }
      } else {
        DoDefault = 1;
      }
    } else {
      DoDefault = 1;
    }
  } else {
    DoDefault = 1;
  }
  //
  // Default palette management for other cases
  //
  if (DoDefault) {
    //
    // Return a pointer to the index values to be used by the controller
    //
    return LCD_GetpPalConvTable(pLogPal);
  }
  //
  // Convert palette colors from ARGB to ABGR
  //
  _InvertAlpha_SwapRB((U32 *)pLogPal->pPalEntries, _aBuffer_DMA2D, pLogPal->NumEntries);
  //
  // Load LUT using DMA2D
  //
  _DMA_LoadLUT(_aBuffer_DMA2D, pLogPal->NumEntries);
  //
  // Return something not NULL
  //
  return _aBuffer_DMA2D;
}


/*********************************************************************
*
*       _DMA_MixColorsBulk
*/
static void _DMA_MixColorsBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U8 Intens, U32 NumItems) {	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00020000UL | (1 << 9);         // Control Register (Memory to memory with blending of FG and BG and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pColorFG;                   // Foreground Memory Address Register
  DMA2D->BGMAR   = (U32)pColorBG;                   // Background Memory Address Register
  DMA2D->OMAR    = (U32)pColorDst;                  // Output Memory Address Register (Destination address)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (1UL << 16)
                 | ((U32)Intens << 24);
  DMA2D->BGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (0UL << 16)
                 | ((U32)(255 - Intens) << 24);
  DMA2D->OPFCCR  = LTDC_Pixelformat_ARGB8888;
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;              // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  DMA2D->CR     |= 1;  
    
  while (DMA2D->CR & DMA2D_CR_START) {
    //__WFI();                                        // Sleep until next interrupt
  }
}
/*********************************************************************
*
*       _DMA_AlphaBlending
*/
static void _DMA_AlphaBlending(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems) {	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;
  //
  // Invert alpha values
  //
  _InvertAlpha(pColorFG, _aBuffer_FG, NumItems);
  _InvertAlpha(pColorBG, _aBuffer_BG, NumItems);
  //
  // Use DMA2D for mixing
  //
  _DMA_AlphaBlendingBulk(_aBuffer_FG, _aBuffer_BG, _aBuffer_DMA2D, NumItems);
  //
  // Invert alpha values
  //
  _InvertAlpha(_aBuffer_DMA2D, pColorDst, NumItems);
}

/*********************************************************************
*
*       _DMA_Index2ColorBulk
*
* Purpose:
*   This routine is used by the emWin color conversion routines to use DMA2D for
*   color conversion. It converts the given index values to 32 bit colors.
*   Because emWin uses ABGR internally and 0x00 and 0xFF for opaque and fully
*   transparent the color array needs to be converted after DMA2D has been used.
*/
static void _DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat) {	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(pIndex, _aBuffer_DMA2D, PixelFormat, LTDC_Pixelformat_ARGB8888, NumItems);
  //
  // Convert colors from ARGB to ABGR and invert alpha values
  //
  _InvertAlpha_SwapRB(_aBuffer_DMA2D, pColor, NumItems);
}

/*********************************************************************
*
*       _DMA_Color2IndexBulk
*
* Purpose:
*   This routine is used by the emWin color conversion routines to use DMA2D for
*   color conversion. It converts the given 32 bit color array to index values.
*   Because emWin uses ABGR internally and 0x00 and 0xFF for opaque and fully
*   transparent the given color array needs to be converted before DMA2D can be used.
*/
static void _DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat) {	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;
  //
  // Convert colors from ABGR to ARGB and invert alpha values
  //
  _InvertAlpha_SwapRB(pColor, _aBuffer_DMA2D, NumItems);
  //
  // Use DMA2D for the conversion
  //
  _DMA_ConvertColor(_aBuffer_DMA2D, pIndex, LTDC_Pixelformat_ARGB8888, PixelFormat, NumItems);
}

/*********************************************************************
*
*       _LCD_MixColorsBulk
*/
static void _LCD_MixColorsBulk(U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens) {
  int y;
		
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  GUI_USE_PARA(OffFG);
  GUI_USE_PARA(OffDest);
  for (y = 0; y < ySize; y++) {
    //
    // Invert alpha values
    //
    _InvertAlpha(pFG, _aBuffer_FG, xSize);
    _InvertAlpha(pBG, _aBuffer_BG, xSize);
    //
    //
    //
    _DMA_MixColorsBulk(_aBuffer_FG, _aBuffer_BG, _aBuffer_DMA2D, Intens, xSize);
    //
    //
    //
    _InvertAlpha(_aBuffer_DMA2D, pDst, xSize);
    pFG  += xSize + OffFG;
    pBG  += xSize + OffBG;
    pDst += xSize + OffDest;
  }
}


/*********************************************************************
*
*       _LTDC_LayerEnableColorKeying
*/
static void _LTDC_LayerEnableColorKeying(LTDC_Layer_TypeDef * LTDC_Layerx, int NewState) {	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;
	
  if (NewState != DISABLE) {
    LTDC_Layerx->CR |= (U32)LTDC_LxCR_COLKEN;
  } else {
    LTDC_Layerx->CR &= ~(U32)LTDC_LxCR_COLKEN;
  }
  LTDC->SRCR = LTDC_SRCR_VBR; // Reload on next blanking period
}

/*********************************************************************
*
*       _LTDC_LayerEnableLUT
*/
static void _LTDC_LayerEnableLUT(LTDC_Layer_TypeDef * LTDC_Layerx, int NewState) {
  if (NewState != DISABLE) {
    LTDC_Layerx->CR |= (U32)LTDC_LxCR_CLUTEN;
  } else {
    LTDC_Layerx->CR &= ~(U32)LTDC_LxCR_CLUTEN;
  }
  LTDC->SRCR = LTDC_SRCR_VBR; // Reload on next blanking period
}

/*********************************************************************
*
*       _LTDC_SetLayerPos
*/
static void _LTDC_SetLayerPos(int LayerIndex, int xPos, int yPos) {
  int xSize, ySize;
  U32 HorizontalStart, HorizontalStop, VerticalStart, VerticalStop;	
	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;
	

  xSize = LCD_GetXSizeEx(LayerIndex);
  ySize = LCD_GetYSizeEx(LayerIndex);
  HorizontalStart = xPos + HBP + 1;
  HorizontalStop  = xPos + HBP + xSize;
  VerticalStart   = yPos + VBP + 1;
  VerticalStop    = yPos + VBP + ySize;
  //
  // Horizontal start and stop position
  //
  _apLayer[LayerIndex]->WHPCR &= ~(LTDC_LxWHPCR_WHSTPOS | LTDC_LxWHPCR_WHSPPOS);
  _apLayer[LayerIndex]->WHPCR = (HorizontalStart | (HorizontalStop << 16));
  //
  // Vertical start and stop position
  //
  _apLayer[LayerIndex]->WVPCR &= ~(LTDC_LxWVPCR_WVSTPOS | LTDC_LxWVPCR_WVSPPOS);
  _apLayer[LayerIndex]->WVPCR  = (VerticalStart | (VerticalStop << 16));
  //
  // Reload configuration
  //
  LTDC_ReloadConfig(LTDC_SRCR_VBR); // Reload on next blanking period
}

/*********************************************************************
*
*       _LTDC_SetLayerAlpha
*/
static void _LTDC_SetLayerAlpha(int LayerIndex, int Alpha) {	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;
  //
  // Set constant alpha value
  //
  _apLayer[LayerIndex]->CACR &= ~(LTDC_LxCACR_CONSTA);
  _apLayer[LayerIndex]->CACR  = 255 - Alpha;
  //
  // Reload configuration
  //
  LTDC_ReloadConfig(LTDC_SRCR_IMR/*LTDC_SRCR_VBR*/); // Reload on next blanking period/**/
}

/*********************************************************************
*
*       _LTDC_SetLUTEntry
*/
static void _LTDC_SetLUTEntry(int LayerIndex, U32 Color, int Pos) {
  U32 r, g, b, a;
		
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  r = ( Color        & 0xff) << 16;
  g = ((Color >>  8) & 0xff) <<  8;
  b = ((Color >> 16) & 0xff);
  a = Pos << 24;
  _apLayer[LayerIndex]->CLUTWR &= ~(LTDC_LxCLUTWR_BLUE | LTDC_LxCLUTWR_GREEN | LTDC_LxCLUTWR_RED | LTDC_LxCLUTWR_CLUTADD);
  _apLayer[LayerIndex]->CLUTWR  = r | g | b | a;
  //
  // Reload configuration
  //
  LTDC_ReloadConfig(LTDC_SRCR_IMR);
}

/*********************************************************************
*
*       _DMA_Copy
*/
static void _DMA_Copy(int LayerIndex, void * pSrc, void * pDst, int xSize, int ySize, int OffLineSrc, int OffLineDst) {
  U32 PixelFormat;
		
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  PixelFormat = _GetPixelformat(LayerIndex);
  DMA2D->CR      = 0x00000000UL | (1 << 9);         // Control Register (Memory to memory and TCIE)
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  DMA2D->FGOR    = OffLineSrc;                      // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = OffLineDst;                      // Output Offset Register (Destination line offset)
  DMA2D->FGPFCCR = PixelFormat;                     // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Number of Line Register (Size configuration of area to be transfered)
  DMA2D->CR     |= 1;                               // Start operation
  //
  // Wait until transfer is done
  //
  while (DMA2D->CR & DMA2D_CR_START) {
    //__WFI();                                        // Sleep until next interrupt
  }
}

/*********************************************************************
*
*       _DMA_Fill
*/
static void _DMA_Fill(int LayerIndex, void * pDst, int xSize, int ySize, int OffLine, U32 ColorIndex) {
  U32 PixelFormat;
		
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  PixelFormat = _GetPixelformat(LayerIndex);
  DMA2D->CR      = 0x00030000UL | (1 << 9);         // Register to memory and TCIE
  DMA2D->OCOLR   = ColorIndex;                      // Color to be used
  DMA2D->OMAR    = (U32)pDst;                       // Destination address
  DMA2D->OOR     = OffLine;                         // Destination line offset
  DMA2D->OPFCCR  = PixelFormat;                     // Defines the number of pixels to be transfered
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize; // Size configuration of area to be transfered
  DMA2D->CR     |= 1;                               // Start operation
  //
  // Wait until transfer is done
  //
  while (DMA2D->CR & DMA2D_CR_START) {
    __WFI();                                        // Sleep until next interrupt
  }
}

/*********************************************************************
*
*       _LCD_DisplayOn
*/
static void _LCD_DisplayOn(void) {
  //LCD_DisplayOn();
  LTDC_Cmd(ENABLE); /* display ON */
}

/*********************************************************************
*
*       _LCD_InitController
*/
static void _LCD_InitController(int LayerIndex) {
  LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct = {0};
  static int xSize, ySize, BytesPerLine, BitsPerPixel, i;
  static U32 Pixelformat, Color;
  static int Done;
  	
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  if (LayerIndex >= GUI_COUNTOF(_apLayer)) {
    return;
  }
  if (Done == 0) {
    Done = 1;

    //LCD_Init();
    //
    // Enable line interrupt
    //
    LTDC_ITConfig(LTDC_IER_LIE, ENABLE);
    NVIC_SetPriority(LTDC_IRQn, 0);
    NVIC_EnableIRQ(LTDC_IRQn);
    //
    // Enable DMA2D transfer complete interrupt
    //
    DMA2D_ITConfig(DMA2D_CR_TCIE, ENABLE);
    NVIC_SetPriority(DMA2D_IRQn, 0);
    NVIC_EnableIRQ(DMA2D_IRQn);
    //
    // Clear transfer complete interrupt flag
    //
    DMA2D->IFCR = (U32)DMA2D_IFSR_CTCIF;
  }
  //
  // Layer configuration
  //
  xSize = LCD_GetXSizeEx(LayerIndex);
  ySize = LCD_GetYSizeEx(LayerIndex);

  LTDC_Layer_InitStruct.LTDC_HorizontalStart = HBP + 1;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop  = (xSize + HBP);
  LTDC_Layer_InitStruct.LTDC_VerticalStart   = VBP + 1;
  LTDC_Layer_InitStruct.LTDC_VerticalStop    = (ySize + VBP);
  //
  // Pixel Format configuration
  //
  Pixelformat = _GetPixelformat(LayerIndex);
  LTDC_Layer_InitStruct.LTDC_PixelFormat = Pixelformat;
  //
  // Alpha constant (255 totally opaque)
  //
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255;
  //
  // Default Color configuration (configure A, R, G, B component values)
  //
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue  = 0;
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed   = 0;
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;
  //
  // Configure blending factors
  //
  BytesPerLine = _GetBytesPerLine(LayerIndex, xSize);
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
  LTDC_Layer_InitStruct.LTDC_CFBLineLength    = BytesPerLine + 3;
  LTDC_Layer_InitStruct.LTDC_CFBPitch         = BytesPerLine;
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber    = ySize;
  //
  // Input Address configuration
  //
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = _aAddr[LayerIndex];
  LTDC_LayerInit(_apLayer[LayerIndex], &LTDC_Layer_InitStruct);
  //
  // Enable LUT on demand
  //
  BitsPerPixel = LCD_GetBitsPerPixelEx(LayerIndex);
  if (BitsPerPixel <= 8) {
    //
    // Enable usage of LUT for all modes with <= 8bpp
    //
    _LTDC_LayerEnableLUT(_apLayer[LayerIndex], ENABLE);
  } else {
    //
    // Optional CLUT initialization for AL88 mode (16bpp)
    //
    if (_apColorConvAPI[LayerIndex] == GUICC_88666I) {
      _LTDC_LayerEnableLUT(_apLayer[LayerIndex], ENABLE);
      for (i = 0; i < 256; i++) {
        Color = LCD_API_ColorConv_8666.pfIndex2Color(i);
        _LTDC_SetLUTEntry(LayerIndex, Color, i);
      }
    }
  }
  //
  // Enable layer
  //
  LTDC_LayerCmd(_apLayer[LayerIndex], ENABLE);
  //
  // Reload configuration
  //
  LTDC_ReloadConfig(LTDC_SRCR_IMR);
}

/*********************************************************************
*
*       _GetBufferSize
*/
static U32 _GetBufferSize(int LayerIndex) {
  U32 BufferSize;
		
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  BufferSize = _axSize[LayerIndex] * _aySize[LayerIndex] * _aBytesPerPixels[LayerIndex];
  return BufferSize;
}

/*********************************************************************
*
*       _DMA_DrawBitmapL8
*/
static void _DMA_DrawBitmapL8(void * pSrc, void * pDst,  U32 OffSrc, U32 OffDst, U32 PixelFormatDst, U32 xSize, U32 ySize) {
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;
	
  //
  // Set up mode
  //
  DMA2D->CR      = 0x00010000UL | (1 << 9);         // Control Register (Memory to memory with pixel format conversion and TCIE)
  //
  // Set up pointers
  //
  DMA2D->FGMAR   = (U32)pSrc;                       // Foreground Memory Address Register (Source address)
  DMA2D->OMAR    = (U32)pDst;                       // Output Memory Address Register (Destination address)
  //
  // Set up offsets
  //
  DMA2D->FGOR    = OffSrc;                          // Foreground Offset Register (Source line offset)
  DMA2D->OOR     = OffDst;                          // Output Offset Register (Destination line offset)
  //
  // Set up pixel format
  //
  DMA2D->FGPFCCR = LTDC_Pixelformat_L8;             // Foreground PFC Control Register (Defines the input pixel format)
  DMA2D->OPFCCR  = PixelFormatDst;                  // Output PFC Control Register (Defines the output pixel format)
  //
  // Set up size
  //
  DMA2D->NLR     = (U32)(xSize << 16) | ySize;      // Number of Line Register (Size configuration of area to be transfered)
  //
  // Execute operation
  //
  DMA2D->CR     |= 1;                               // Start operation
  //
  // Wait until transfer is done
  //
  while (DMA2D->CR & DMA2D_CR_START) {
    //__WFI();                                        // Sleep until next interrupt
  }
}
/*********************************************************************
*
*       _LCD_CopyBuffer
*/
static void _LCD_CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst) {
  U32 BufferSize, AddrSrc, AddrDst;
		
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrSrc    = _aAddr[LayerIndex] + BufferSize * IndexSrc;
  AddrDst    = _aAddr[LayerIndex] + BufferSize * IndexDst;
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, _axSize[LayerIndex], _aySize[LayerIndex], 0, 0);
}

/*********************************************************************
*
*       _LCD_CopyRect
*/
static void _LCD_CopyRect(int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize) {
  U32 BufferSize, AddrSrc, AddrDst;
		
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrSrc = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y0 * _axSize[LayerIndex] + x0) * _aBytesPerPixels[LayerIndex];
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y1 * _axSize[LayerIndex] + x1) * _aBytesPerPixels[LayerIndex];
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, xSize, ySize, _axSize[LayerIndex] - xSize, 0);
}

/*********************************************************************
*
*       _LCD_FillRect
*/
static void _LCD_FillRect(int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex) {
  U32 BufferSize, AddrDst;
  int xSize, ySize;
		
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  if (GUI_GetDrawMode() == GUI_DM_XOR) {
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, NULL);
    LCD_FillRect(x0, y0, x1, y1);
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, (void(*)(void))_LCD_FillRect);
  } else {
    xSize = x1 - x0 + 1;
    ySize = y1 - y0 + 1;
    BufferSize = _GetBufferSize(LayerIndex);
    AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y0 * _axSize[LayerIndex] + x0) * _aBytesPerPixels[LayerIndex];
    _DMA_Fill(LayerIndex, (void *)AddrDst, xSize, ySize, _axSize[LayerIndex] - xSize, PixelIndex);
  }
}

/*********************************************************************
*
*       _LCD_DrawBitmap16bpp
*/
static void _LCD_DrawBitmap16bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
		
  /* Function was called, EMWIN has do some job, update LCD when necessary */
  EMWIN_LCD_DRIVER_CB_CALLED = 1;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = 240/*(BytesPerLine / 2)*/ - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_Copy(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}


/*********************************************************************
*
*       _LCD_DrawBitmap8bpp
*/
static void _LCD_DrawBitmap8bpp(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine) {
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;
	

  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = BytesPerLine - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  PixelFormat = _GetPixelformat(LayerIndex);
  _DMA_DrawBitmapL8((void *)p, (void *)AddrDst, OffLineSrc, OffLineDst, PixelFormat, xSize, ySize);
}
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       DMA2D_ISR_Handler
*
* Purpose:
*   Transfer-complete-interrupt of DMA2D
*/
void DMA2D_ISR_Handler(void) {
  DMA2D->IFCR = (U32)DMA2D_IFSR_CTCIF;
}

/*********************************************************************
*
*       LTDC_ISR_Handler
*
* Purpose:
*   End-Of-Frame-Interrupt for managing multiple buffering
*/
void LTDC_ISR_Handler(void) {
  U32 Addr;
  int i;

  LTDC->ICR = (U32)LTDC_IER_LIE;
  for (i = 0; i < GUI_NUM_LAYERS; i++) {
    if (_aPendingBuffer[i] >= 0) {
      //
      // Calculate address of buffer to be used  as visible frame buffer
      //
      Addr = _aAddr[i] + _axSize[i] * _aySize[i] * _aPendingBuffer[i] * _aBytesPerPixels[i];
      //
      // Store address into SFR
      //
      _apLayer[i]->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
      _apLayer[i]->CFBAR = Addr;
      //
      // Reload configuration
      //
      LTDC_ReloadConfig(LTDC_SRCR_IMR);
      //
      // Tell emWin that buffer is used
      //
      GUI_MULTIBUF_ConfirmEx(i, _aPendingBuffer[i]);
      //
      // Clear pending buffer flag of layer
      //
      _aBufferIndex[i] = _aPendingBuffer[i];
      _aPendingBuffer[i] = -1;
    }
  }
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r = 0;

  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the display controller and put it into operation.
    //
    _LCD_InitController(LayerIndex);
    break;
  }
  case LCD_X_SETORG: {
    //
    // Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p
    //
    LCD_X_SETORG_INFO * p;

    p = (LCD_X_SETORG_INFO *)pData;
    _apLayer[LayerIndex]->CFBAR = _aAddr[LayerIndex] + p->yPos * _axSize[LayerIndex] * _aBytesPerPixels[LayerIndex];
    LTDC_ReloadConfig(LTDC_SRCR_VBR); // Reload on next blanking period
    break;
  }
  case LCD_X_SHOWBUFFER: {
    //
    // Required if multiple buffers are used. The 'Index' element of p contains the buffer index.
    //
    LCD_X_SHOWBUFFER_INFO * p;

    p = (LCD_X_SHOWBUFFER_INFO *)pData;
    _aPendingBuffer[LayerIndex] = p->Index;
    break;
  }
  case LCD_X_SETLUTENTRY: {
    //
    // Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p
    //
    LCD_X_SETLUTENTRY_INFO * p;

    p = (LCD_X_SETLUTENTRY_INFO *)pData;
    _LTDC_SetLUTEntry(LayerIndex, p->Color, p->Pos);
    break;
  }
  case LCD_X_ON: {
    //
    // Required if the display controller should support switching on and off
    //
    _LCD_DisplayOn();
    break;
  }
  case LCD_X_OFF: {
    //
    // Required if the display controller should support switching on and off
    //
    //LCD_DisplayOff();
    break;
  }
  case LCD_X_SETVIS: {
    //
    // Required for setting the layer visibility which is passed in the 'OnOff' element of pData
    //
    LCD_X_SETVIS_INFO * p;

    p = (LCD_X_SETVIS_INFO *)pData;
    LTDC_LayerCmd(_apLayer[LayerIndex], p->OnOff ? ENABLE : DISABLE);
   
    /* Reload shadow register */
   LTDC_ReloadConfig(LTDC_SRCR_IMR);
    break;
  }
  case LCD_X_SETPOS: {
    //
    // Required for setting the layer position which is passed in the 'xPos' and 'yPos' element of pData
    //
    LCD_X_SETPOS_INFO * p;

    p = (LCD_X_SETPOS_INFO *)pData;
    _LTDC_SetLayerPos(LayerIndex, p->xPos, p->yPos);
    break;
  }
  case LCD_X_SETSIZE: {
    //
    // Required for setting the layer position which is passed in the 'xPos' and 'yPos' element of pData
    //
    LCD_X_SETSIZE_INFO * p;
    int xPos, yPos;

    GUI_GetLayerPosEx(LayerIndex, &xPos, &yPos);
    p = (LCD_X_SETSIZE_INFO *)pData;
    _axSize[LayerIndex] = p->xSize;
    _aySize[LayerIndex] = p->ySize;
    _LTDC_SetLayerPos(LayerIndex, xPos, yPos);
    break;
  }
  case LCD_X_SETALPHA: {
    //
    // Required for setting the alpha value which is passed in the 'Alpha' element of pData
    //
    LCD_X_SETALPHA_INFO * p;

    p = (LCD_X_SETALPHA_INFO *)pData;
    _LTDC_SetLayerAlpha(LayerIndex, p->Alpha);
    break;
  }
  case LCD_X_SETCHROMAMODE: {
    //
    // Required for setting the chroma mode which is passed in the 'ChromaMode' element of pData
    //
    LCD_X_SETCHROMAMODE_INFO * p;

    p = (LCD_X_SETCHROMAMODE_INFO *)pData;
    _LTDC_LayerEnableColorKeying(_apLayer[LayerIndex], (p->ChromaMode != 0) ? ENABLE : DISABLE);
    break;
  }
  case LCD_X_SETCHROMA: {
    //
    // Required for setting the chroma value which is passed in the 'ChromaMin' and 'ChromaMax' element of pData
    //
    LCD_X_SETCHROMA_INFO * p;
    U32 Color;

    p = (LCD_X_SETCHROMA_INFO *)pData;
    Color = ((p->ChromaMin & 0xFF0000) >> 16) | (p->ChromaMin & 0x00FF00) | ((p->ChromaMin & 0x0000FF) << 16);
    _apLayer[LayerIndex]->CKCR = Color;
    LTDC_ReloadConfig(LTDC_SRCR_VBR); // Reload on next blanking period
    break;
  }
  default:
    r = -1;
  }
  return r;
}

/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) {
  int i;

  //
  // At first initialize use of multiple buffers on demand
  //
  #if (NUM_BUFFERS > 1)
    for (i = 0; i < GUI_NUM_LAYERS; i++) {
      GUI_MULTIBUF_ConfigEx(i, NUM_BUFFERS);
    }
  #endif
  //
  // Set display driver and color conversion for 1st layer
  //
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_0, COLOR_CONVERSION_0, 0, 0);
  //
  // Set size of 1st layer
  //
  LCD_SetSizeEx (0, XSIZE_0, YSIZE_0);
  LCD_SetVSizeEx(0, XSIZE_0, YSIZE_0 * NUM_VSCREENS);
  #if (GUI_NUM_LAYERS > 1)
    //
    // Set display driver and color conversion for 2nd layer
    //
    GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_1, COLOR_CONVERSION_1, 0, 1);
    //
    // Set size of 2nd layer
    //
    LCD_SetSizeEx (1, XSIZE_1, YSIZE_1);
    LCD_SetVSizeEx(1, XSIZE_1, YSIZE_1 * NUM_VSCREENS);
  #endif
  //
  // Setting up VRam address and custom functions for CopyBuffer-, CopyRect- and FillRect operations
  //
  for (i = 0; i < GUI_NUM_LAYERS; i++) 
  {
    _aPendingBuffer[i] = -1;
    //
    // Set VRAM address
    //
    LCD_SetVRAMAddrEx(i, (void *)(_aAddr[i]));
    //
    // Remember color depth for further operations
    //
    _aBytesPerPixels[i] = LCD_GetBitsPerPixelEx(i) >> 3;
    //
    // Set custom functions for several operations
    //
    LCD_SetDevFunc(i, LCD_DEVFUNC_COPYBUFFER, (void(*)(void))_LCD_CopyBuffer);
    LCD_SetDevFunc(i, LCD_DEVFUNC_COPYRECT,   (void(*)(void))_LCD_CopyRect);
    //
    // Filling via DMA2D does only work with 16bpp or more
    //
    if (_GetPixelformat(i) <= LTDC_Pixelformat_ARGB4444) {
      LCD_SetDevFunc(i, LCD_DEVFUNC_FILLRECT, (void(*)(void))_LCD_FillRect);
      LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_8BPP, (void(*)(void))_LCD_DrawBitmap8bpp); 
    }
    //
    // Set up drawing routine for 16bpp bitmap using DMA2D
    //
    if (_GetPixelformat(i) == LTDC_Pixelformat_RGB565) {
      LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_16BPP, (void(*)(void))_LCD_DrawBitmap16bpp);     // Set up drawing routine for 16bpp bitmap using DMA2D. Makes only sense with RGB565
    }
    
  //
  // Set up custom color conversion using DMA2D, works only for direct color modes because of missing LUT for DMA2D destination
  //
  GUICC_M1555I_SetCustColorConv(_Color2IndexBulk_M1555I_DMA2D, _Index2ColorBulk_M1555I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB1555
  GUICC_M565_SetCustColorConv  (_Color2IndexBulk_M565_DMA2D,   _Index2ColorBulk_M565_DMA2D);   // Set up custom bulk color conversion using DMA2D for RGB565
  GUICC_M4444I_SetCustColorConv(_Color2IndexBulk_M4444I_DMA2D, _Index2ColorBulk_M4444I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB4444
  GUICC_M888_SetCustColorConv  (_Color2IndexBulk_M888_DMA2D,   _Index2ColorBulk_M888_DMA2D);   // Set up custom bulk color conversion using DMA2D for RGB888
  GUICC_M8888I_SetCustColorConv(_Color2IndexBulk_M8888I_DMA2D, _Index2ColorBulk_M8888I_DMA2D); // Set up custom bulk color conversion using DMA2D for ARGB8888
  //  

    //
  // Set up custom alpha blending function using DMA2D
  //
  GUI_SetFuncAlphaBlending(_DMA_AlphaBlending);                                                // Set up custom alpha blending function using DMA2D
  //
  // Set up custom function for translating a bitmap palette into index values.
  // Required to load a bitmap palette into DMA2D CLUT in case of a 8bpp indexed bitmap
  //
  GUI_SetFuncGetpPalConvTable(_LCD_GetpPalConvTable);
  //
  // Set up a custom function for mixing up single colors using DMA2D
  //
  GUI_SetFuncMixColors(_DMA_MixColors);
  //
  // Set up a custom function for mixing up arrays of colors using DMA2D
  //
  GUI_SetFuncMixColorsBulk(_LCD_MixColorsBulk);
  }
}

/*************************** End of file ****************************/
