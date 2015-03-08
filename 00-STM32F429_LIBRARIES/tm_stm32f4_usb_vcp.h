/**
 *	USB Virtual COM Port for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/08/library-24-virtual-com-port-vcp-stm32f4xx/
 *	@version 	v1.2
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 *	
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
 *
 * Version 1.2
 *	- March 08, 2015
 *	- Added options to get user settings from terminal
 *	- Baudrate, stop bits, parity, data bits.
 *	- useful if you make USB->UART converter like FTDI
 *
 * Version 1.1
 *	- December 27, 2014
 *	- Added advanced functions for string operations
 *	- Now, Gets function will wait till buffer is full or \n is received
 *	- This is prevent for while loop if \n character is not received
 *
 *	With this library, your STM32F4xx will be seen to your computer as Virtual COM Port (VCP).
 *	To be able to work, you have to install ST's VCP Driver, from link below:
 *
 *		http://www.st.com/web/en/catalog/tools/PF257938
 *
 *	This library can work in 2 ways.
 *	First and default is Full-Speed mode, second option is High-Speed mode.
 *	Also, different modes have different pinouts.
 *
 *	In default settings, USB FS mode is selected.
 *
 *	STM32F4-Discovery has USB connected to FS mode, but
 *	STM32F429-Discovery has connected it to USB HS in FS mode.
 *	But if you have cable, like me, USB->4wires, you can connect Data+ and Data- to any pin on Discovery board.
 *	I did this, to check, if both mdoes work on bots discovery boards and yes, it worked.
 *	For security reasons set 22Ohm resistors in serial to your data pins.
 *
 * USB FS MODE (micro USB connected on STM32F4 Discovery board)
 *	- This is default option and don't need any special settings.
 *
 *	- Pinout
 *
 *		USB			STM32F4xx
 *		Data +		PA12
 *		Data -		PA11
 *
 * USB HS in FS mode (micro USB connected on STM32F429 Discovery board)
 *
 *	If you are working with STM32F429 Discovery board, and you want to use microUSB connector for VCP,
 *	then set define below in your defines.h file
 *
 *	//Activate USB HS in FS mode
 *	#define USE_USB_OTG_HS
 *
 *	- Pinout
 *
 *		USB			STM32F4xx
 *		Data +		PB15
 *		Data -		PB14
 *
 */
#ifndef TM_USB_VCP_H
#define TM_USB_VCP_H 	120
/**
 * Dependencies:
 * 	- STM32F4xx
 * 	- STM32F4xx RCC
 * 	- STM32F4xx GPIO
 * 	- STM32F4xx EXTI
 * 	- misc.h
 * 	- defines.h
 * 	- USB DEVICE
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "defines.h"
/* Parts of USB device */
#include "usbd_cdc_core.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

/* Default buffer length */
#ifndef USB_VCP_RECEIVE_BUFFER_LENGTH
#define USB_VCP_RECEIVE_BUFFER_LENGTH		128
#endif

/**
 * Enumerations
 */
typedef enum {
	TM_USB_VCP_OK,
	TM_USB_VCP_ERROR,
	TM_USB_VCP_RECEIVE_BUFFER_FULL,
	TM_USB_VCP_DATA_OK,
	TM_USB_VCP_DATA_EMPTY,
	TM_USB_VCP_NOT_CONNECTED,
	TM_USB_VCP_CONNECTED,
	TM_USB_VCP_DEVICE_SUSPENDED,
	TM_USB_VCP_DEVICE_RESUMED
} TM_USB_VCP_Result;

/**
 * Structure for USART if you are working USB/UART converter with STM32F4xx
 *
 * Parameters:
 *	- uint32_t Baudrate:
 *		Baudrate, which is set by user on terminal. Value is number of bits per second, for example: 115200
 *	- uint8_t Stopbits:
 *		Stop bits, which is set by user on terminal. 
 *		Values:
 *			- 0: 1 stop bit
 *			- 1: 1.5 stop bits
 *			- 2: 2 stop bits
 *	- uint8_t DataBits:
 *		Data bits, which is set by user on terminal.
 *		Values:
 *			- 5: 5 data bits
 *			- 6: 6 data bits
 *			- 7: 7 data bits
 *			- 8: 8 data bits
 *			- 9: 9 data bits
 *	- uint8_t Parity:
 *		Parity, which is set by user on terminal.
 *		Values:
 *			- 0: No parity
 *			- 1: Odd parity
 *			- 2: Even parity
 *			- 3: Mark parity
 *			- 4: Space parity
 *	- uint8_t Changed:
 *		When you check for settings in my function, this will be set to 1 if user has changed parameters,
 *		so you can reinitialize USART peripheral if you need to.
 */
typedef struct {
	uint32_t Baudrate;
	uint8_t Stopbits;
	uint8_t DataBits;
	uint8_t Parity;
	uint8_t Changed;
} TM_USB_VCP_Settings_t;

/**
 * Initialize USB VCP
 *
 * TM_USB_VCP_OK is returned
 */
extern TM_USB_VCP_Result TM_USB_VCP_Init(void);

/**
 * Read settings from user.
 * These settings are set in terminal.
 * 
 * Parameters:
 * 	- TM_USB_VCP_Settings_t* Settings:
 * 		Pointer to TM_USB_VCP_Settings_t structure where to save data
 * 
 * TM_USB_VCP_DATA_OK is returned
 */
extern TM_USB_VCP_Result TM_USB_VCP_GetSettings(TM_USB_VCP_Settings_t* Settings);

/**
 * Get received character from internal buffer
 *
 * Parameters:
 * 	- uint8_t* c: pointer to store new data
 *
 * TM_USB_VCP_DATA_OK is returned if data are valid, otherwise TM_USB_VCP_DATA_EMPTY
 */
extern TM_USB_VCP_Result TM_USB_VCP_Getc(uint8_t* c);

/**
 * Put character to USB VCP
 *
 * Parameters:
 * 	- volatile char c: character to send over USB
 *
 * TM_USB_VCP_OK is returned
 */
extern TM_USB_VCP_Result TM_USB_VCP_Putc(volatile char c);

/**
 * Get string from VCP port
 *
 * To use this method, you have to send \n (0x0D) at the end of your string,
 * otherwise data can be lost and you will fall in infinite loop.
 *
 * Parameters:
 * 	- char* buffer: pointer to buffer variable
 * 	- uint8_t bufsize: maximum buffer size
 *
 * Returns number of characters in string or 0 if not valid
 */
extern uint16_t TM_USB_VCP_Gets(char* buffer, uint16_t bufsize);

/**
 * Put string to USB VCP
 *
 * Parameters:
 * 	- char* str: pointer to string variable
 *
 * TM_USB_VCP_OK is returned
 */
extern TM_USB_VCP_Result TM_USB_VCP_Puts(char* str);

/**
 * Get VCP status
 *
 * Returns TM_USB_VCP_CONNECTED if device is configured and drivers are OK on computer
 */
extern TM_USB_VCP_Result TM_USB_VCP_GetStatus(void);

/**
 * This function checks if USB VCP buffer is empty
 *
 * Returns 1 if empty, 0 if not
 */
extern uint8_t TM_USB_VCP_BufferEmpty(void);

/**
 * This function checks if USB VCP buffer is full
 *
 * Returns 1 if full, 0 if not
 */
extern uint8_t TM_USB_VCP_BufferFull(void);

/**
 * This function checks if character is available in buffer
 * 
 * Parameters:
 *	- volatile char c: Character to be checked if available
 *
 * Returns 1 if available, 0 if not
 */
extern uint8_t TM_USB_VCP_FindCharacter(volatile char c);

/* Internal functions */
extern TM_USB_VCP_Result TM_INT_USB_VCP_AddReceived(uint8_t c);

#endif

