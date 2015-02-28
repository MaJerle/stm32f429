/**
 *	Coocox project template for ethernet DHCP
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Coocox
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_ethernet.h"
#include "tm_stm32f4_watchdog.h"
#include "tm_stm32f4_id.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	uint8_t i;
	uint8_t mac_address[6];
	uint8_t ip_address[] = {192, 168, 0, 150};

	/* Initialize system */
	SystemInit();

	/* Init USART6, TX: PC6 for debug */
	TM_USART_Init(USART6, TM_USART_PinsPack_1, 115200);

	/* Enable watchdog, 4 seconds before timeout */
	if (TM_WATCHDOG_Init(TM_WATCHDOG_Timeout_4s)) {
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

	/* Set MAC address from unique ID */
	for (i = 0; i < 6; i++) {
		/* Set MAC addr */
		mac_address[i] = TM_ID_GetUnique8(11 - i);
	}

	/* Initialize ethernet peripheral */
	/* Set MAC address, set IP address which will be used in case DHCP can't get IP from router */
	if (TM_ETHERNET_Init(mac_address, ip_address, NULL, NULL) == TM_ETHERNET_Result_Ok) {
		/* Successfully initialized */
		TM_DISCO_LedOn(LED_GREEN);
	} else {
		/* Unsuccessfull communication */
		TM_DISCO_LedOn(LED_RED);
	}

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

void TM_ETHERNET_DHCPStartCallback(void) {
	/* Print to user */
	printf("DHCP has started with assigning IP address\n");
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
