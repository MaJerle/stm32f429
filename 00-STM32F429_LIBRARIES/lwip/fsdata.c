#ifndef __FS_DATA_FILE_H
#define __FS_DATA_FILE_H

/* Here are stored custom files in flash. */
/* If you have custom files on SD card, then take a look for my open/read/close file callbacks, which can operate with SD card easily */

#include "fs.h"
#include "lwip/def.h"
#include "fsdata.h"
#include "tm_stm32f4_ethernet.h"

/* If user uses custom data */
#ifndef ETHERNET_USE_CUSTOM_DEFAULT_INDEX

#define file_NULL (struct fsdata_file *) NULL

/* style.css file */
__weak const unsigned char data__style_css[] = "html,body{width:100%;margin:0;padding:0;font-family:Arial, Calibri;font-size:11px}#header{background:#DDDDDD;text-align:center;font-size:36px;padding:20px;white-space:nowrap;border-bottom:1px solid #999999}#main{margin:0 auto;width:450px}table{border-collapse:collapse;border:1px solid #DDDDDD;width:450px !important;margin:0 auto;width:100%}th,td{text-align:left;border-collapse:collapse;border:1px solid #CCCCCC;padding:5px;font-size:11px}td.td_header{text-align:center;padding:4px;background:#CCCCCC;font-size:13px;font-weight:bold}.blue{color:blue}.red{color:red}.orange{color:orange}.green{color:green}a:link,a:visited{border:1px solid #CCCCCC;background:#EEEEEE;display:inline-block;padding:3px 6px;color:#333333;margin:0 2px}a:hover{border-color:#BBBBBB;background:#DDDDDD}.bold{font-weight:bold}.align_center{text-align:center}.float_right{float:right}.clear{clear:both}img{max-width:450px}";

/* Create 404 page */
__weak const unsigned char data__404_html[] = "<html><head><meta http-equiv=\"content-type\" content=\"text/html\"/><meta name=\"author\" content=\"Tilen Majerle\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/><link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\"/><title>Error 404</title></head><body><div id=\"header\">Web server on STM32F4xx</div><div id=\"main\"><p><a href=\"index.shtml\">Go back to first site</a></p><table><tr><td class=\"td_header\">Error 404</td></tr><tr><td>Requested page is not available!</td></tr></table></div></body></html>";

/* index.shtml page */
__weak const unsigned char data__index_html[] = "<html><head><meta http-equiv=\"content-type\" content=\"text/html\" /><meta name=\"author\" content=\"Tilen Majerle\" /><!--<meta http-equiv=\"refresh\" content=\"2\" />--><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /><link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\" /><title>Web on STM32F4xx flash</title></head><body><div id=\"header\">Web server on STM32F4xx</div><div id=\"main\"><table><tr><td colspan=\"3\" class=\"td_header\">Program</td></tr><tr><td colspan=\"2\" class=\"bold\">Compiled datetime</td><td><!--#compiled--></t></tr><tr><td colspan=\"2\" class=\"bold\">Hardware</td><td><!--#hardware--></t></tr><tr><td colspan=\"2\" class=\"bold\">Current time</td><td><!--#rtc_time--></t></tr><tr><td colspan=\"3\" class=\"td_header\">Network</td></tr><tr><td colspan=\"2\" class=\"bold\">Local address</td><td><!--#srv_adr--></t></tr><tr><td colspan=\"2\" class=\"bold\">MAC address</td><td><!--#mac_adr--></t></tr><tr><td colspan=\"2\" class=\"bold\">Gateway</td><td><!--#gateway--></t></tr><tr><td colspan=\"2\" class=\"bold\">Netmask</td><td><!--#netmask--></t></tr><tr><td colspan=\"2\" class=\"bold\">Link</td><td><!--#link--></t></tr><tr><td colspan=\"2\" class=\"bold\">Duplex</td><td><!--#duplex--></t></tr><tr><td colspan=\"3\" class=\"td_header\">Leds</td></tr><tr><th>LED</th><th>Status</th><th>Actions</th></tr><tr><td class=\"bold green\">GREEN</td><td><!--#led1_s--></td><td><a href='/ledaction.cgi?ledtoggle=1'>Toggle</a><a href='/ledaction.cgi?ledon=1'>On</a><a href='/ledaction.cgi?ledoff=1'>Off</a></td></tr><tr><td class=\"bold red\">RED</td><td><!--#led3_s--></td><td><a href='/ledaction.cgi?ledtoggle=3'>Toggle</a><a href='/ledaction.cgi?ledon=3'>On</a><a href='/ledaction.cgi?ledoff=3'>Off</a></td></tr><tr><td class=\"bold orange\">ORANGE</td><td><!--#led2_s--></td><td><a href='/ledaction.cgi?ledtoggle=2'>Toggle</a><a href='/ledaction.cgi?ledon=2'>On</a><a href='/ledaction.cgi?ledoff=2'>Off</a></td></tr><tr><td class=\"bold blue\">BLUE</td><td><!--#led4_s--></td><td><a href='/ledaction.cgi?ledtoggle=4'>Toggle</a><a href='/ledaction.cgi?ledon=4'>On</a><a href='/ledaction.cgi?ledoff=4'>Off</a></td></tr><tr><td colspan=\"3\" class=\"td_header\">Client</td></tr><tr><td colspan=\"2\" class=\"bold\">All connections</td><td><!--#clt_a_c--></td></tr><tr><td colspan=\"2\" class=\"bold\">Successful connections</td><td><!--#clt_s_c--></td></tr><tr><td colspan=\"2\" class=\"bold\">Percentage</td><td><!--#clt_per--></td></tr><tr><td colspan=\"2\" class=\"bold\">TX Bytes</td><td><!--#clt_tx--></td></tr><tr><td colspan=\"2\" class=\"bold\">RX Bytes</td><td><!--#clt_rx--></td></tr><tr><td colspan=\"3\" class=\"td_header\">Server</td></tr><tr><td colspan=\"2\" class=\"bold\">All connections</td><td><!--#srv_c--></td></tr><tr><td colspan=\"2\" class=\"bold\">TX Bytes</td><td><!--#srv_tx--></td></tr><tr><td colspan=\"2\" class=\"bold\">RX Bytes</td><td><!--#srv_rx--></td></tr></table><p class=\"float_right\"><a href=\"http://stm32f4-discovery.com\">Website by Tilen Majerle</a></p><div class=\"clear\"></div></div></body></html>";

/* style.css file */
const struct fsdata_file file__style_css[] = {{
	NULL, /* Next file */
	(const unsigned char *)"/style.css", /* File name */
	(const unsigned char *)data__style_css,/* File data */
	sizeof(data__style_css), /* Length of file data */
	1,
}};

/* 404.html file */
const struct fsdata_file file__404_html[] = {{
	file__style_css, /* Next file */
	(const unsigned char *)"/404.html", /* File name */
	(const unsigned char *)data__404_html,/* File data */
	sizeof(data__404_html),
	1,
}};

/* index.sfile */
const struct fsdata_file file__index_shtml[] = {{
	file__404_html, /* Next file */
	(const unsigned char *)"/index.shtml", /* File name */
	(const unsigned char *)data__index_html, /* File data */
	sizeof(data__index_html), /* Length of file data */
	1,
}};

#endif

#endif
