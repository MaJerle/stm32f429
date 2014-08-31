#ifndef TM_DEFINES
#define TM_DEFINES

/* Place your custom library settings */

/* Enable if you want t use USB HS in FS mode */
/* For "STM32F429-Discovery Target" I have done this define in preprocessor defines */
/* Under "Options for target" -> "C/C++" -> "Define" */
//#define USE_USB_OTG_HS	/* STM32F429-Discovery */

/* Uncomment if you want to disable ID pin for USB HID HOST library */
#define USB_HID_HOST_DISABLE_ID

/* Uncomment if you want to disable VBUS pin for USB HID HOST library */
/* If you do this, on F4 and F429 Discovery boards USB will not work */
//#define USB_HID_HOST_DISABLE_VBUS

#endif

