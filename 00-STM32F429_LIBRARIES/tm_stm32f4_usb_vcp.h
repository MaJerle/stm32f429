/**
 *	USB Virtual COM Port for STM32F4xx devices
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2014/08/library-24-virtual-com-port-vcp-stm32f4xx/
 *	@version 	v1.0
 *	@ide		Keil uVision
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
#define TM_USB_VCP_H 	100
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

/* USB VCP Internal receive buffer */
extern uint8_t TM_INT_USB_VCP_ReceiveBuffer[USB_VCP_RECEIVE_BUFFER_LENGTH];

/**
 * Initialize USB VCP
 *
 * TM_USB_VCP_OK is returned
 */
extern TM_USB_VCP_Result TM_USB_VCP_Init(void);

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
 * TM_USB_VCP_DATA_OK is returned if data are valid, otherwise TM_USB_VCP_DATA_EMPTY
 */
extern TM_USB_VCP_Result TM_USB_VCP_Gets(char* buffer, uint8_t bufsize);

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

/* Internal functions */
extern TM_USB_VCP_Result TM_INT_USB_VCP_AddReceived(uint8_t c);

#endif

