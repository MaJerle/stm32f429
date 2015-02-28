/**
 *	Keil project template for ethernet client and DNS.
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
#include "tm_stm32f4_watchdog.h"
#include <stdio.h>
#include <stdlib.h>

/* Create simple typedef for DNS controlling */
typedef struct {
	uint8_t Working;
	uint8_t HaveIP;
	uint8_t ip[4];
} TM_DNS_t;
TM_DNS_t MyDNS;

uint16_t requests_count = 1;

int main(void) {
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

	while (1) {
		/* Update ethernet, call this as fast as possible */
		TM_ETHERNET_Update();
		
		/* If DNS is not working and we don't have IP yet */
		if (MyDNS.Working == 0 && MyDNS.HaveIP == 0) {
			/* Try to start DNS */
			/* It will return error in case Ethernet is not ready yet so you have to try more than one time */
			if (TM_ETHERNETDNS_GetHostByName("stm32f4-discovery.com") == TM_ETHERNET_Result_Ok) {
				/* We started with working */
				MyDNS.Working = 1;
			}
		}
		
		/* On button pressed, make a new connection and if we have IP known */
		if (TM_DISCO_ButtonOnPressed() && MyDNS.HaveIP) {
			/* Try to make a new connection, port 80 */
			if (TM_ETHERNETCLIENT_Connect("stm32f4-discovery.com", MyDNS.ip[0], MyDNS.ip[1], MyDNS.ip[2], MyDNS.ip[3], 80, &requests_count) != TM_ETHERNET_Result_Ok) {
				/* Print to user */
				printf("Can not make a new connection!\n");
			}
		}
		
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

uint16_t TM_ETHERNETCLIENT_CreateHeadersCallback(TM_TCPCLIENT_t* connection, char* buffer, uint16_t buffer_length) {
	/* Create request headers */
	sprintf(buffer, "GET /hello_world.php?number=%d HTTP/1.1\r\n", *(uint16_t *)connection->user_parameters);
	strcat(buffer, "Host: stm32f4-discovery.com\r\n");
	strcat(buffer, "Connection: close\r\n");
	strcat(buffer, "\r\n");
	
	/* Return number of bytes in buffer */
	return strlen(buffer);
}

void TM_ETHERNETCLIENT_ReceiveDataCallback(TM_TCPCLIENT_t* connection, uint8_t* buffer, uint16_t buffer_length, uint16_t total_length) {
	uint16_t i = 0;
	
	/* We have available data for connection to receive */
	printf("Receiveing %d bytes of data from %s\n", buffer_length, connection->name);
	
	/* Go through entire buffer, remove response headers */
	if (connection->headers_done == 0) {
		for (i = 0; i < buffer_length; i++) {
			if (
				buffer[i] == '\r' &&
				buffer[i + 1] == '\n' &&
				buffer[i + 2] == '\r' &&
				buffer[i + 3] == '\n'
			) {
				/* Headers done */
				connection->headers_done = 1;
				/* Increase i */
				i += 3;
				/* Break */
				break;
			}
		}
	}
	
	/* Print data */
	for (; i < buffer_length; i++) {
		/* Print response */
		printf("%c", buffer[i]);
	}
}

void TM_ETHERNETCLIENT_ConnectedCallback(TM_TCPCLIENT_t* connection) {
	/* We are connected */
	printf("We are connected to %s\n", connection->name);
}

void TM_ETHERNETCLIENT_ConnectionClosedCallback(TM_TCPCLIENT_t* connection, uint8_t success) {
	/* We are disconnected, done with connection */
	if (success) {
		printf("Connection %s was successfully closed. Number of active connections: %d\n", connection->name, *connection->active_connections_count);
	} else {
		printf("Connection %s was closed because of error. Number of active connections: %d\n", connection->name, *connection->active_connections_count);
	}
	
	/* Increase number of requests */
	requests_count++;
}

void TM_ETHERNETCLIENT_ErrorCallback(TM_TCPCLIENT_t* connection) {
	/* Print to user */
	printf("An error occured on connection %s\n", connection->name);
}

void TM_ETHERNETCLIENT_ConnectionStartedCallback(TM_TCPCLIENT_t* connection) {
	/* Print to user */
	printf("Connection %s has started\n", connection->name);
}

/* DNS has IP */
void TM_ETHERNETDNS_FoundCallback(char* host_name, uint8_t ip_addr1, uint8_t ip_addr2, uint8_t ip_addr3, uint8_t ip_addr4) {
	/* If host name is stm32f4-discovery.com */
	if (strstr(host_name, "stm32f4-discovery.com")) {
		/* We have IP */
		MyDNS.HaveIP = 1;
		/* Save IP */
		MyDNS.ip[0] = ip_addr1;
		MyDNS.ip[1] = ip_addr2;
		MyDNS.ip[2] = ip_addr3;
		MyDNS.ip[3] = ip_addr4;
		/* We are not working anymore */
		MyDNS.Working = 0;
		
		/* Print to user */
		printf("We have IP address for %s: %d.%d.%d.%d\n", host_name, ip_addr1, ip_addr2, ip_addr3, ip_addr4);
	}
}

/* DNS error callback */
void TM_ETHERNETDNS_ErrorCallback(char* host_name) {
	/* If host name is stm32f4-discovery.com */
	if (strstr(host_name, "stm32f4-discovery.com")) {
		/* We have IP */
		MyDNS.HaveIP = 0;
		/* We are not working anymore */
		MyDNS.Working = 0;
	}
	/* Print to user */
	printf("DNS has failed to get IP address for %s\n", host_name);
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
	printf("Link is down, do you have connected to your modem/router?");
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
