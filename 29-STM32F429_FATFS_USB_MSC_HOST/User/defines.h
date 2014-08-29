#ifndef TM_DEFINES
#define TM_DEFINES

/* Place your custom library settings here */


/* Uncomment if you want to disable ID pin for USB MSC HOST library */
//#define USB_MSC_HOST_DISABLE_ID

/* Uncomment if you want to disable VBUS pin for USB MSC HOST library */
/* If you do this, on F4 and F429 Discovery boards USB will not work */
//#define USB_MSC_HOST_DISABLE_VBUS

/* Uncomment if you want to enable USB HS in FS mode */
/* By default, FS mode is used */
//#define USB_USE_OTG_HS

/* This line MUST BE ADDED in order to use FATFS with USB */
/* Otherwise you will have errors */
#define FATFS_USE_USB		1

/* By default, if you use USB, then SD card is disabled */
/* If you don't use USB, then SDIO is used by default! */
/* If you want to use both, USB and sdcard at the same time */
/* Enable SD card communication below: */

/* Enable SDIO communication */
#define FATFS_USE_SDIO		1
/* If you want SPI communication instead of SDIO, enable with */
//#define FATFS_USE_SDIO	0

#endif

