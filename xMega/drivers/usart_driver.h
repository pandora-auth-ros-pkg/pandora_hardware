/*!
 *	\file usart_driver.h
 *	\brief USART driver Header File
 *
 *	Created: 5/6/2014 4:28:29 pm
 *	Author: Thomas / Panayiotou Konstantinos
 *	Email: glagloui@gmail.com / klpanagi@auth.gr
 */ 


#ifndef USART_DRIVER_H_
#define USART_DRIVER_H_

#include "system.h"

/* <ASCII CONTROL CHARACTERS> */
#define SOH		0x01	/*! <start of heading> */
#define STX		0x02	/*! <start of text> */
#define ETX		0x03	/*! <end of text> */
#define EOT		0x04	/*! <end of transmission> */
#define UACK	0x06	/*! <usart acknowledge> */
#define LF		0x0a	/*! <line feed> */
#define UNAk	0x15	/*! <usart negative acknowledge> */
#define GS		0x1D	/*! <group separator> */
#define RS		0x1E	/*! <record separator> */
#define US		0x1F	/*! <unit separator> */
#define NPFF	0x0C	/*! <New page, Form Feed> */

#define WaitAckNakTimeOut	10000	/*!< Max Time for ACK/NAK response (ms) */  


void init_uart(void);
void usart_putc(char c);
char usart_getc(void);
void usart_sendstring(char *s);

#endif /* USART_H_ */