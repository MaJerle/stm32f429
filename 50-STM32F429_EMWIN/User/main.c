/**
 *	Keil project example for emWin
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_emwin.h"
/* GUI modules */
#include "button.h"
#include "DIALOG.h"

int main(void) {
	BUTTON_Handle hButton, hB1, hB2, hB3, hB4;
	PROGBAR_Handle hProgbar;
	uint8_t i;
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay functions */
	TM_DELAY_Init();
	
	/* Initialize LEDs */
	TM_DISCO_LedInit();
	
	/* Initialize emWin */
	if (TM_EMWIN_Init() != TM_EMWIN_Result_Ok) {
		/* Initialization error */
		while (1) {
			/* Toggle RED led */
			TM_DISCO_LedToggle(LED_RED);
			
			/* Delay */
			Delayms(100);
		}
	}
	
	/* Create progress bar at location x = 10, y = 10, length = 219, height = 30 */
	hProgbar = PROGBAR_CreateEx(10, 10, 219, 30, 0, WM_CF_SHOW, 0, GUI_ID_PROGBAR0);
	/* Set progress bar font */
	PROGBAR_SetFont(hProgbar, &GUI_Font8x16);
	/* Set progress bar text */
	PROGBAR_SetText(hProgbar, "LOADING..Please wait..");
	
	/* Imitate loading */
	for (i = 1; i <= 100; i++) {
		/* Set bar */
		PROGBAR_SetValue(hProgbar, i);
		/* Little delay, update value on LCD */
		GUI_Delay(20);
	}
	
	/* Create button with GUI_ID_OK ID number */
	hButton = BUTTON_CreateEx(10, 50, 219, 30, 0, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
	/* Set text and font */
	BUTTON_SetText(hButton, "Click me to continue..");
	BUTTON_SetFont(hButton, &GUI_Font8x15B_ASCII);
	
	/* Execute, show button */
	GUI_Exec();
	
	/* Wait till button pressed */
	while (1) {
		/* Check if button was pressed */
		if (GUI_GetKey() == GUI_ID_BUTTON0) {
			/* Led Off */
			TM_DISCO_LedOff(LED_GREEN);
			
			/* Stop while loop */
			break;
		}
		/* Toggle green led */
		TM_DISCO_LedToggle(LED_GREEN);
		/* Delay 100ms */
		GUI_Delay(100);
	}

	/* Delete button functionality */
	BUTTON_Delete(hButton);
	/* Delete button from LCD */
	GUI_ClearRect(10, 50, 269, 90);
	
	/* Create buttons for leds control */
	hB1 = BUTTON_CreateEx(10, 90, 105, 50, 0, WM_CF_SHOW, 0, GUI_ID_BUTTON1);
	hB2 = BUTTON_CreateEx(124, 90, 105, 50, 0, WM_CF_SHOW, 0, GUI_ID_BUTTON2);
	hB3 = BUTTON_CreateEx(10, 150, 105, 50, 0, WM_CF_SHOW, 0, GUI_ID_BUTTON3);
	hB4 = BUTTON_CreateEx(124, 150, 105, 50, 0, WM_CF_SHOW, 0, GUI_ID_BUTTON4);
	
	/* Set font for buttons */
	BUTTON_SetFont(hB1, &GUI_Font13HB_ASCII);
	BUTTON_SetFont(hB2, &GUI_Font13HB_ASCII);
	BUTTON_SetFont(hB3, &GUI_Font13HB_ASCII);
	BUTTON_SetFont(hB4, &GUI_Font13HB_ASCII);
	
	/* Set button text */
	BUTTON_SetText(hB1, "GREEN on");
	BUTTON_SetText(hB2, "GREEN off");
	BUTTON_SetText(hB3, "RED on");
	BUTTON_SetText(hB4, "RED off");
	
	/* Button styling */
	/* Background color when button is not pressed */
	BUTTON_SetBkColor(hB1, BUTTON_CI_UNPRESSED, GUI_DARKGREEN);
	/* Background color when button is pressed */
	BUTTON_SetBkColor(hB1, BUTTON_CI_PRESSED, GUI_GREEN);
	
	/* Background color when button is not pressed */
	BUTTON_SetBkColor(hB3, BUTTON_CI_UNPRESSED, GUI_DARKRED);
	/* Background color when button is pressed */
	BUTTON_SetBkColor(hB3, BUTTON_CI_PRESSED, GUI_RED);
	
	/* Show buttons */
	GUI_Exec();
	
	while (1) {
		/* Get pressed key */
		switch (GUI_GetKey()) {
			case GUI_ID_BUTTON1:
				/* Button 1 pressed */
				TM_DISCO_LedOn(LED_GREEN);
				break;
			case GUI_ID_BUTTON2:
				/* Button 2 pressed */
				TM_DISCO_LedOff(LED_GREEN);
				break;
			case GUI_ID_BUTTON3:
				/* Button 3 pressed */
				TM_DISCO_LedOn(LED_RED);
				break;
			case GUI_ID_BUTTON4:
				/* Button 4 pressed */
				TM_DISCO_LedOff(LED_RED);
				break;
			default:
				break;
		}
	}
}

/* User handler for 1ms interrupts */
void TM_DELAY_1msHandler(void) {
	/* Call periodically each 1ms */
	TM_EMWIN_UpdateTouch();
}
