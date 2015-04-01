/**
 *	Keil project template for ethernet server.
 *	Data to be shown to user by default are stored in file fsdata.c where you can edit it if you want.
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@conf		PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_ethernet.h"
#include "tm_stm32f4_rtc.h"
#include "tm_stm32f4_watchdog.h"
#include <stdio.h>
#include <stdlib.h>

/* RTC data variable */
TM_RTC_t RTC_Data;

/* Set SSI tags for handling variables */
static TM_ETHERNET_SSI_t SSI_Tags[] = {
	"led1_s", /* Tag 0 = led1 status */
	"led2_s", /* Tag 1 = led2 status */
	"led3_s", /* Tag 2 = led3 status */
	"led4_s", /* Tag 3 = led4 status */
	"srv_adr",/* Tag 4 = server address */
	"clt_a_c",/* Tag 5 = client all connections */
	"clt_s_c",/* Tag 6 = client successfull connections */
	"clt_per",/* Tag 7 = client percentage */
	"clt_tx", /* Tag 8 = client TX bytes */
	"clt_rx", /* Tag 9 = client RX bytes */
	"srv_c",  /* Tag 10 = server all connections */
	"srv_tx", /* Tag 11 = server TX bytes */
	"srv_rx", /* Tag 12 = server RX bytes */
	"mac_adr",/* Tag 13 = MAC address */
	"gateway",/* Tag 14 = gateway */
	"netmask",/* Tag 15 = netmask */
	"link",   /* Tag 16 = link status */
	"duplex", /* Tag 17 = duplex status */
	"hardware",/* Tag 18 = hardware where code is running */
	"rtc_time",/* Tag 19 = current RTC time */
	"compiled",/* Tag 20 = compiled date and time */
};

/* LED CGI handler */
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

/* CGI call table, only one CGI used */
TM_ETHERNET_CGI_t CGI_Handlers[] = {
	{"/ledaction.cgi", LEDS_CGI_Handler}, /* LEDS_CGI_Handler will be called when user connects to "/ledaction.cgi" URL */
};

int main(void) {
	/* Initialize system */
	SystemInit();
	
	/* Init USART6, TX: PC6 for debug */
	TM_USART_Init(USART6, TM_USART_PinsPack_1, 115200);
	
	/* Enable watchdog, 4 seconds before timeout */
	if (TM_WATCHDOG_Init(TM_WATCHDOG_Timeout_16s)) {
		/* Report to user */
		printf("Reset occured because of Watchdog\n");
	}
	
	/* Initialize delay */
	TM_DELAY_Init();
	
	/* Initialize leds on board */
	TM_DISCO_LedInit();
	
	/* Initialize button */
	TM_DISCO_ButtonInit();
	
	/* Display to user */
	printf("Program starting..\n");
	
	/* Initialize RTC with internal clock if not already */
	if (!TM_RTC_Init(TM_RTC_ClockSource_Internal)) {
		/* Set default time for RTC */
		
		/* Set date and time if RTC is not initialized already */
		TM_RTC_SetDateTimeString("28.02.15.6;23:35:30");
	};
	
	/* Initialize ethernet peripheral */
	/* All parameters NULL, default options for MAC, static IP, gateway and netmask will be used */
	/* They are defined in tm_stm32f4_ethernet.h file */
	if (TM_ETHERNET_Init(NULL, NULL, NULL, NULL) == TM_ETHERNET_Result_Ok) {
		/* Successfully initialized */
		TM_DISCO_LedOn(LED_GREEN);
	} else {
		/* Unsuccessfull communication */
		TM_DISCO_LedOn(LED_RED);
	}
	
	/* Reset watchdog */
	TM_WATCHDOG_Reset();
	
	/* Initialize ethernet server if you want use it, server port 80 */
	TM_ETHERNETSERVER_Enable(80);
	
	/* Set SSI tags, we have 21 SSI tags */
	TM_ETHERNETSERVER_SetSSITags(SSI_Tags, 21);
	
	/* Set CGI tags, we have 1 CGI handler, for leds only */
	TM_ETHERNETSERVER_SetCGIHandlers(CGI_Handlers, 1);
	
	/* Read RTC clock */
	TM_RTC_GetDateTime(&RTC_Data, TM_RTC_Format_BIN);
	
	/* Print current time to USART */
	printf("Current date: %02d:%02d:%02d\n", RTC_Data.hours, RTC_Data.minutes, RTC_Data.seconds);
	
	/* Reset watchdog */
	TM_WATCHDOG_Reset();

	while (1) {
		/* Update ethernet, call this as fast as possible */
		TM_ETHERNET_Update();
		
		/* Reset watchdog */
		TM_WATCHDOG_Reset();
	}
}

/* Delay 1ms handler */
void TM_DELAY_1msHandler(void) {
	/* Time update for ethernet, 1ms */
	/* Add 1ms time for ethernet */
	TM_ETHERNET_TimeUpdate(1);
}

/* Handle CGI request for LEDS */
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
	uint8_t i;
	
	/* This function handles request like one below: */
	/* /ledaction.cgi?ledtoggle=1&ledoff=2 */
	/* This will toggle LED 1 and turn off LED 2 */
	
	/* Callback */
	if (iIndex == 0) {
		/* Go through all params */
		for (i = 0; i < iNumParams; i++) {
			/* If current pair = ledtoggle=someled */
			if (strstr(pcParam[i], "ledtoggle")) {
				/* Switch first character */
				switch (pcValue[i][0]) {
					case '1':
						TM_DISCO_LedToggle(LED_GREEN);
						break;
					case '2':
						TM_DISCO_LedToggle(LED_ORANGE);
						break;
					case '3':
						TM_DISCO_LedToggle(LED_RED);
						break;
					case '4':
						TM_DISCO_LedToggle(LED_BLUE);
						break;
					default:
						break;
				}
			} else if (strstr(pcParam[i], "ledon")) {
				switch (pcValue[i][0]) {
					case '1':
						TM_DISCO_LedOn(LED_GREEN);
						break;
					case '2':
						TM_DISCO_LedOn(LED_ORANGE);
						break;
					case '3':
						TM_DISCO_LedOn(LED_RED);
						break;
					case '4':
						TM_DISCO_LedOn(LED_BLUE);
						break;
					default:
						break;
				}
			} else if (strstr(pcParam[i], "ledoff")) {
				switch (pcValue[i][0]) {
					case '1':
						TM_DISCO_LedOff(LED_GREEN);
						break;
					case '2':
						TM_DISCO_LedOff(LED_ORANGE);
						break;
					case '3':
						TM_DISCO_LedOff(LED_RED);
						break;
					case '4':
						TM_DISCO_LedOff(LED_BLUE);
						break;
					default:
						break;
				}
			}
		}
	}
	
	/* Return URL to be used after call */
	return "/index.shtml";
}

/* SSI server callback, always is called this callback */
uint16_t TM_ETHERNETSERVER_SSICallback(int iIndex, char *pcInsert, int iInsertLen) {
	uint8_t status;
	
	/* Return number of characters written */
	if (iIndex < 4) {
		/* First 4 tags are leds */
		/* Get led status */
		switch (iIndex) {
			case 0:
				/* Green LED */
				status = TM_DISCO_LedIsOn(LED_GREEN);
				break;
			case 1:
				/* Orange LED */
				status = TM_DISCO_LedIsOn(LED_ORANGE);
				break;
			case 2:
				/* Red LED */
				status = TM_DISCO_LedIsOn(LED_RED);
				break;
			case 3:
				/* Blue LED */
				status = TM_DISCO_LedIsOn(LED_BLUE);
				break;
			default:
				return 0;
		}
		
		/* Set string according to status */
		if (status) {
			/* Led is ON */
			sprintf(pcInsert, "<span class=\"green\">On</span>");
		} else {
			/* Led is OFF */
			sprintf(pcInsert, "<span class=\"red\">Off</span>");
		}
	} else if (iIndex == 4) {
		/* #serv_adr tag is requested */
		sprintf(pcInsert, "%d.%d.%d.%d", TM_ETHERNET_GetLocalIP(0), TM_ETHERNET_GetLocalIP(1), TM_ETHERNET_GetLocalIP(2), TM_ETHERNET_GetLocalIP(3));
	} else if (iIndex == 5) {
		/* #clt_a_c tag */
		sprintf(pcInsert, "%u", TM_ETHERNETCLIENT_GetConnectionsCount());
	} else if (iIndex == 6) {
		/* #clt_s_c tag */
		sprintf(pcInsert, "%u", TM_ETHERNETCLIENT_GetSuccessfullConnectionsCount());
	} else if (iIndex == 7) {
		/* #clt_per tag */
		if (TM_ETHERNETCLIENT_GetConnectionsCount() == 0) {
			strcpy(pcInsert, "0 %");
		} else {
			sprintf(pcInsert, "%f %%", (float)TM_ETHERNETCLIENT_GetSuccessfullConnectionsCount() / (float)TM_ETHERNETCLIENT_GetConnectionsCount() * 100);
		}
	} else if (iIndex == 8) {
		/* #clt_tx tag */
		sprintf(pcInsert, "%llu", TM_ETHERNETCLIENT_GetTXBytes());
	} else if (iIndex == 9) {
		/* #clt_rx tag */
		sprintf(pcInsert, "%llu", TM_ETHERNETCLIENT_GetRXBytes());
	} else if (iIndex == 10) {
		/* #srv_c tag */
		sprintf(pcInsert, "%u", TM_ETHERNETSERVER_GetConnectionsCount());
	} else if (iIndex == 11) {
		/* #srv_tx tag */
		sprintf(pcInsert, "%llu", TM_ETHERNETSERVER_GetTXBytes());
	} else if (iIndex == 12) {
		/* #srv_rx tag */
		sprintf(pcInsert, "%llu", TM_ETHERNETSERVER_GetRXBytes());
	} else if (iIndex == 13) {
		/* #mac_adr */
		sprintf(pcInsert, "%02X-%02X-%02X-%02X-%02X-%02X",
			TM_ETHERNET_GetMACAddr(0),
			TM_ETHERNET_GetMACAddr(1),
			TM_ETHERNET_GetMACAddr(2),
			TM_ETHERNET_GetMACAddr(3),
			TM_ETHERNET_GetMACAddr(4),
			TM_ETHERNET_GetMACAddr(5)
		);
	} else if (iIndex == 14) {
		/* #gateway */
		sprintf(pcInsert, "%d.%d.%d.%d",
			TM_ETHERNET_GetGateway(0),
			TM_ETHERNET_GetGateway(1),
			TM_ETHERNET_GetGateway(2),
			TM_ETHERNET_GetGateway(3)
		);
	} else if (iIndex == 15) {
		/* #netmask */
		sprintf(pcInsert, "%d.%d.%d.%d",
			TM_ETHERNET_GetNetmask(0),
			TM_ETHERNET_GetNetmask(1),
			TM_ETHERNET_GetNetmask(2),
			TM_ETHERNET_GetNetmask(3)
		);
	} else if (iIndex == 16) {
		/* #link */
		if (TM_ETHERNET_Is100M()) {
			strcpy(pcInsert, "100Mbit");
		} else {
			strcpy(pcInsert, "10Mbit");
		}
	} else if (iIndex == 17) {
		/* #duplex */
		if (TM_ETHERNET_IsFullDuplex()) {
			strcpy(pcInsert, "Full");
		} else {
			strcpy(pcInsert, "Half");
		}
	} else if (iIndex == 18) {
		/* #hardware */
		strcpy(pcInsert, "STM32F4-Discovery");
	} else if (iIndex == 19) {
		/* #rtc_time */
		TM_RTC_GetDateTime(&RTC_Data, TM_RTC_Format_BIN);
		sprintf(pcInsert, "%04d-%02d-%02d %02d:%02d:%02d",
			RTC_Data.year + 2000,
			RTC_Data.month,
			RTC_Data.date,
			RTC_Data.hours,
			RTC_Data.minutes,
			RTC_Data.seconds
		);
	} else if (iIndex == 20) {
		/* #compiled */
		strcpy(pcInsert, __DATE__ " at " __TIME__);
	} else {
		/* No valid tag */
		return 0;
	}
	
	/* Return number of characters written in buffer */
	return strlen(pcInsert);
}

void TM_ETHERNET_IPIsSetCallback(uint8_t ip_addr1, uint8_t ip_addr2, uint8_t ip_addr3, uint8_t ip_addr4, uint8_t dhcp) {
	/* Called when we have valid IP, it might be static or DHCP */
	
	if (dhcp) {
		/* IP set with DHCP */
		printf("IP: %d.%d.%d.%d assigned by DHCP server\n", ip_addr1, ip_addr2, ip_addr3, ip_addr4);
	} else {
		/* Static IP */
		printf("IP: %d.%d.%d.%d; STATIC IP used\n", ip_addr1, ip_addr2, ip_addr3, ip_addr4);
	}
	
	/* Print MAC address to user */
	printf("MAC: %02X-%02X-%02X-%02X-%02X-%02X\n",
		TM_ETHERNET_GetMACAddr(0),
		TM_ETHERNET_GetMACAddr(1),
		TM_ETHERNET_GetMACAddr(2),
		TM_ETHERNET_GetMACAddr(3),
		TM_ETHERNET_GetMACAddr(4),
		TM_ETHERNET_GetMACAddr(5)
	);
	/* Print netmask to user */
	printf("Netmask: %d.%d.%d.%d\n",
		TM_ETHERNET_GetGateway(0),
		TM_ETHERNET_GetGateway(1),
		TM_ETHERNET_GetGateway(2),
		TM_ETHERNET_GetGateway(3)
	);
	/* Print gateway to user */
	printf("Gateway: %d.%d.%d.%d\n",
		TM_ETHERNET_GetNetmask(0),
		TM_ETHERNET_GetNetmask(1),
		TM_ETHERNET_GetNetmask(2),
		TM_ETHERNET_GetNetmask(3)
	);
	/* Print 100M link status, 1 = 100M, 0 = 10M */
	printf("Link 100M: %d\n", TM_ETHERNET.speed_100m);
	/* Print duplex status: 1 = Full, 0 = Half */
	printf("Full duplex: %d\n", TM_ETHERNET.full_duplex);
}

void TM_ETHERNET_LinkIsDownCallback(void) {
	/* This function will be called when ethernet cable will not be plugged */
	/* It will also be called on initialization if connection is not detected */
	
	/* Print to user */
	printf("Link is down, do you have connected to your modem/router?\n");
}
void TM_ETHERNET_LinkIsUpCallback(void) {
	/* Cable has been plugged in back, link is detected */
	/* I suggest you that you reboot MCU here */
	/* Do important stuff before */
	
	/* Print to user */
	printf("Link is up back\n");
}

/* For printf function */
int fputc(int ch, FILE *f) {
	/* Send over usart */
	TM_USART_Putc(USART6, ch);

	/* Return character back */
	return ch;
}
