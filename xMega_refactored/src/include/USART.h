/*
 * USART.h
 *
 * Created: 3/22/2014 5:17:21 PM
 *  Author: klpanagi
 */ 


#ifndef USART_H_
#define USART_H_

#include "system.h"

//ASCII CONTROL CHARACTERS
#define SOH		0x01	//start of heading
#define STX		0x02	//start of text
#define ETX		0x03	//end of text
#define EOT		0x04	//end of transmission
#define UACK	0x06	//usart acknowledge
#define LF		0x0a	//line feed
#define UNAk	0x15	//usart negative acknowledge
#define GS		0x1D	//group separator
#define RS		0x1E	//record separator
#define US		0x1F	//unit separator
#define NPFF	0x0C	//New page, Form Feed

#define WaitAckNakTimeOut	10000	//ms

#define PC_USART USARTD0
/* USART buffer defines. */

/* \brief  Receive buffer size: 2,4,8,16,32,64,128 or 256 bytes. */
#define USART_RX_BUFFER_SIZE 16
/* \brief Transmit buffer size: 2,4,8,16,32,64,128 or 256 bytes */
#define USART_TX_BUFFER_SIZE 16
/* \brief Receive buffer mask. */
#define USART_RX_BUFFER_MASK ( USART_RX_BUFFER_SIZE - 1 )
/* \brief Transmit buffer mask. */
#define USART_TX_BUFFER_MASK ( USART_TX_BUFFER_SIZE - 1 )


#if ( USART_RX_BUFFER_SIZE & USART_RX_BUFFER_MASK )
#error RX buffer size is not a power of 2
#endif
#if ( USART_TX_BUFFER_SIZE & USART_TX_BUFFER_MASK )
#error TX buffer size is not a power of 2
#endif


/* \brief USART transmit and receive ring buffer. */
typedef struct USART_Buffer
{
	/* \brief Receive buffer. */
	volatile uint8_t RX[USART_RX_BUFFER_SIZE];
	/* \brief Transmit buffer. */
	volatile uint8_t TX[USART_TX_BUFFER_SIZE];
	/* \brief Receive buffer head. */
	volatile uint8_t RX_Head;
	/* \brief Receive buffer tail. */
	volatile uint8_t RX_Tail;
	/* \brief Transmit buffer head. */
	volatile uint8_t TX_Head;
	/* \brief Transmit buffer tail. */
	volatile uint8_t TX_Tail;
} USART_Buffer_t;


/*! \brief Struct used when interrupt driven driver is used.
*
*  Struct containing pointer to a usart, a buffer and a location to store Data
*  register interrupt level temporary.
*/
typedef struct Usart_and_buffer
{
	/* \brief Pointer to USART module to use. */
	USART_t * usart;
	/* \brief Data register empty interrupt level. */
	USART_DREINTLVL_t dreIntLevel;
	/* \brief Data buffer. */
	USART_Buffer_t buffer;
} USART_data_t;

uint16_t	PCTXWaitAckNakTimer;

void init_Rs232(USART_data_t *usart_data);
void init_uart(void);
void usart_putc(char c);
char usart_getc(void);
void usart_sendstring(char *s);
void test_rs232();
void echo_mode();
void echo_mode_string();

#endif /* USART_H_ */