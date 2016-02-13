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
#include "tm_stm32f4_dma2d_graphic.h"

/* Absolute number */
#define ABS(X)	((X) > 0 ? (X) : -(X))    

/* Internal structure */
typedef struct {
	uint16_t Width;
	uint16_t Height;
	uint16_t CurrentWidth;
	uint16_t CurrentHeight;
	uint32_t StartAddress;
	uint32_t LayerOffset;
	uint32_t Offset;
	uint32_t Pixels;
	uint8_t Initialized;
	uint8_t Orientation;
	uint8_t PixelSize;
} TM_INT_DMA2D_t;

/* Private structures */
static DMA2D_InitTypeDef GRAPHIC_DMA2D_InitStruct;
//static DMA2D_FG_InitTypeDef GRAPHIC_DMA2D_FG_InitStruct;
volatile TM_INT_DMA2D_t DIS;

__STATIC_INLINE void DrawPixel(uint16_t x, uint16_t y, uint32_t color) {
	TM_DMA2DGRAPHIC_DrawHorizontalLine(x, y, 1, color);
}

/* Private functions */
void TM_INT_DMA2DGRAPHIC_InitAndTransfer(void);
void TM_INT_DMA2DGRAPHIC_SetMemory(uint32_t MemoryAddress, uint32_t Offset, uint32_t NumberOfLine, uint32_t PixelPerLine);
void TM_INT_DMA2DGRAPHIC_DrawCircleCorner(int16_t x0, int16_t y0, int16_t r, uint8_t corner, uint32_t color);
void TM_INT_DMA2DGRAPHIC_DrawFilledCircleCorner(int16_t x0, int16_t y0, int16_t r, uint8_t corner, uint32_t color);

void TM_DMA2DGRAPHIC_Init(void) {
	/* Internal settings */
	DIS.StartAddress = DMA2D_GRAPHIC_RAM_ADDR;
	DIS.Offset = 0;
	DIS.Width = DMA2D_GRAPHIC_LCD_WIDTH;
	DIS.Height = DMA2D_GRAPHIC_LCD_HEIGHT;
	DIS.Pixels = DMA2D_GRAPHIC_PIXELS;
	DIS.CurrentHeight = DMA2D_GRAPHIC_LCD_WIDTH;
	DIS.CurrentWidth = DMA2D_GRAPHIC_LCD_HEIGHT;
	DIS.Orientation = 0;
	DIS.PixelSize = 2;
	DIS.LayerOffset = DMA2D_GRAPHIC_LCD_WIDTH * DMA2D_GRAPHIC_LCD_HEIGHT * DIS.PixelSize;
	
	/* Enable DMA2D clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2DEN;
	
	/* Initialized */
	DIS.Initialized = 1;
}

void TM_DMA2DGRAPHIC_SetLayer(uint8_t layer_number) {
	/* Set offset */
	DIS.Offset = (layer_number - 1) * DIS.LayerOffset;
}

void TM_DMA2DGRAPHIC_DrawPixel(uint16_t x, uint16_t y, uint32_t color) {
	if (DIS.Orientation == 1) { /* Normal */
		*(__IO uint16_t *) (DIS.StartAddress + DIS.Offset + DIS.PixelSize * (y * DIS.Width + x)) = color;
	} else if (DIS.Orientation == 0) { /* 180 */
		*(__IO uint16_t *) (DIS.StartAddress + DIS.Offset + DIS.PixelSize * ((DIS.Height - y - 1) * DIS.Width + (DIS.Width - x - 1))) = color;
	} else if (DIS.Orientation == 3) { /* 90 */ /* x + width * y */
		*(__IO uint16_t *) (DIS.StartAddress + DIS.Offset + DIS.PixelSize * ((x) * DIS.Width + DIS.Width - y - 1)) = color;
	} else if (DIS.Orientation == 2) { /* 270 */
		*(__IO uint16_t *) (DIS.StartAddress + DIS.Offset + DIS.PixelSize * ((DIS.Height - x - 1) * DIS.Width + y)) = color;
	}
}

uint32_t TM_DMA2DGRAPHIC_GetPixel(uint16_t x, uint16_t y) {
	if (DIS.Orientation == 1) { /* Normal */
		return *(__IO uint16_t *) (DIS.StartAddress + DIS.Offset + DIS.PixelSize * (y * DIS.Width + x));
	} else if (DIS.Orientation == 0) { /* 180 */
		return *(__IO uint16_t *) (DIS.StartAddress + DIS.Offset + DIS.PixelSize * ((DIS.Height - y - 1) * DIS.Width + (DIS.Width - x - 1)));
	} else if (DIS.Orientation == 3) { /* 90 */ /* x + width * y */
		return *(__IO uint16_t *) (DIS.StartAddress + DIS.Offset + DIS.PixelSize * ((x) * DIS.Width + DIS.Width - y - 1));
	} else if (DIS.Orientation == 2) { /* 270 */
		return *(__IO uint16_t *) (DIS.StartAddress + DIS.Offset + DIS.PixelSize * ((DIS.Height - x - 1) * DIS.Width + y));
	}
	return 0;
}

void TM_DMA2DGRAPHIC_SetOrientation(uint8_t orientation) {
	/* Filter */
	if (orientation > 3) {
		return;
	}
	
	/* Save new orientation */
	DIS.Orientation = orientation;
	
	if (
		orientation == 0 ||
		orientation == 1
	) {
		DIS.CurrentHeight = DMA2D_GRAPHIC_LCD_HEIGHT;
		DIS.CurrentWidth = DMA2D_GRAPHIC_LCD_WIDTH;
	} else {
		DIS.CurrentHeight = DMA2D_GRAPHIC_LCD_WIDTH;
		DIS.CurrentWidth = DMA2D_GRAPHIC_LCD_HEIGHT;
	}
}

void TM_DMA2DGRAPHIC_Fill(uint32_t color) {
	/* Set DMA2D settings */
	GRAPHIC_DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;
	GRAPHIC_DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;
	
	/* Set colors */
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputBlue = (0x001F & color);
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputGreen = (0x07E0 & color) >> 5;
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputRed = (0xF800 & color) >> 11;
	
	/* Set memory */
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputMemoryAdd = DIS.StartAddress + DIS.Offset;
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputOffset = 0;
	GRAPHIC_DMA2D_InitStruct.DMA2D_NumberOfLine = DIS.Height;
	GRAPHIC_DMA2D_InitStruct.DMA2D_PixelPerLine = DIS.Width;
	
	/* Start transfer and wait till done */
	TM_INT_DMA2DGRAPHIC_InitAndTransfer();
}

void TM_DMA2DGRAPHIC_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
	/* Check if initialized */
	if (DIS.Initialized != 1) {
		return;
	}
	
	/* Filter */
	if (
		x >= DIS.CurrentWidth ||
		y >= DIS.CurrentHeight
	) {
		return;
	}
	
	/* Filter height */
	if ((y + height) >= DIS.CurrentHeight) {
		height = DIS.CurrentHeight - y;
	}
	/* Filter width */
	if ((x + width) >= DIS.CurrentWidth) {
		width = DIS.CurrentWidth - x;
	}
	
	/* Set DMA2D settings */
	GRAPHIC_DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;
	GRAPHIC_DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;
	
	/* Set colors */
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputBlue = (0x001F & color);
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputGreen = (0x07E0 & color) >> 5;
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputRed = (0xF800 & color) >> 11;
	
	/* Set memory settings */
	if (DIS.Orientation == 1) { /* Normal */
		TM_INT_DMA2DGRAPHIC_SetMemory(DIS.PixelSize * (y * DIS.Width + x), DIS.Width - width, height, width);
	} 
	if (DIS.Orientation == 0) { /* 180 */
		TM_INT_DMA2DGRAPHIC_SetMemory(DIS.PixelSize * ((DIS.Height - height - y) * DIS.Width + DIS.Width - x - width), DIS.Width - width, height, width);
	} 
	if (DIS.Orientation == 3) { /* 90 */
		TM_INT_DMA2DGRAPHIC_SetMemory(DIS.PixelSize * (DIS.Width - y - height + DIS.Width * x), DIS.Width - height, width, height);
	} 
	if (DIS.Orientation == 2) { /* 270 */
		TM_INT_DMA2DGRAPHIC_SetMemory(DIS.PixelSize * (y + DIS.Width * (DIS.Height - width - x)), DIS.Width - height, width, height);
	}
	
	/* Start transfer and wait till done */
	TM_INT_DMA2DGRAPHIC_InitAndTransfer();
}

void TM_DMA2DGRAPHIC_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
	/* Draw 2 vertical lines */
	TM_DMA2DGRAPHIC_DrawVerticalLine(x, y, height, color);
	TM_DMA2DGRAPHIC_DrawVerticalLine(x + width - 1, y, height, color);
	
	/* Draw 2 horizontal lines */
	TM_DMA2DGRAPHIC_DrawHorizontalLine(x, y, width, color);
	TM_DMA2DGRAPHIC_DrawHorizontalLine(x, y + height, width, color);
}

void TM_DMA2DGRAPHIC_DrawRoundedRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t r, uint32_t color) {
	/* Check input parameters */
	if (width == 0 || height == 0) {
		return;
	}
	
	/* Check max radius */
	if (r > (width / 2)) {
		r = width / 2 - 1;
	}
	if (r > (height / 2)) {
		r = height / 2 - 1;
	}
	if (
		r > (width / 2) || 
		r > (height / 2)
	) {
		r = 0;
	}
	
	/* No radius */
	if (r == 0) {
		/* Draw normal rectangle */
		TM_DMA2DGRAPHIC_DrawRectangle(x, y, width, height, color);
		
		/* Return from function */
		return;
	}
	
	/* Draw lines */
	TM_DMA2DGRAPHIC_DrawHorizontalLine(x + r, y, width - 2 * r, color); /* Top */
	TM_DMA2DGRAPHIC_DrawHorizontalLine(x + r, y + height - 1, width - 2 * r, color); /* Bottom */
	
	TM_DMA2DGRAPHIC_DrawVerticalLine(x, y + r, height - 2 * r, color); /* Right */
	TM_DMA2DGRAPHIC_DrawVerticalLine(x + width - 1, y + r, height - 2 * r, color); /* Left */
	
	/* Draw corners */
	TM_INT_DMA2DGRAPHIC_DrawCircleCorner(x + r, y + r, r, 0x01, color); /* Top left */
	TM_INT_DMA2DGRAPHIC_DrawCircleCorner(x + width - r - 1, y + r, r, 0x02, color); /* Top right */
	TM_INT_DMA2DGRAPHIC_DrawCircleCorner(x + width - r - 1, y + height - r - 1, r, 0x04, color); /* Bottom right */
	TM_INT_DMA2DGRAPHIC_DrawCircleCorner(x + r, y + height - r - 1, r, 0x08, color); /* Bottom left */
}

void TM_DMA2DGRAPHIC_DrawFilledRoundedRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t r, uint32_t color) {
	/* Check input parameters */
	if (width == 0 || height == 0) {
		return;
	}
	
	/* Check max radius */
	if (r > (width / 2)) {
		r = width / 2 - 1;
	}
	if (r > (height / 2)) {
		r = height / 2 - 1;
	}
	if (
		r > (width / 2) || 
		r > (height / 2)
	) {
		r = 0;
	}
	
	/* No radius */
	if (r == 0) {
		TM_DMA2DGRAPHIC_DrawFilledRectangle(x, y, width, height, color);
		return;
	}
	
	/* Draw rectangles */
	TM_DMA2DGRAPHIC_DrawFilledRectangle(x + r, y, width - 2 * r, height, color);
	TM_DMA2DGRAPHIC_DrawFilledRectangle(x, y + r, r, height - 2 * r, color);
	TM_DMA2DGRAPHIC_DrawFilledRectangle(x + width - r, y + r, r, height - 2 * r, color);
	
	/* Draw corners */
	TM_INT_DMA2DGRAPHIC_DrawFilledCircleCorner(x + r, y + r, r, 0x01, color); /* Top left */
	TM_INT_DMA2DGRAPHIC_DrawFilledCircleCorner(x + width - r - 1, y + r, r, 0x02, color); /* Top right */
	TM_INT_DMA2DGRAPHIC_DrawFilledCircleCorner(x + width - r - 1, y + height - r - 1, r, 0x04, color); /* Bottom right */
	TM_INT_DMA2DGRAPHIC_DrawFilledCircleCorner(x + r, y + height - r - 1, r, 0x08, color); /* Bottom left */
}

void TM_DMA2DGRAPHIC_DrawVerticalLine(int16_t x, int16_t y, uint16_t length, uint32_t color) {
	/* Check if initialized */
	if (DIS.Initialized != 1) {
		return;
	}
	
	/* Filter */
	if (
		x >= DIS.CurrentWidth ||
		y >= DIS.CurrentHeight ||
		length == 0
	) {
		return;
	}
	
	if (x < 0) {
		x = 0;
	}
	
	if (y < 0) {
		y = 0;
	}
	
	/* Filter height */
	if ((y + length) >= DIS.CurrentHeight) {
		length = DIS.CurrentHeight - y;
	}
	
	/* Set DMA2D settings */
	GRAPHIC_DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;
	GRAPHIC_DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;
	
	/* Set colors */
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputBlue = (0x001F & color);
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputGreen = (0x07E0 & color) >> 5;
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputRed = (0xF800 & color) >> 11;
	
	/* Set memory settings */
	if (DIS.Orientation == 1) { /* Normal */
		TM_INT_DMA2DGRAPHIC_SetMemory(DIS.PixelSize * (y * DIS.Width + x), DIS.Width - 1, length, 1);
	} 
	if (DIS.Orientation == 0) { /* 180 */
		TM_INT_DMA2DGRAPHIC_SetMemory(DIS.PixelSize * ((DIS.Height - length - y) * DIS.Width + DIS.Width - x - 1), DIS.Width - 1, length, 1);
	} 
	if (DIS.Orientation == 3) { /* 90 */
		TM_INT_DMA2DGRAPHIC_SetMemory(DIS.PixelSize * (DIS.Width - y - length + DIS.Width * x), DIS.Width - length, 1, length);
	} 
	if (DIS.Orientation == 2) { /* 270 */
		TM_INT_DMA2DGRAPHIC_SetMemory(DIS.PixelSize * (y + DIS.Width * (DIS.Height - 1 - x)), DIS.Width - length, 1, length);
	}
	
	/* Start transfer and wait till done */
	TM_INT_DMA2DGRAPHIC_InitAndTransfer();
}

void TM_DMA2DGRAPHIC_DrawHorizontalLine(int16_t x, int16_t y, uint16_t length, uint32_t color) {
	/* Check if initialized */
	if (DIS.Initialized != 1) {
		return;
	}
	
	/* Filter */
	if (
		x >= DIS.CurrentWidth ||
		y >= DIS.CurrentHeight ||
		length == 0
	) {
		return;
	}
	
	if (x < 0) {
		length += x;
		x = 0;
	}
	
	if (y < 0) {
		y = 0;
	}
	
	/* Filter height */
	if ((x + length) >= DIS.CurrentWidth) {
		length = DIS.CurrentWidth - x;
	}

	/* Set DMA2D settings */
	GRAPHIC_DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;
	GRAPHIC_DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;
	
	/* Set colors */
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputAlpha = 0x00;
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputBlue = (0x001F & color);
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputGreen = (0x07E0 & color) >> 5;
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputRed = (0xF800 & color) >> 11;
	
	/* Set memory settings */
	if (DIS.Orientation == 1) { /* Normal */
		TM_INT_DMA2DGRAPHIC_SetMemory(DIS.PixelSize * (y * DIS.Width + x), DIS.Width - length, 1, length);
	} 
	if (DIS.Orientation == 0) { /* 180 */
		TM_INT_DMA2DGRAPHIC_SetMemory(DIS.PixelSize * ((DIS.Height - 1 - y) * DIS.Width + DIS.Width - x - length), DIS.Width - length, 1, length);
	} 
	if (DIS.Orientation == 3) { /* 90 */
		TM_INT_DMA2DGRAPHIC_SetMemory(DIS.PixelSize * (DIS.Width - y - 1 + DIS.Width * x), DIS.Width - 1, length, 1);
	} 
	if (DIS.Orientation == 2) { /* 270 */
		TM_INT_DMA2DGRAPHIC_SetMemory(DIS.PixelSize * (y + DIS.Width * (DIS.Height - length - x)), DIS.Width - 1, length, 1);
	}
	
	/* Start transfer and wait till done */
	TM_INT_DMA2DGRAPHIC_InitAndTransfer();
}

void TM_DMA2DGRAPHIC_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color) {
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;

	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;
	
	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	} else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	} else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay) {
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	} else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		DrawPixel(x, y, color);
		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void TM_DMA2DGRAPHIC_DrawPolyLine(TM_DMA2DRAPHIC_Poly_t* Coordinates, uint16_t count, uint32_t color) {
	uint16_t X, Y;
	
	while (--count) {
		X = Coordinates->X;
		Y = Coordinates->Y;
		Coordinates++;
		TM_DMA2DGRAPHIC_DrawLine(X, Y, Coordinates->X, Coordinates->Y, color);
	};
}

void TM_DMA2DGRAPHIC_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    TM_DMA2DGRAPHIC_DrawVerticalLine(x0, y0 + r, 1, color);
    TM_DMA2DGRAPHIC_DrawVerticalLine(x0, y0 - r, 1, color);
    TM_DMA2DGRAPHIC_DrawVerticalLine(x0 + r, y0, 1, color);
    TM_DMA2DGRAPHIC_DrawVerticalLine(x0 - r, y0, 1, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        TM_DMA2DGRAPHIC_DrawVerticalLine(x0 + x, y0 + y, 1, color);
        TM_DMA2DGRAPHIC_DrawVerticalLine(x0 - x, y0 + y, 1, color);
        TM_DMA2DGRAPHIC_DrawVerticalLine(x0 + x, y0 - y, 1, color);
        TM_DMA2DGRAPHIC_DrawVerticalLine(x0 - x, y0 - y, 1, color);

        TM_DMA2DGRAPHIC_DrawVerticalLine(x0 + y, y0 + x, 1, color);
        TM_DMA2DGRAPHIC_DrawVerticalLine(x0 - y, y0 + x, 1, color);
        TM_DMA2DGRAPHIC_DrawVerticalLine(x0 + y, y0 - x, 1, color);
        TM_DMA2DGRAPHIC_DrawVerticalLine(x0 - y, y0 - x, 1, color);
    }
}

void TM_DMA2DGRAPHIC_DrawFilledCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    TM_DMA2DGRAPHIC_DrawVerticalLine(x0, y0 + r, 1, color);
    TM_DMA2DGRAPHIC_DrawVerticalLine(x0, y0 - r, 1, color);
    TM_DMA2DGRAPHIC_DrawVerticalLine(x0 + r, y0, 1, color);
    TM_DMA2DGRAPHIC_DrawVerticalLine(x0 - r, y0, 1, color);
    TM_DMA2DGRAPHIC_DrawHorizontalLine(x0 - r, y0, 2 * r, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        TM_DMA2DGRAPHIC_DrawHorizontalLine(x0 - x, y0 + y, 2 * x, color);
		TM_DMA2DGRAPHIC_DrawHorizontalLine(x0 - x, y0 - y, 2 * x, color);

        TM_DMA2DGRAPHIC_DrawHorizontalLine(x0 - y, y0 + x, 2 * y, color);
        TM_DMA2DGRAPHIC_DrawHorizontalLine(x0 - y, y0 - x, 2 * y, color);
    }
}

void TM_DMA2DGRAPHIC_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color) {
	/* Draw lines */
	TM_DMA2DGRAPHIC_DrawLine(x1, y1, x2, y2, color);
	TM_DMA2DGRAPHIC_DrawLine(x2, y2, x3, y3, color);
	TM_DMA2DGRAPHIC_DrawLine(x3, y3, x1, y1, color);
}


void TM_DMA2DGRAPHIC_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color) {
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;

	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	} else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	} else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay){
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	} else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		TM_DMA2DGRAPHIC_DrawLine(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void TM_DMA2DGRAPHIC_CopyBuffer(void* pSrc, void* pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLineSrc, uint32_t OffLineDst) {
	/* Copy buffer using interrupt */
	TM_DMA2DGRAPHIC_CopyBufferIT(pSrc, pDst, xSize, ySize, OffLineSrc, OffLineDst);

	/* Wait until transfer is done */
	DMA2D_WAIT;
}


void TM_DMA2DGRAPHIC_CopyBufferIT(void* pSrc, void* pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLineSrc, uint32_t OffLineDst) {
	/* Wait for previous operation to be done */
	DMA2D_WAIT;
	
	/* DeInit DMA2D */
	RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA2DRST;
	RCC->AHB1RSTR &= ~RCC_AHB1RSTR_DMA2DRST;
	
	DMA2D->CR = 0x00000000UL | (1 << 9);
	
	/* Set up pointers */
	DMA2D->FGMAR = (uint32_t)pSrc;                       
	DMA2D->OMAR = (uint32_t)pDst;                       
	DMA2D->FGOR = OffLineSrc;                      
	DMA2D->OOR = OffLineDst; 

	/* Set up pixel format */  
	DMA2D->FGPFCCR = LTDC_Pixelformat_RGB565;  

	/* Set up size */
	DMA2D->NLR = (uint32_t)(xSize << 16) | (uint16_t)ySize; 

	/* Start DMA2D */
	DMA2D->CR |= DMA2D_CR_START; 
}

/* Private functions */
void TM_INT_DMA2DGRAPHIC_SetConf(TM_DMA2DGRAPHIC_INT_Conf_t* Conf) {
	/* Fill settings for DMA2D */
	DIS.Width = Conf->Width;
	DIS.Height = Conf->Height;
	DIS.StartAddress = Conf->BufferStart;
	DIS.LayerOffset = Conf->BufferOffset;
	DIS.PixelSize = Conf->BytesPerPixel;
	DIS.Pixels = DIS.Width * DIS.Height;
	DIS.Orientation = Conf->Orientation;
	
	/* Set DMA2D orientation */
	TM_DMA2DGRAPHIC_SetOrientation(DIS.Orientation);
}

void TM_INT_DMA2DGRAPHIC_InitAndTransfer(void) {
	/* Wait until transfer is done first from other calls */
	DMA2D_WAIT;
	
	/* DeInit DMA2D */
	RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA2DRST;
	RCC->AHB1RSTR &= ~RCC_AHB1RSTR_DMA2DRST;
	
	/* Initialize DMA2D */
	DMA2D_Init(&GRAPHIC_DMA2D_InitStruct);
	
	/* Start transfer */
	DMA2D->CR |= (uint32_t)DMA2D_CR_START;
	
	/* Wait till transfer ends */
	DMA2D_WAIT;
}

void TM_INT_DMA2DGRAPHIC_SetMemory(uint32_t MemoryAddress, uint32_t Offset, uint32_t NumberOfLine, uint32_t PixelPerLine) {	
	/* Set memory settings */
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputMemoryAdd = DIS.StartAddress + DIS.Offset + MemoryAddress;
	GRAPHIC_DMA2D_InitStruct.DMA2D_OutputOffset = Offset;
	GRAPHIC_DMA2D_InitStruct.DMA2D_NumberOfLine = NumberOfLine;
	GRAPHIC_DMA2D_InitStruct.DMA2D_PixelPerLine = PixelPerLine;
}

void TM_INT_DMA2DGRAPHIC_DrawCircleCorner(int16_t x0, int16_t y0, int16_t r, uint8_t corner, uint32_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (corner & 0x01) {/* Top left */	
			TM_DMA2DGRAPHIC_DrawPixel(x0 - y, y0 - x, color);
			TM_DMA2DGRAPHIC_DrawPixel(x0 - x, y0 - y, color);
		}
		
        if (corner & 0x02) {/* Top right */
			TM_DMA2DGRAPHIC_DrawPixel(x0 + x, y0 - y, color);
			TM_DMA2DGRAPHIC_DrawPixel(x0 + y, y0 - x, color);
		}
		
		if (corner & 0x04) {/* Bottom right */
			TM_DMA2DGRAPHIC_DrawPixel(x0 + x, y0 + y, color);
			TM_DMA2DGRAPHIC_DrawPixel(x0 + y, y0 + x, color);
		}
		
        if (corner & 0x08) {/* Bottom left */	
			TM_DMA2DGRAPHIC_DrawPixel(x0 - x, y0 + y, color);
			TM_DMA2DGRAPHIC_DrawPixel(x0 - y, y0 + x, color);
		}
    }
}

void TM_INT_DMA2DGRAPHIC_DrawFilledCircleCorner(int16_t x0, int16_t y0, int16_t r, uint8_t corner, uint32_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (corner & 0x01) {/* Top left */
			TM_DMA2DGRAPHIC_DrawLine(x0, y0 - y, x0 - x, y0 - y, color);
			TM_DMA2DGRAPHIC_DrawLine(x0, y0 - x, x0 - y, y0 - x, color);
		}
		
        if (corner & 0x02) {/* Top right */
			TM_DMA2DGRAPHIC_DrawLine(x0 + x, y0 - y, x0, y0 - y, color);
			TM_DMA2DGRAPHIC_DrawLine(x0 + y, y0 - x, x0, y0 - x, color);
		}
		
		if (corner & 0x04) {/* Bottom right */
			TM_DMA2DGRAPHIC_DrawLine(x0, y0 + y, x0 + x, y0 + y, color);
			TM_DMA2DGRAPHIC_DrawLine(x0 + y, y0 + x, x0, y0 + x, color);
		}
		
        if (corner & 0x08) {/* Bottom left */
			TM_DMA2DGRAPHIC_DrawLine(x0 - x, y0 + y, x0, y0 + y, color);
			TM_DMA2DGRAPHIC_DrawLine(x0, y0 + x, x0 - y, y0 + x, color);
		}
    }
}
