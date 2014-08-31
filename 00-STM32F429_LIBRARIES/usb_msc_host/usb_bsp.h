#ifndef __USB_BSP__H__
#define __USB_BSP__H__

#include "usb_core.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"


//#define USE_ACCURATE_TIME
#define TIM_MSEC_DELAY						0x01
#define TIM_USEC_DELAY						0x02

// Power control
#ifdef USE_USB_OTG_FS
	//F4-Discovery board
	#define HOST_POWERSW_PORT_RCC			RCC_AHB1Periph_GPIOC
	#define HOST_POWERSW_PORT				GPIOC
	#define HOST_POWERSW_VBUS				GPIO_Pin_0
#else
	//F429-Discovery board
	#define HOST_POWERSW_PORT_RCC			RCC_AHB1Periph_GPIOC
	#define HOST_POWERSW_PORT				GPIOC
	#define HOST_POWERSW_VBUS				GPIO_Pin_4
#endif

void USB_OTG_BSP_Init (USB_OTG_CORE_HANDLE *pdev);
void USB_OTG_BSP_uDelay (const uint32_t usec);
void USB_OTG_BSP_mDelay (const uint32_t msec);
void USB_OTG_BSP_EnableInterrupt (USB_OTG_CORE_HANDLE *pdev);

void USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev);
void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev,uint8_t state);



#endif


