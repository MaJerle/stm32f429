
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "memp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tcp_echoclient.h"

#include "tm_stm32f4_ethernet.h"

/* Maximal number of active client connections at a time */
#define TCPCLIENT_MAX_CONNECTIONS       4

#if LWIP_TCP
static uint8_t tcp_active_connections = 0;
/* Header data */
uint8_t data[ETHERNET_MAX_HEADER_SIZE];

static uint8_t CablePluggedWithActiveConnection = 0;

/* For private use, connections */
TM_TCPCLIENT_t TM_Client[TCPCLIENT_MAX_CONNECTIONS];

/* Private function prototypes */
static err_t tcp_echoclient_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_echoclient_connection_close(TM_TCPCLIENT_t* client, uint8_t success);
static err_t tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_echoclient_send(TM_TCPCLIENT_t* client);
static err_t tcp_echoclient_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
static void tcp_echoclient_error(void *arg, err_t err);

static TM_TCPCLIENT_t* tcp_client_malloc(void) {
	uint8_t i;
	
	/* Go through all free memory */
	for (i = 0; i < TCPCLIENT_MAX_CONNECTIONS; i++) {
		/* If not in use */
		if (TM_Client[i].active == 0) {
			/* Use it */
			TM_Client[i].active = 1;
			
			/* Increase number of active connections */
			tcp_active_connections++;
			
			/* Set pointer to active connections */
			TM_Client[i].active_connections_count = &tcp_active_connections;
			
			/* Return it */
			return &TM_Client[i];
		}
	}
	
	/* Memory available */
	return NULL;
}

static void tcp_client_free(TM_TCPCLIENT_t* client) {
	uint8_t i;
	
	/* Go through memory */
	for (i = 0; i < TCPCLIENT_MAX_CONNECTIONS; i++) {
		/* If they are the same */
		if (&TM_Client[i] == client) {
			/* Set all to NULL */
			client->pcb = NULL;
			client->p_tx = NULL;
			
			/* Decrease number of active connections */
			tcp_active_connections--;
			
			/* Clear flag */
			TM_Client[i].active = 0x00;
		}
	}
}

static uint8_t tcp_client_get_number_active_connections(void) {
	/* Return number of connections */
	return tcp_active_connections;
}

void tcp_echoclient_link_is_up_callback(void) {
	/* Called when cable is plugged back */
	/* Called from TM_ETHERNET library */
	
	/* */
	
	/* Close all connections */
}

void tcp_echoclient_link_is_down_callback(void) {
	/* Set variable that cable is plugged */
	if (tcp_client_get_number_active_connections()) {
		/* We had active connection before cable was unplugged */
		/* When we release it back, we need to make reset first */
		/* For safety purposes */
		CablePluggedWithActiveConnection = 1;
	}
}

/* Connects to the TCP echo server */
err_t tcp_echoclient_connect(char* conn_name, uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4, uint16_t port, void* user_parameters) {
	struct ip_addr DestIPaddr;
	TM_TCPCLIENT_t* client;
	
	/* Check if we have already active connections */
	if (tcp_client_get_number_active_connections()) {
		/* If cable is unplugged and we want to make a new connection */
		if (CablePluggedWithActiveConnection) {
			/* Call user function if he wants to reset MCU */
			TM_ETHERNET_SystemResetCallback();
			
			/* Return "error" */
			return ERR_CONN;
		}
	}
	
	/* Check if conection name is too big */
	if (strlen(conn_name) >= ETHERNET_MAX_CONNECTION_NAME) {
		return ERR_ARG;
	}
	
	/* Check for connection */
	client = tcp_client_malloc();
	
	/* In case no memory available */
	if (client == NULL) {
		return ERR_MEM;
	}
	
	/* create new tcp pcb */
	client->pcb = tcp_new();
	
	if (client->pcb != NULL) {
		/* Set IP address */
		IP4_ADDR(&DestIPaddr, ip1, ip2, ip3, ip4);
		
		/* Save IP address */
		client->ip_addr[0] = ip1;
		client->ip_addr[1] = ip2;
		client->ip_addr[2] = ip3;
		client->ip_addr[3] = ip4;
		
		/* Save port */
		client->port = port;
		
		/* Clear flag for headers */
		client->headers_done = 0;
		
		/* Save user parameters */
		client->user_parameters = user_parameters;
		
		/* Save connection name */
		strcpy(client->name, conn_name);
		
		/* Set parameters */
		tcp_arg(client->pcb, client);
		
		/* Set error function */
		tcp_err(client->pcb, tcp_echoclient_error);
		
		/* connect to destination address/port */
		tcp_connect(client->pcb, &DestIPaddr, port, tcp_echoclient_connected);
		
		/* Connection has started */
		TM_ETHERNETCLIENT_ConnectionStartedCallback(client);
		
		/* Return OK */
		return ERR_OK;
	} else {
		/* Deallocate */
		tcp_client_free(client);
	}
	
	/* Return error */
	return ERR_CONN;
}

/**
  * @brief Function called when TCP connection established
  */
static err_t tcp_echoclient_connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
	uint16_t length;
	TM_TCPCLIENT_t* client;

	/* Client is passed as arguments */
	client = (TM_TCPCLIENT_t *)arg;
	
	if (err == ERR_OK) {
		/* We are connected */
		client->state = CLIENT_CONNECTED;

		/* Get HTTP request from user */
		length = TM_ETHERNETCLIENT_CreateHeadersCallback(client, (char *)data, sizeof(data));
		
		/* Check if length = 0 */
		if (length == 0) {
			/* Close connection */
			tcp_echoclient_connection_close(client, 1);
			
			/* Return */
			return ERR_CONN;
		}

		/* Allocate pbuf */
		client->p_tx = pbuf_alloc(PBUF_TRANSPORT, strlen((char *)data), PBUF_POOL);

		/* If we have memory for buffer */
		if (client->p_tx) {
			/* Call user function */
			TM_ETHERNETCLIENT_ConnectedCallback(client);
			
			/* copy data to pbuf */
			pbuf_take(client->p_tx, (char *)data, length);

			/* initialize LwIP tcp_recv callback function */ 
			tcp_recv(client->pcb, tcp_echoclient_recv);

			/* initialize LwIP tcp_sent callback function */
			tcp_sent(client->pcb, tcp_echoclient_sent);

			/* initialize LwIP tcp_poll callback function */
			tcp_poll(client->pcb, tcp_echoclient_poll, 1);

			/* Set new error handler */
			tcp_err(client->pcb, tcp_echoclient_error);
			
			/* send data */
			tcp_echoclient_send(client);

			/* Return OK */
			return ERR_OK;
		}
	} else {
		/* close connection */
		tcp_echoclient_connection_close(client, 0);
	}
	return err;
}
    
/**
  * @brief tcp_receiv callback
  */
static err_t tcp_echoclient_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) { 
	struct pbuf* point_pbuf;
	err_t ret_err;
	TM_TCPCLIENT_t* client;

	/* Client is passed as arguments */
	client = (TM_TCPCLIENT_t *)arg;

	/* Check all next buffers and pointers */
	if (p != NULL && p->tot_len <= 1460) {
		/* Increase RX bytes */
		TM_ETHERNET.Client_RX_Bytes += p->tot_len;
		
		for (point_pbuf = p; point_pbuf != NULL; point_pbuf = point_pbuf->next) {
			if (point_pbuf->len == 0) continue;
			/* Call user function if defined to respond to incoming data */
			TM_ETHERNETCLIENT_ReceiveDataCallback(client, (uint8_t *) point_pbuf->payload, point_pbuf->len, point_pbuf->tot_len);
		}
	}
	
	/* if we receive an empty tcp frame from server => close connection */
	if (p == NULL) {
		/* remote host closed connection */
		client->state = CLIENT_CLOSING;
		if (client->p_tx == NULL) {
			/* we're done sending, close connection */
			tcp_echoclient_connection_close(client, 1);
		} else {    
			/* send remaining data*/
			tcp_echoclient_send(client);
		}
		ret_err = ERR_OK;
	} else if (err != ERR_OK) {/* else : a non empty frame was received from echo server but for some reason err != ERR_OK */
		/* free received pbuf*/
		pbuf_free(p);

		ret_err = err;
	} else if (client->state == CLIENT_CONNECTED) {
		/* Acknowledge data reception */
		tcp_recved(tpcb, p->tot_len);  

		/* free pbuf and do nothing */
		pbuf_free(p);
		//tcp_echoclient_connection_close(tpcb, es);
		ret_err = ERR_OK;
	} else {/* data received when connection already closed */
		/* Acknowledge data reception */
		tcp_recved(tpcb, p->tot_len);

		/* free pbuf and do nothing */
		pbuf_free(p);
		ret_err = ERR_OK;
	}
	
	/* Return */
	return ret_err;
}

/**
  * @brief function used to send data
  */
static void tcp_echoclient_send(TM_TCPCLIENT_t* client) {
	struct pbuf *ptr;
	err_t wr_err = ERR_OK;
	
	while ((wr_err == ERR_OK) &&
		(client->p_tx != NULL) && 
		(client->p_tx->len <= tcp_sndbuf(client->pcb))
	) {
		/* get pointer on pbuf from structure */
		ptr = client->p_tx;

		/* enqueue data for transmission */
		wr_err = tcp_write(client->pcb, ptr->payload, ptr->len, 1);
		
		if (wr_err == ERR_OK) {
			/* Increase number of TX bytes sent as client */
			TM_ETHERNET.Client_TX_Bytes += ptr->len;
			
			/* Continue with next pbuf in chain (if any) */
			client->p_tx = ptr->next;

			if (client->p_tx != NULL) {
				/* Increment reference count for client->p */
				pbuf_ref(client->p_tx);
			}
			/* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
			pbuf_free(ptr);
		} else if (wr_err == ERR_MEM) {
			/* we are low on memory, try later, defer to poll */
			client->p_tx = ptr;
		} else {
			/* other problem ?? */
		}
	}
}

/**
  * @brief  This function implements the tcp_poll callback function
  */
static err_t tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb) {
	err_t ret_err;
	TM_TCPCLIENT_t* client;

	/* Client is passed as arguments */
	client = (TM_TCPCLIENT_t *)arg;
	
	/* Valid client */
	if (client != NULL) {
		if (client->p_tx != NULL) {
			/* there is a remaining pbuf (chain), try to send data */
			tcp_echoclient_send(client);
		} else {
			/* no remaining pbuf (chain)  */
			if (client->state == CLIENT_CLOSING) {
				/* Close tcp connection */
				tcp_echoclient_connection_close(client, 1);
			}
		}
		ret_err = ERR_OK;
	} else {
		/* nothing to be done */
		tcp_abort(tpcb);
		ret_err = ERR_ABRT;
	}
	return ret_err;
}

/**
  * @brief  This function implements the tcp_sent LwIP callback (called when ACK
  *         is received from remote host for sent data)
  */
static err_t tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
	TM_TCPCLIENT_t* client;

	/* Client is passed as arguments */
	client = (TM_TCPCLIENT_t *)arg;

	if (client->p_tx != NULL) {
		/* still got pbufs to send */
		tcp_echoclient_send(client);
	}

	return ERR_OK;
}

/**
  * @brief This function is used to close the tcp connection with server
  */
static void tcp_echoclient_connection_close(TM_TCPCLIENT_t* client, uint8_t success) {
	/* Remove callbacks from PCB */
	if (client->pcb != NULL) {
		tcp_recv(client->pcb, NULL);
		tcp_sent(client->pcb, NULL);
		tcp_poll(client->pcb, NULL, 0);
	}
	
	/* Close tcp connection */
	if (client->pcb != NULL) {
		tcp_close(client->pcb);
	}
	
	/* Free pcb */
	mem_free(client->pcb);
	
	/* Increase number of connections */
	TM_ETHERNET.ClientConnections++;
	
	/* In case connection was successfull */
	if (success) {
		/* Increase successfull connections counter */
		TM_ETHERNET.ClientSuccessfullConnections++;
	}
	
	/* Free client, it's not real free, only free flag is set */
	/* Called before connection callback, so user can make a new connection */
	/* inside connection closed callback */
	tcp_client_free(client);
	
	/* Call user function if defined */
	TM_ETHERNETCLIENT_ConnectionClosedCallback(client, success);
}

/**
  * @brief Function called when TCP connection error
  */
static void tcp_echoclient_error(void *arg, err_t err) {
	TM_TCPCLIENT_t* client;

	/* Client is passed as arguments */
	client = (TM_TCPCLIENT_t *)arg;
	
	/* Call user function */
	TM_ETHERNETCLIENT_ErrorCallback(client);

	/* Close connection */
	tcp_echoclient_connection_close(client, 0);
}

#endif /* LWIP_TCP */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
