/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
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
#include "tm_stm32f4_emwin.h"

TM_STMPE811_TouchData TS_Data;
GUI_PID_STATE TS_State;

extern void LTDC_ISR_Handler(void);
extern void DMA2D_ISR_Handler(void);

TM_EMWIN_Result_t TM_EMWIN_Init(void) {
	/* Initialize CRC for emWin */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	
	/* Initialize delay */
	TM_DELAY_Init();

	/* Initialize LCD */
	TM_ILI9341_Init();
	
	/* Initialize touch screen */
	if (TM_STMPE811_Init() != TM_STMPE811_State_Ok) {
		/* Return touch error */
		return TM_EMWIN_Result_TouchError;
	}
	
	/* Initialize GUI */
	if (GUI_Init() != 0) {
		/* Return GUI error */
		return TM_EMWIN_Result_GUIError;
	}
	
	/* Set LCD default orientation */
	TS_Data.orientation = TM_STMPE811_Orientation_Portrait_2;
	
	/* Return OK */
	return TM_EMWIN_Result_Ok;
}

TM_EMWIN_Result_t TM_EMWIN_Rotate(TM_EMWIN_Rotate_t rotation) {	
#if TM_EMWIN_ROTATE_LCD == 0
	/* Return error */
	return TM_EMWIN_Result_Error;	
#else
	uint8_t result;
	TM_STMPE811_TouchData td;
	
	/* Try to rotate LCD */
	switch (rotation) {
		case TM_EMWIN_Rotate_0:
			result = GUI_SetOrientation(0);
			td.orientation = TM_STMPE811_Orientation_Portrait_2;
			break;
		case TM_EMWIN_Rotate_90:
			result = GUI_SetOrientation(GUI_SWAP_XY | GUI_MIRROR_X);
			td.orientation = TM_STMPE811_Orientation_Landscape_2;
			break;
		case TM_EMWIN_Rotate_180:
			result = GUI_SetOrientation(GUI_MIRROR_X | GUI_MIRROR_Y);
			td.orientation = TM_STMPE811_Orientation_Portrait_1;
			break;
		case TM_EMWIN_Rotate_270:
			result = GUI_SetOrientation(GUI_SWAP_XY | GUI_MIRROR_Y);
			td.orientation = TM_STMPE811_Orientation_Landscape_1;
			break;
		default:
			break;
	}
	
	/* Rotation was successfull? */
	if (result == 0) {
		/* Save new orientation */
		TS_Data.orientation = td.orientation;
		
		/* Return OK */
		return TM_EMWIN_Result_Ok;
	}
	
	/* Return error */
	return TM_EMWIN_Result_Error;
#endif
}

TM_EMWIN_Result_t TM_EMWIN_UpdateTouch(void) {
	static uint8_t millis = 0, last = 0;
	
	/* If we need to update it */
	if (++millis >= EMWIN_UPDATE_TOUCH_MILLIS) {
		/* Reset counter */
		millis = 0;
		
		/* Get data from touch */
		TM_STMPE811_ReadTouch(&TS_Data);
		
		/* Get data */
		TS_State.x = TS_Data.x;
		TS_State.y = TS_Data.y;
		
		/* Pressed */
		TS_State.Pressed = (TS_Data.pressed == TM_STMPE811_State_Pressed);
		
		/* Set layer */
		TS_State.Layer = 0;
		
		/* Update only if we have difference */
		if (
			TS_State.Pressed != last ||
			last
		) {
			/* Save new state */
			last = TS_State.Pressed;
			
			/* Update */
			GUI_TOUCH_StoreStateEx(&TS_State);
			
			/* Execute */
			GUI_Exec();
		}
	}
	
	/* Return OK */
	return TM_EMWIN_Result_Ok;
}

/* LTDC IRQ Handler */
void LTDC_IRQHandler(void) {
	LTDC_ISR_Handler();
}

/* DMA2D IRQ Handler */
void DMA2D_IRQHandler(void) {
	DMA2D_ISR_Handler();
}

