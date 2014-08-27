//--------------------------------------------------------------
// File     : usbh_usr.h
//--------------------------------------------------------------


#ifndef __USH_USR_H__
#define __USH_USR_H__


//--------------------------------------------------------------
// Includes
//-------------------------------------------------------------- 
#include "usbh_core.h"
#include <stdio.h>
#include "usbh_msc_core.h"

#include "tm_stm32f4_usb_msc_host.h"



/* This value can be equal to (512 * x) according to RAM size availability with x=[1, 128]
   In this project x is fixed to 64 => 512 * 64 = 32768bytes = 32 Kbytes */   
#define BUFFER_SIZE        ((uint16_t)512*64)  


#define USH_USR_FS_INIT    ((uint8_t)0x00)
#define USH_USR_FS_LOOP    ((uint8_t)0x01)

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern  USBH_Usr_cb_TypeDef USR_Callbacks;
/* Exported functions ------------------------------------------------------- */ 

void USBH_USR_Init(void);
void USBH_USR_DeviceAttached(void);
void USBH_USR_ResetDevice(void);
void USBH_USR_DeviceDisconnected (void);
void USBH_USR_OverCurrentDetected (void);
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed); 
void USBH_USR_Device_DescAvailable(void *);
void USBH_USR_DeviceAddressAssigned(void);
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc);
void USBH_USR_Manufacturer_String(void *);
void USBH_USR_Product_String(void *);
void USBH_USR_SerialNum_String(void *);
void USBH_USR_EnumerationDone(void);
USBH_USR_Status USBH_USR_UserInput(void);
int USBH_USR_MSC_Application(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceNotSupported(void);
void USBH_USR_UnrecoveredError(void);





#endif /*__USH_USR_H__*/


