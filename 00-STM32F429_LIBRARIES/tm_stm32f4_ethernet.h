/**
 *	ETHERNET library for STM32F4xx devices with built-in Ethernet controller
 *	and external PHY with RMII connnection (tested with DP83848)
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@link		http://stm32f4-discovery.com/2015/02/library-52-ethernet-peripheral-on-stm32f4xx/
 *	@version 	v1.0
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2015
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
 * Pinout
 *	Currently, only RMII mode is available with 2 possible pins pack
 *	By default, PP1 (PinsPack 1) is used. If you want to use PP2, then you need
 *	define in defines.h file:
 *	
 *	//Enable PinsPack 2 for RMII ethernet:
 *	#define ETHERNET_RMII_PINSPACK_2
 *
 *	ETH PHY			STM32F4xx with ethernet
 *					|PP1		|PP2
 *	
 *	MDIO			|PA2		|PA2
 *	MDC				|PC1		|PC1
 *	REF_CLK 		|PA1		|PA1
 *	CRS				|PA7		|PA7
 *	RX0				|PC4		|PC4
 *	RX1				|PC5		|PC5
 *	TX_EN			|PB11		|PG11
 *	TX0				|PB12		|PG13
 *	TX1				|PB13		|PG14
 *
 * Also, PA8 pins HAVE TO be initialized as MCO alternate function,
 * or communication will not work. Who knows why.
 *
 * Library supports DHCP to get IP from router.
 * This feature is disabled by default
 * To enable it, you need to add define in defines.h file:
 *
 *	//Enable DHCP 
 *	#define ETHERNET_USE_DHCP
 *
 * This library supports more than just DP83848 PHY.
 * Currently, supported list is below.
 * The number which is near text means value you have to set ETHERNET_PHY define
 * in case you want to select custom PHY.
 *
 *	- 0: DP83848
 *	- 1: LAN8720A
 *	- 2: KSZ8081RNA
 *
 * If you want (for example) to use KSZ8081RNA PHY, you need to do this (defines.h):
 *	
 *	//Use KSZ8081RNA PHY
 *	#define ETHERNET_PHY	2
 */
#ifndef TM_ETHERNET_H
#define TM_ETHERNET_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * Library dependencies
 * - STM32F4xx
 * - STM32F4xx RCC
 * - STM32F4xx GPIO
 * - STM32F4xx ETH
 * - defines.h
 * - attributes.h
 * - LwIP stack
 * - TM DELAY
 * - TM GPIO
 */
/**
 * Includes
 */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "defines.h"
#include "attributes.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_gpio.h"

/* Ethernet based includes */
#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"
#include "netconf.h"
#include "tcp_echoclient.h"
#include "lwip/ip_addr.h"
#include "lwip/dns.h"
#include "lwip/tcp.h"

/* C based includes */
#include "string.h"
#include "stdio.h"

/* Ethernet PHY used in your project */
/* Default value is DP83848 */
#ifndef ETHERNET_PHY
#define ETHERNET_PHY						0
#endif

/* Check for link status every 1000ms */
#ifndef LINK_TIMER_INTERVAL
#define LINK_TIMER_INTERVAL					1000
#endif

/* Default server PORT used */
#ifndef ETHERNET_SERVER_PORT
#define ETHERNET_SERVER_PORT				80
#endif

/* Include server based options */
#include "httpd.h"
#include "fs.h"

/* MAC ADDRESS: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5 */
/* In case you want to use custom MAC, use parameter in init function */
#ifndef MAC_ADDR0
#define MAC_ADDR0							0x06
#define MAC_ADDR1							0x05
#define MAC_ADDR2							0x04
#define MAC_ADDR3							0x03
#define MAC_ADDR4							0x02
#define MAC_ADDR5							0x01
#endif

/* Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
/* Used in case DHCP is not used or response failed */
/* In case you want to use custom IP, use parameter in init function */
#ifndef IP_ADDR0
#define IP_ADDR0							192
#define IP_ADDR1							168
#define IP_ADDR2							0
#define IP_ADDR3							120
#endif

/* NETMASK */
/* In case you want to use custom netmask, use parameter in init function */
#ifndef NETMASK_ADDR0
#define NETMASK_ADDR0						255
#define NETMASK_ADDR1						255
#define NETMASK_ADDR2						255
#define NETMASK_ADDR3						0
#endif

/* Gateway Address */
/* In case you want to use custom gateway, use parameter in init function */
#ifndef GW_ADDR0
#define GW_ADDR0							192
#define GW_ADDR1							168
#define GW_ADDR2							0
#define GW_ADDR3							1
#endif

/* Device name which will be seen in router when requesting IP via DHCP */
#ifndef ETHERNET_HOSTNAME_DHCP
#define ETHERNET_HOSTNAME_DHCP				"TM-ETH-Device"
#endif

/* RMII mode by default */
#if !defined(ETHERNET_MII_MODE) && !defined(ETHERNET_RMII_MODE)
	#define ETHERNET_RMII_MODE
#endif

/* Buffer size for ethernet headers when requesting as client */
/* Used for headers after you are connected to server, HTTP headers */
/* Ex: GET / HTTP/1.1\r\n...etc */
#ifndef ETHERNET_MAX_HEADER_SIZE
#define ETHERNET_MAX_HEADER_SIZE			1024
#endif

/* Max connection string name when connection as client */
/* This is also max domain name when using DNS request */
#ifndef ETHERNET_MAX_CONNECTION_NAME
#define ETHERNET_MAX_CONNECTION_NAME		100
#endif

/* Maximal number of opened files at a time as server */
#ifndef ETHERNET_MAX_OPEN_FILES
#define ETHERNET_MAX_OPEN_FILES     		10
#endif

/* Maximal length for SSI tag used in HTML */
#ifndef ETHERNET_SSI_MAX_TAG_LEN
#define ETHERNET_SSI_MAX_TAG_LEN			32
#endif

/* Maximal buffer length for SSI tag insert text */
/* Content of buffer will be replaced with SSI tag */
#ifndef ETHERNET_SSI_MAX_TAG_INSERT_LEN
#define ETHERNET_SSI_MAX_TAG_INSERT_LEN		512
#endif

/* Internal struct which should not be used by user */
typedef struct {
	uint8_t ip_addr[4];
	uint8_t mac_addr[6];
	uint8_t netmask[4];
	uint8_t gateway[4];
	uint8_t staticip;
	uint8_t ip_is_set;
	uint8_t link_is_up;
	uint32_t ClientSuccessfullConnections, ClientConnections;
	uint64_t Client_TX_Bytes, Client_RX_Bytes;
	uint32_t Server_Connections;
	uint64_t Server_TX_Bytes, Server_RX_Bytes;
	uint8_t server_active;
	uint16_t server_port;
	uint8_t speed_100m;
	uint8_t full_duplex;
} TM_ETHERNET_t;
extern TM_ETHERNET_t TM_ETHERNET;

/* Client protocol states */
/* For private use */
typedef enum  {
	CLIENT_NOT_CONNECTED = 0,
	CLIENT_CONNECTED,
	CLIENT_RECEIVED,
	CLIENT_CLOSING,
} client_states;

/**
 * Client structure, passed as first parameter in all client based callback functions
 *
 * Parameters:
 * 	- char name[ETHERNET_MAX_CONNECTION_NAME]:
 * 		Connection name, we choose it when we call TM_ETHERNETCLIENT_Connect function
 * 	- uint8_t active:
 * 		Connection is active
 * 	- uint8_t ip_addr[4]:
 * 		IP address for our external connection
 * 	- uint16_t port:
 * 		Port for our external connection
 * 	- uint8_t headers_done:
 * 		This is user defined option.
 * 		When you connect to some website and receive data back, you will also get HTTP headers and your data.
 * 		When receive data handler will be first called, this parameter will be set to 0.
 * 		If you detect when headers are done ("\r\n\r\n" string) then you can set this parameter to 1,
 * 		and it will stay 1 until connection is closed. This can be used, if you don't want to print headers to user.
 *	- uint8_t* active_connections_count:
 *		Pointer to number of active connections this time.
 *	- void* user_parameters:
 *		Pointer to user parameters for connection which are passed on "Connect" function
 *		This can be used to pass special data for your connection, which you can then easily be used in headers callback
 *		to format proper request string.
 */
typedef struct {
	char name[ETHERNET_MAX_CONNECTION_NAME];
	uint8_t active;
	uint8_t ip_addr[4];
	uint16_t port;
	uint8_t headers_done;
	uint8_t* active_connections_count;
	void* user_parameters;
	/* Private use */
	struct tcp_pcb* pcb;
	client_states state;
	struct pbuf *p_tx;
} TM_TCPCLIENT_t;

/**
 * POST request structure
 * Used in POST request callbacks
 *
 * Parameters:
 * 	- void *connection:
 * 		Pointer to connection, it is type of struct http_state *
 * 	- const char* uri:
 * 		URL which is used in POST request
 *		URL user use with POST request
 * 	- const char* http_request:
 * 		Pointer to HTTP request buffer.
 *		Here are stored entire HTTP request headers
 * 	- uint16_t http_request_len:
 * 		Length of HTTP request buffer
 * 	- int content_len:
 * 		Length of content (data) in bytes
 * 	- char* response_uri:
 * 		Pointer to address (file address, "/index.html" for example) which will be used to show url to user.
 * 		This parameter must be filled by user
 * 	- uint16_t response_uri_len:
 * 		Length of buffer for response uri
 */
typedef struct {
	void* connection;
	const char* uri;
	const char* http_request;
	uint16_t http_request_len;
	int content_len;
	char* response_uri;
	uint16_t response_uri_len;
} TM_ETHERNETPOST_t;

/**
 * Result enumeration used in ethernet library.
 *
 * Parameters:
 * 	- TM_ETHERNET_Result_Ok:
 * 		Everything is OK
 * 	- TM_ETHERNET_Result_Error:
 * 		An error occured
 * 	- TM_ETHERNET_Result_IPIsNotSetYet:
 * 		We don't have set IP
 * 	- TM_ETHERNET_Result_LinkIsDown:
 * 		Link is down
 * 	- TM_ETHERNET_Result_NeedHardReset:
 * 		We need hardware reset
 * 	- TM_ETHERNET_Result_LibraryNotInitialized:
 * 		Library is not initialized
 */
typedef enum {
	TM_ETHERNET_Result_Ok = 0,
	TM_ETHERNET_Result_Error,
	TM_ETHERNET_Result_IPIsNotSetYet,
	TM_ETHERNET_Result_LinkIsDown,
	TM_ETHERNET_Result_NeedHardReset,
	TM_ETHERNET_Result_LibraryNotInitialized
} TM_ETHERNET_Result_t;

/* Additional typedefs */
/* User friendly typedef for SSI tags */
typedef const char * TM_ETHERNET_SSI_t;
/* User friendly typedef for CGI tags */
/**
 * Structure is as following:
 * 
 * Parameters:
 * 	- const char *pcCGIName:
 *		Link, when handler function will be called, "/leds.cgi" for example
 *	- tCGIHandler pfnCGIHandler:
 *		Function handler, which will be called when user access to some link
 *
 * Handler function looks like this:
 *	const char * HandlerFunctionName(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
 *  
 * Function must return "URL" which will be displayed to user, for example "/index.html" to show first site
 */
typedef tCGI TM_ETHERNET_CGI_t;

/* Ethernet */
/**
 * Initialize ethernet device and prepare device to work.
 *
 * This function prepares LwIP stack to work and DP83848 PHY,
 * but does not enable server functionality. You have separate
 * function which enables server functionality in case you need it.
 *
 * Parameters:
 *	- uint8_t* mac_addr:
 *		Pointer to 6 bytes long array, if you want to select custom
 *		MAC address, instead of one in in defines.
 *		Useful if you have more than one device in one network
 *		and you want to separate them by custom MAC (with their unique ID number)
 *		Set to NULL for default value
 *	- uint8_t* ip_addr:
 *		Pointer to 4 bytes long array, if you want to select custom
 *		IP address in case STATIC is used. This will also be used,
 *		if DHCP can't get IP address
 *		Set to NULL for default value
 *	- uint8_t* gateway:
 *		Pointer to 4 bytes long array, if you want to select custom	gateway.
 *		Set to NULL for default value
 *	- uint8_t* netmask:
 *		Pointer to 4 bytes long array, if you want to select custom netmask
 *		Set to NULL for default value
 *
 * Member of TM_ETHERNET_Result_t is returned
 */
extern TM_ETHERNET_Result_t TM_ETHERNET_Init(uint8_t* mac_addr, uint8_t* ip_addr, uint8_t* gateway, uint8_t* netmask);

/**
 * Update ethernet LwIP stack
 *
 * This function should be called periodically, as fast as possible.
 *
 * Member of TM_ETHERNET_Result_t is returned
 */
extern TM_ETHERNET_Result_t TM_ETHERNET_Update(void);

/**
 * This function should be called in specific time period for LwIP stack
 *
 * Parameters:
 *  - uint16_t millis:
 *		Number of milliseconds, that will be added to LwIP stack.
 *		This value should be the same as period for your interrupt (Systick) timer is
 *
 * Member of TM_ETHERNET_Result_t is returned
 */
extern TM_ETHERNET_Result_t TM_ETHERNET_TimeUpdate(uint16_t millis);

/**
 * This function can be used to test if you are ready to work as server or client.
 *
 * Returns:
 *  - TM_ETHERNET_Result_Ok: In case link is up and ip is set (static or DHCP)
 *  - TM_ETHERNET_Result_Error: On other cases
 */
extern TM_ETHERNET_Result_t TM_ETHERNET_TestReady(void);

/**
 * Check if device has static IP
 *
 * Returns 1 if it is static or 0 if not (DHCP)
 */
#define TM_ETHERNET_IsIPStatic()			(TM_ETHERNET.staticip)

/**
 * Check if device has 100Mbit network connection
 *
 * Returns 1 if it has, or 0 if not
 */
#define TM_ETHERNET_Is100M()				(TM_ETHERNET.speed_100m)

/**
 * Check if device is in full duplex mode
 *
 * Returns 1 if it is or 0 if not
 */
#define TM_ETHERNET_IsFullDuplex()			(TM_ETHERNET.full_duplex)

/**
 * Get local IP address
 *
 * Parameters:
 * 	- uint8_t x:
 * 		IP section, 0 to 3 are allowed. 0 is MSB and 3 is LSB
 *
 * IP address is returned
 */
#define TM_ETHERNET_GetLocalIP(x)			(((x) >= 0 && (x) < 4) ? TM_ETHERNET.ip_addr[(x)] : 0)

/**
 * Get MAC address
 *
 * Parameters:
 * 	- uint8_t x:
 * 		MAC section, 0 to 5 are allowed. 0 is MSB and 5 is LSB
 *
 * MAC address is returned
 */
#define TM_ETHERNET_GetMACAddr(x)			(((x) >= 0 && (x) < 6) ? TM_ETHERNET.mac_addr[(x)] : 0)

/**
 * Get gateway address
 *
 * Parameters:
 * 	- uint8_t x:
 * 		Gateway section, 0 to 3 are allowed. 0 is MSB and 3 is LSB
 *
 * Gateway is returned
 */
#define TM_ETHERNET_GetGateway(x)			(((x) >= 0 && (x) < 4) ? TM_ETHERNET.gateway[(x)] : 0)

/**
 * Get netmask address
 *
 * Parameters:
 * 	- uint8_t x:
 * 		Netmask section, 0 to 4 are allowed. 0 is MSB and 3 is LSB
 *
 * Net mask is returned
 */
#define TM_ETHERNET_GetNetmask(x)			(((x) >= 0 && (x) < 4) ? TM_ETHERNET.netmask[(x)] : 0)

/* Server based functions */
/**
 * Enable and prepare LwIP stack for server functionality.
 * Before you can use this function, you have to initialize LwIP stack
 * with TM_ETHERNET_Init() function.
 *
 * Parameters:
 *	- uint16_t server_port:
 *		Select your port on which you want to operate.
 *		Set this variable to 0 and last used PORT will be selected
 *		If you call this function multiple times, only first time will work this parameter
 *
 * Member of TM_ETHERNET_Result_t is returned
 */
extern TM_ETHERNET_Result_t TM_ETHERNETSERVER_Enable(uint16_t server_port);

/**
 * Disable and stop LwIP stack from server functionality.
 *
 * Member of TM_ETHERNET_Result_t is returned
 */
extern TM_ETHERNET_Result_t TM_ETHERNETSERVER_Disable(void);

/**
 * Check if server mode is enabled.
 *
 * Returns 1 if active, or 0 if not
 */
#define TM_ETHERNETSERVER_Enabled()				(TM_ETHERNET.server_active)

/**
 * Get port number on which server is active
 *
 * Returns port number on which server is active
 */
#define TM_ETHERNETSERVER_GetPortNumber()		(TM_ETHERNET.server_port)

/**
 * Get number of all sent bytes from server to client
 *
 * Returns number of sent bytes from server to client
 */
#define TM_ETHERNETSERVER_GetTXBytes()			(TM_ETHERNET.Server_TX_Bytes)

/**
 * Get number of all received bytes from client to server
 *
 * Returns number of received bytes from client to server
 */
#define TM_ETHERNETSERVER_GetRXBytes()			(TM_ETHERNET.Server_RX_Bytes)

/**
 * Get number of all connections that have been made to server
 *
 * Returns number of connections made to server
 */
#define TM_ETHERNETSERVER_GetConnectionsCount()	(TM_ETHERNET.Server_Connections)

/**
 * Set SSI tags, which will be used in your HTML files for displaying variables on website.
 *
 * For more about SSI tags, you can check chapter 4.2 on link below:
 * http://www.state-machine.com/lwip/AN_QP_and_lwIP.pdf
 *
 * Note: SSI tags only work if your file name ends with .shtm, .shtml, or .ssi!
 *
 * This function can be called only once, future calls will be denied!
 *
 * Parameters:
 * 	- TM_ETHERNET_SSI_t* tags:
 * 		Pointer to array of pointers for tags that are used in your html files
 * 	- uint16_t number_of_tags:
 * 		Number of tags in your array
 *
 * Member of TM_ETHERNET_Result_t is returned
 */
extern TM_ETHERNET_Result_t TM_ETHERNETSERVER_SetSSITags(TM_ETHERNET_SSI_t* tags, uint16_t number_of_tags);

/**
 * Set GCI handlers which will be used when you want to control something over ethernet configured as server,
 * for example, turn on/off leds.
 *
 * For more about CGI tags, you can check chapter 4.3 on link below:
 * http://www.state-machine.com/lwip/AN_QP_and_lwIP.pdf
 *
 * Note: CGI tags only work if your file name ends with .cgi!
 *
 * This function can be called only once, future calls will be denied!
 *
 * Parameters:
 *  - const TM_ETHERNET_CGI_t *cgis:
 *  	Pointer to TM_ETHERNET_CGI_t structure with link/function pair combination.
 *		It can be array of structures
 *  - uint16_t number_of_handlers:
 *  	Number of pairs for .cgi and handler combination
 *
 * Member of TM_ETHERNET_Result_t is returned
 */
extern TM_ETHERNET_Result_t TM_ETHERNETSERVER_SetCGIHandlers(const TM_ETHERNET_CGI_t* cgis, uint16_t number_of_handlers);

/* Client based functions */
/**
 * Make TCP CLIENT Connection to external server.
 *
 * After this call, different client callbacks will be called depending on what will happen.
 *
 * Parameters:
 * 	- char* conn_name:
 * 		Connection name. This value will be then passed to all client based handlers
 * 	- uint8_t ip1:
 *	- uint8_t ip2:
 *	- uint8_t ip3:
 *	- uint8_t ip4:
 *		IP addresses. ip1 is MSB, ip4 is LSB
 *	- uint16_t port:
 *		Port to which we want connect to server
 *	- void* user_parameters:
 *		Pointer to user data to be passed to this function.
 *		This data will be used in all client based callbacks inside TM_TCPCLIENT_t pointer variable
 *		Use NULL in case you don't want to pass any data.
 *
 * Member of TM_ETHERNET_Result_t is returned
 */
extern TM_ETHERNET_Result_t TM_ETHERNETCLIENT_Connect(char* conn_name, uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, uint16_t port, void* user_parameters);

/**
 * Get total number of sent bytes when you are client
 *
 * Returns number of transmitted bytes as a client
 */
#define TM_ETHERNETCLIENT_GetTXBytes()						(TM_ETHERNET.Client_TX_Bytes)

/**
 * Get total number of received bytes when you are client
 *
 * Returns number of received bytes as a client
 */
#define TM_ETHERNETCLIENT_GetRXBytes()						(TM_ETHERNET.Client_RX_Bytes)

/**
 * Get number of all connections done as a client
 *
 * Returns number of all connections done as a client
 */
#define TM_ETHERNETCLIENT_GetConnectionsCount()				(TM_ETHERNET.ClientConnections)

/**
 * Get number of all successfull connections done as a client
 *
 * Returns number of all successfull connections done as a client
 */
#define TM_ETHERNETCLIENT_GetSuccessfullConnectionsCount()	(TM_ETHERNET.ClientSuccessfullConnections)

/* DNS */
/**
 * DNS Get host by name
 *
 * This function can be used to get IP address by passing it's domain name to it.
 *
 * Different DNS callback functions will be called, depending on what will happen with DNS.
 *
 * Parameters:
 * 	- char* host_name:
 * 		Domain name for which you need IP address.
 *
 * Member of TM_ETHERNET_Result_t is returned
 */
extern TM_ETHERNET_Result_t TM_ETHERNETDNS_GetHostByName(char* host_name);

/* Ethernet based callbacks */
/**
 * IP is set callback
 * This function is called when IP is assigned to device.
 *
 * It is called in case DHCP is used or static ip is used.
 * It can be used to store IP address, if you use DHCP dynamic IP address.
 *
 * Parameters:
 * 	- uint8_t ip_addr1:
 * 	- uint8_t ip_addr2:
 * 	- uint8_t ip_addr3:
 * 	- uint8_t ip_addr4:
 * 		IP addresses. ip_addr1 is MSB, ip_addr4 is LSB
 * 	- uint8_t dhcp:
 * 		This is set to 1 in case DHCP has been used for IP assign or 0 if static is used in case of user select or error
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNET_IPIsSetCallback(uint8_t ip_addr1, uint8_t ip_addr2, uint8_t ip_addr3, uint8_t ip_addr4, uint8_t dhcp);

/**
 * DHCP start callback
 *
 * This function is called when DHCP starts with IP address assigning for device.
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNET_DHCPStartCallback(void);

/**
 * Link is down callback
 *
 * This function will be called when link is down.
 * This can happen, if your router is down, or you unplug cable
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNET_LinkIsDownCallback(void);

/**
 * Link is up callback
 *
 * This function will be called when link is up again.
 * It is not called in case that link is up already on initialization at the begin
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNET_LinkIsUpCallback(void);

/* System reset callback */
/**
 * System reset callback
 *
 * If client connection is opened and link become down in this time, after link is up back,
 * then before new connection can be made, system MUST be restarted for security purposes.
 * Before you call NVIC_SystemReset() in function to reset system, you can do important stuff here.
 *
 * Note: if you don't reset your sistem, then function which is used to connect as a client,
 * will always refuse your try to connect to your specific IP
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNET_SystemResetCallback(void);

/* Ethernet client based callbacks */
/* Callbacks for ethernet client */
/**
 * Create headers callback
 * This function is called when connection is prepared to send some headers data to server which is waiting for response.
 *
 * If function is not implemented, then default headers are sent.
 *
 * Default headers:
 *  GET / HTTP/1.1\r\n
 *  Host: TM_ETHERNETClient\r\n
 *  Connection: close\r\n
 *  \r\n
 *
 * Parameters for function:
 * 	- TM_TCPCLIENT_t* connection:
 * 		Pointer to TM_TCPCLIENT_t which contains current connection specifications.
 *  - char* buffer:
 *  	Char memory buffer where to store your header data
 *  - uint16_t buffer_length:
 *  	Maximal memory length for your header data
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * Function must return number of characters written in buffer.
 * If you return 0 (zero) then connection will be closed.
 * Buffer length can't be larger than ETHERNET_MAX_HEADER_SIZE define
 */
extern uint16_t TM_ETHERNETCLIENT_CreateHeadersCallback(TM_TCPCLIENT_t* connection, char* buffer, uint16_t buffer_length);

/**
 * Receive data callback
 *
 * This function is called when we have data available to be received.
 * It might happen that it will be called multiple times for one request, depends on server's response and length of data.
 *
 * Parameters:
 * 	- TM_TCPCLIENT_t* connection:
 * 		Pointer to TM_TCPCLIENT_t which contanins current connection's specifications.
 * 	- uint8_t* buffer:
 * 		Memory buffer
 * 	- uint16_t buffer_length:
 * 		Number of characters in buffer
 *	- uint16_t total_length
 *		Number of unread data. If this value is the same as buffer_length, then no remaining data is available in packet
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNETCLIENT_ReceiveDataCallback(TM_TCPCLIENT_t* connection, uint8_t* buffer, uint16_t buffer_length, uint16_t total_length);

/**
 * Connected callback
 *
 * This function is called when device is connected to specific server.
 *
 * Parameters:
 * 	- TM_TCPCLIENT_t* connection:
 * 		Pointer to TM_TCPCLIENT_t which contains current connection's specifications
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNETCLIENT_ConnectedCallback(TM_TCPCLIENT_t* connection);

/**
 * Connection closed callback
 *
 * This function is called when connection is closed and you are ready to make a new one.
 * You can also detect, if connection was closed after success or error.
 *
 * Parameters:
 * 	- TM_TCPCLIENT_t* connection:
 * 		Pointer to TM_TCPCLIENT_t which contanins current connection specifications
 * 	- uint8_t success:
 *		It is set to 1 if connection was closed after successfull transmission
 *		or 0 if because of failure
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNETCLIENT_ConnectionClosedCallback(TM_TCPCLIENT_t* connection, uint8_t success);

/**
 * Error callback
 *
 * This function is called when an error occured.
 * It will be called in case when IP address is not available
 *
 * Parameters:
 * 	- TM_TCPCLIENT_t* connection:
 * 		Pointer to TM_TCPCLIENT_t which contanins current connection specifications
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNETCLIENT_ErrorCallback(TM_TCPCLIENT_t* connection);

/**
 * Connection started callback
 *
 * This function will be called when connection to server has started
 *
 * Parameters:
 * 	- TM_TCPCLIENT_t* connection:
 * 		Pointer to TM_TCPCLIENT_t which contanins current connection specifications
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNETCLIENT_ConnectionStartedCallback(TM_TCPCLIENT_t* connection);

/* DNS based callbacks */
/**
 * DNS Found callback
 *
 * This function will be called when DNS server has valid IP address for your domain request.
 * It can be useful to make first connection in this function when you know your IP address where you must connect to.
 *
 * Parameters:
 * 	- char* host_name:
 * 		The same host name as you use for DNS requesting, can't be larger than ETHERNET_MAX_CONNECTION_NAME define
 * 	- uint8_t ip_addr1:
 * 		IP address 1st byte, MSB
 * 	- uint8_t ip_addr2:
 * 		IP address 2nd byte
 * 	- uint8_t ip_addr3:
 * 		IP address 3rd byte
 * 	- uint8_t ip_addr4:
 * 		IP address 4th byte, LSB
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNETDNS_FoundCallback(char* host_name, uint8_t ip_addr1, uint8_t ip_addr2, uint8_t ip_addr3, uint8_t ip_addr4);

/**
 * DNS error callback
 *
 * This function will be called when DNS error occured.
 * Possible errors are:
 * 	- Requested domain does not exists,
 * 	- You have bad parameters in DNS function call
 *
 * Parameters:
 *  - char* host_name:
 *  	Host name where an error occured
 *
 * No return
 */
extern void TM_ETHERNETDNS_ErrorCallback(char* host_name);

/* Server based callbacks */
/**
 * SSI callbacks for ethernet server
 *
 * This function is called, when ethernet server wants to serve tag you pass in TM_ETHERNETSERVER_SetSSITags() function.
 *
 * Parameters:
 * 	- int iIndex:
 * 		Tag index number. They are specified in order you pass to TM_ETHERNETSERVER_SetSSITags function
 * 	- char *pcInsert:
 * 		Pointer where you should set your content that will be displayed where your tag is specified in files
 * 	- int iInsertLen:
 * 		Max string length
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * Must return number of characters in your pcInsert pointer.
 */
extern uint16_t TM_ETHERNETSERVER_SSICallback(int iIndex, char *pcInsert, int iInsertLen);

/**
 * Called when new client is connected to server
 * This function can be used, to block IP address.
 * IP address, from where user access to your server is stored in *pcb variable
 *
 * remote_ip = pcb->remote_ip.addr
 *
 * You can check here for your blocked IP addresses
 *
 * Parameters:
 *	- struct tcp_pcb *pcb:
 *		Pointer to active LwIP PCB structure
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * This function must return 1 if you want to allow connection,
 * or 0 if you want to deny access to your web.
 */
extern uint8_t TM_ETHERNETSERVER_ClientConnectedCallback(struct tcp_pcb *pcb);

/**
 * Called when new client is diconnected from server (Or connection closed)
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNETSERVER_ClientDisconnectedCallback(void);

/**
 * Called when user based file should be open when new client is connected
 * If you want to use server on your SDcard or USB flash or whatever,
 * here you can open file and do the stuff.
 *
 * Note: You must set file->len parameter to your file size number!
 *
 * If You don't open file (return 0) then default file in flash will be used,
 * that will say "Hello world from default server file", stored in fsdata.c.
 *
 * Only one file at a time will be opened, so if you need to mount card,
 * you can mount it in this function also.
 *
 * Parameters:
 *  - struct fs_file *file:
 *  	Pointer to fs_file struct
 *  - const char *name:
 *  	File name that should be opened
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * Return 0 in case you can't/don't want to open file, or 1 if file is successfully opened
 */
extern int TM_ETHERNETSERVER_OpenFileCallback(struct fs_file* file, const char *name);

/**
 * Called when reading file is finished and file can be closed.
 *
 * If you need to unmount your SD card, you can do it here.
 *
 * Parameters:
 * 	- struct fs_file *file:
 * 		Pointer to fs_file struct
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNETSERVER_CloseFileCallback(struct fs_file* file);

/**
 * Called when file should be read.
 * This function may be called more than just one time, depends on file length.
 *
 * Parameters:
 * 	- struct fs_file *file:
 * 		Pointer to fs_file struct
 * 	- char *buffer:
 * 		Pointer to buffer, where you should save your data
 * 	- int count:
 * 		Number of data that should be read.
 * 		You can read less than this number. This is max number you can read this time.
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * Returns number of read data, or -1 (EOF) if file has end.
 */
extern int TM_ETHERNETSERVER_ReadFileCallback(struct fs_file* file, char* buffer, int count);

/**
 * Post request begin callback
 *
 * This function is called, when POST request is trying to be started to our server.
 *
 * With this function, you can accept/decline POST request, depending on parameters you receive
 *
 * Note:
 * After POST request finish, you have to choose one file, which will be used to displayed to user after successfull POST end.
 * If you don't set it by yourself, then "/index.html" file will be used.
 * You can do that to set your custom file:
 *
 * strcpy(params->response_uri, "/yourfile.html");
 *
 * In this structure are also other variables, which one, you should look at TM_ETHERNETPOST_t struct declaration.
 *
 * Parameters:
 * 	- TM_ETHERNETPOST_t* params:
 * 		Pointer to TM_ETHERNETPOST_t structure which contains everything about POST connection.
 * 		Look for TM_ETHERNETPOST_t struct comments what you can use in this function.
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * This function must return 1 in case you want to accept POST request, or return 0 in case you want to deny it.
 * In params structure you have also HTTP headers, so you can look at them and deny access in some cases.
 */
extern uint8_t TM_ETHERNETSERVER_PostRequestBeginCallback(TM_ETHERNETPOST_t* params);

/**
 * Post request receive data available callback
 *
 * This function is called, when we have "Content-Length" statement.
 * Function might be called multiple times, depends on how many data you try to sent over POST method.
 * Data are stored inside p->payload variable pointer, which is void pointer. Make a cast to char* variable.
 * In pbuf struct is also length of this packet, so you have a little control about that.
 *
 * To know when you have last time called this function, you can make a compare of params variables.
 *
 * Parameters:
 * 	- TM_ETHERNETPOST_t* params:
 * 		Pointer to TM_ETHERNETPOST_t structure which contains everything about POST connection.
 * 		Look for TM_ETHERNETPOST_t struct comments what you can use in this function.
 * 	- struct pbuf* p:
 * 		Pointer to pbuf structure where data are stored.
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * This function must return 1 if you want to receive more data, or return 0 in case you want to stop POST request at some time.
 */
extern uint8_t TM_ETHERNETSERVER_PostRequestReceiveDataCallback(TM_ETHERNETPOST_t* params, struct pbuf* p);

/**
 * Post request end callback
 *
 * This function is called when POST request is done.
 *
 * This function must set response_uri variable to set response URL, which will be used to show to user.
 * If you don't set it, then default will be used.
 *
 * Parameters:
 * 	- void* connection:
 * 		Pointer to connection parameters, it is type of struct http_state *
 * 	- char* response_uri:
 * 		Pointer to buffer for response uri set
 * 	- u16_t response_uri_len:
 * 		Length of response uri buffer length
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNETSERVER_PostRequestEndCallback(void* connection, char* response_uri, u16_t response_uri_len);

/**
 * Called when ethernet peripheral is ready.
 * In this function, user can do other stuff, depending on PHY which is used in project.
 *
 * Example:
 *	- Set LED mode for DP83848 PHY, or something else
 * 
 * Parameters:
 * 	- uint32_t PHYAddress:
 * 		Phy address when using ETH_ReadPHYRegister and ETH_WritePHYRegister functions
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * No return
 */
extern void TM_ETHERNETPHY_CustomOptions(uint32_t PHYAddress);

/**
 * Called on ethernet GPIO initialization
 * In this function, user can initialize custom pins, which will be used for communication with PHY.
 * Don't implement this function in case you want default pinout
 *
 * With __weak parameter to prevent link errors if not defined by user
 *
 * Must return 1 in case user has made own pins, or 0 in case we want default pins
 */
extern uint8_t TM_ETHERNET_InitPinsCallback(void);

/* Private functions which should not be called from user */
extern void TM_ETHERNET_INT_CustomOptions(uint32_t PHYAddress);
extern void TM_ETHERNET_INT_SetIPAddress(uint8_t ip_addr1, uint8_t ip_addr2, uint8_t ip_addr3, uint8_t ip_addr4, uint8_t dhcp);
extern void TM_ETHERNET_INT_LinkIsDownCallback(void);
extern void TM_ETHERNET_INT_LinkIsUpCallback(void);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif

