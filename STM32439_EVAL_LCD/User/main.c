/**
 * Keil project template
 *
 * Before you start, select your target, on the right of the "Load" button
 *
 * @author    Tilen Majerle
 * @email     tilen@majerle.eu
 * @website   http://stm32f4-discovery.com
 * @ide       Keil uVision 5
 * @conf      PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 * @packs     STM32F4xx Keil packs version 2.4.0 or greater required
 * @stdperiph STM32F4xx Standard peripheral drivers version 1.5.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_lcd.h"
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_usart.h"
#include "stdio.h"
#include "tm_stm32f4_dma2d_graphic.h"
#include "GUI.h"

#include "PROGBAR.h"
#include "BUTTON.h"
#include "GRAPH.h"
#include "KNOB.h"


// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0        (GUI_ID_USER + 0x00)
#define ID_HEADER_0        (GUI_ID_USER + 0x01)
#define ID_MENU_0        (GUI_ID_USER + 0x02)
#define ID_GRAPH_0        (GUI_ID_USER + 0x03)
#define ID_BUTTON_0        (GUI_ID_USER + 0x04)
#define ID_LISTWHEEL_0        (GUI_ID_USER + 0x05)
#define ID_PROGBAR_0        (GUI_ID_USER + 0x06)
#define ID_TEXT_0        (GUI_ID_USER + 0x07)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 640, 480, 0, 0x0, 0 },
  { HEADER_CreateIndirect, "Header", ID_HEADER_0, 0, 0, 640, 20, 0, 0x0, 0 },
  { MENU_CreateIndirect, "Menu", ID_MENU_0, 0, 20, 640, 20, 0, 0x0, 0 },
  //{ GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 50, 240, 300, 100, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Open Window", ID_BUTTON_0, 136, 101, 295, 113, 0, 0x0, 0 },
  { LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_0, 538, 108, 66, 35, 0, 0x0, 0 },
  { PROGBAR_CreateIndirect, "Progbar", ID_PROGBAR_0, 0, 0, 100, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 560, 3, 80, 20, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  MENU_ITEM_DATA   ItemData;
  WM_HWIN          hItem;
  int              NCode;
  int              Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Menu'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MENU_0);
    ItemData.Flags    = 0;
    ItemData.hSubmenu = 0;
    ItemData.Id       = 0;
    ItemData.pText    = "File";
    MENU_AddItem(hItem, &ItemData);
    ItemData.Flags    = 0;
    ItemData.hSubmenu = 0;
    ItemData.Id       = 1;
    ItemData.pText    = "View";
    MENU_AddItem(hItem, &ItemData);
    ItemData.Flags    = 0;
    ItemData.hSubmenu = 0;
    ItemData.Id       = 2;
    ItemData.pText    = "Edit";
    MENU_AddItem(hItem, &ItemData);
    ItemData.Flags    = 0;
    ItemData.hSubmenu = 0;
    ItemData.Id       = 3;
    ItemData.pText    = "Help";
    MENU_AddItem(hItem, &ItemData);
    //
    // Initialization of 'Graph'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
    GRAPH_SetBorder(hItem, 0, 0, 0, 0);
    //
    // Initialization of 'Open Window'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, GUI_FONT_32B_ASCII);
    //
    // Initialization of 'Listwheel'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0);
    LISTWHEEL_AddString(hItem, "A");
    LISTWHEEL_AddString(hItem, "B");
    LISTWHEEL_AddString(hItem, "C");
    LISTWHEEL_AddString(hItem, "D");
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_HEADER_0: // Notifications sent by 'Header'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_MENU_0: // Notifications sent by 'Menu'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_0: // Notifications sent by 'Open Window'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        /* Start ethernet connections */
		
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_LISTWHEEL_0: // Notifications sent by 'Listwheel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN hWin;
WM_HWIN CreateWindow(void);
WM_HWIN CreateWindow(void) {

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/


GRAPH_DATA_Handle graph_data = 5;
GRAPH_SCALE_Handle graph_scale;


/* LCD I2C address */
#define LCD_ADDR     0x80

uint8_t DataSend[] = {
	0x00, 0x81, 0x08
};

uint8_t DataReceive[11];

PROGBAR_Handle hProgbar;
BUTTON_Handle hButton;
GRAPH_Handle hGraph;
KNOB_Handle hKnob;
GUI_MEMDEV_Handle hMemdev;

/* Callback functions for timers */
static void TOUCH_Timer_CB(void);
static void PROGBAR_Timer_CB(void);

TM_DELAY_Timer_t* TIM_Touch;
TM_DELAY_Timer_t* TIM_PROGBAR;

GRAPH_DATA_Handle hData;
GRAPH_SCALE_Handle hScale;
int16_t aData0[] = {
	0,1,2,3,4,5,6,7,8,9,10,11,10,12,13,14,15,16,17
};
#include "math.h"

int main(void) {
	uint8_t i;
	char buff[100];
	
	/* Initialize system */
	SystemInit();
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	
	/* Init USART for debug */
	TM_USART_Init(USART1, TM_USART_PinsPack_1, 921600);
	
	/* Init LCD */
	TM_LCD_Init();
	
	/* Enable CRC for EMWIN */
	RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
	
	/* Init I2C */
	TM_I2C_Init(I2C1, TM_I2C_PinsPack_3, 100000);
	
	/* Create timers */
	TIM_Touch = TM_DELAY_TimerCreate(50, 1, 1, TOUCH_Timer_CB, NULL);
	//TIM_PROGBAR = TM_DELAY_TimerCreate(50, 1, 1, PROGBAR_Timer_CB);
	
	/* Init EMWIN GUI */
	GUI_Init();
	
	/* Create EMWIN window from GUI builder */
	//CreateWindow();
	
	/* Show window */
	GUI_Exec();
	
	/* Create graph */
	hGraph = GRAPH_CreateEx(0, 0, 640, 480, 0, WM_CF_SHOW, 0, ID_GRAPH_0);
	hData = GRAPH_DATA_YT_Create(GUI_DARKGREEN, 500, 0, 20);
	GRAPH_SetGridVis(hGraph, 1);
    GRAPH_SetBorder(hGraph, 25, 5, 5, 5); 
    GRAPH_AttachData(hGraph, hData);
	graph_scale = GRAPH_SCALE_Create(3, GUI_TA_LEFT, GRAPH_SCALE_CF_VERTICAL, 25);
	GRAPH_AttachScale(hGraph, graph_scale);
	
	/* Execute pending */
	GUI_Exec();
	
	TM_LCD_ChangeLayers();
	
	while (1) {
		/* Add new value */
		GRAPH_DATA_YT_AddValue(hData, 100 + 60 * sin((float)2 * (float)3.14 * (float)5 * (float)i++ / (float)0xFF));
		GRAPH_DATA_YT_AddValue(hData, 100 + 60 * sin((float)2 * (float)3.14 * (float)5 * (float)i++ / (float)0xFF));
		GRAPH_DATA_YT_AddValue(hData, 100 + 60 * sin((float)2 * (float)3.14 * (float)5 * (float)i++ / (float)0xFF));
		
		/* Execute pending GUI tasks */
		GUI_Exec();
		TM_LCD_Layer1To2();
		
		TM_DISCO_LedToggle(LED_RED);
		
		//Delayms(50);
	}
}

GUI_PID_STATE GUI_TS;

static void TOUCH_Timer_CB(void) {
	static uint8_t TouchState = 0;

	uint16_t X1, X2, Y1, Y2;
	
	/* Write to I2C first */
	TM_I2C_WriteMultiNoRegister(I2C1, LCD_ADDR, DataSend, sizeof(DataSend));

	/* Read from device */
	TM_I2C_ReadMulti(I2C1, LCD_ADDR, 0x8A, DataReceive, sizeof(DataReceive));
	
	/* Format coordinates */
	X1 = DataReceive[1] << 8 | DataReceive[2];
	Y1 = DataReceive[3] << 8 | DataReceive[4];
	X2 = DataReceive[5] << 8 | DataReceive[6];
	Y2 = DataReceive[7] << 8 | DataReceive[8];
	
	/* Save last value */
	TouchState = GUI_TS.Pressed;
	
	/* Get touch state */
	GUI_TS.Pressed = (X1 != 0xFFFF && Y1 != 0xFFFF);
	GUI_TS.x = X1;
	GUI_TS.y = Y1;
	
	if (GUI_TS.Pressed != TouchState) {
		/* Store touch data */
		GUI_TOUCH_StoreStateEx(&GUI_TS);
		
		/* Execute */
		GUI_Exec();
	}
}
#include "math.h"
static void PROGBAR_Timer_CB(void) {
	static uint8_t i = 0;
	static uint8_t c = 0;
	
	/* Add value to graph */
	GRAPH_DATA_YT_AddValue(hData, 100 + 60 * sin((float)2 * (float)5 * (float)3.14 * (float)i++ / (float)255));
}

int fputc(int ch, FILE* fil) {
	TM_USART_Putc(USART1, ch);
	
	return ch;
}
