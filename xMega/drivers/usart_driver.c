
/*!
 *	\file usart_driver.c
 *	\brief USART driver Source file
 *
 *	Created: 5/6/2014 1:01:48  pm
 *	Author: Panayiotou Konstantinos
 *	Email: glagloui@gmail.com
 */ 


#include "usart_driver.h"

void init_uart(void)
{
	PORTD.OUTSET = PIN3_bm;     /* <TX high> */
	PORTD.DIRSET = PIN3_bm;     /* <And output> */
	PORTD.DIRCLR = PIN2_bm;     /* <RX is input> */
	RS232.CTRLA = 0x00;		/* <0x00:No IRQ, 0x01:Enable Data Register Empty Interrupt Level > */
	RS232.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	RS232.CTRLC = 3|8;
	RS232.BAUDCTRLA = 131;	/* <12 9600//137 28800// FIXED 135 for 57600  //FIX 131 FOR 115200 WITH CLK2X=0 (((CPU_SPEED) / (16)) / USARTF0BAUDRATE) - 1; //value=207 for CLKper CPU_SPEED=32MHz and desired speed USARTF0BAUDRATE> */
	RS232.BAUDCTRLB = 0xD0;	/* <4 9600//f0=-1 28800//D0=(-03) 115200//E0=(-2) 57600> */
}

/*!	\fn void usart_putc(char c)
  *	\brief Brief Function Description
 *
 * This function gets as input a character and sends it over USART
 */
void usart_putc(char c)
{
	while (!(RS232.STATUS & USART_DREIF_bm));
	RS232.DATA = c;
}

/*!< Brief Function Description
 *
 *  Returns a char from USART RX Buffer.
 *	BLocking Function waiting for data to arrive in USART Module 
 */
char usart_getc(void)
{
	while (!(RS232.STATUS & USART_RXCIF_bm));
	return RS232.DATA;
}

/*!< Brief Function Description
 *
 *	Send a series of characters over USART.
 * Input: Character array to send.
 * Uses usart_putc() to send the data.
 */
void usart_sendstring(char *s)
{
	while (*s) {
		usart_putc(*s++);
	}
}