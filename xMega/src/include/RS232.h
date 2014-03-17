/*
 * RS232.h
 *
 * Created: 3/11/2014 11:55:12 PM
 *  Author: Klpanagi
 */ 


#ifndef RS232_H_
#define RS232_H_

	#include "include/sensors.h"

	//ASCII CONTROL CHARACTERS
	#define SOH		0x01			//start of heading
	#define STX		0x02			//start of text
	#define ETX		0x03			//end of text
	#define EOT		0x04			//end of transmission
	#define UACK	0x06			//usart acknowledge
	#define LF		0x0a			//line feed
	#define UNAK	0x15			//usart negative acknowledge
	#define GS		0x1D			//group separator
	#define RS		0x1E			//record separator
	#define US		0x1F			//unit separator
	#define NPFF	0x0C			//New page, Form Feed

	#define WaitAckNakTimeOut	10000	//ms
	#define REQUEST_COM '\a'		//The bell character (BEL)
	#define CONF_PACKET_SIZE 4		//configuration packet size

void rs232_initialization();
void pc_start_com();
void send_conf_packet(sensors_info data);
void testing_com();


#endif /* RS232_H_ */

 
//extern void test_rs232_com();

