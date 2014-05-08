/*
 * USART.c
 *
 * Created: 1/6/2012 1:01:48 πμ
 *  Author: Thomas
 */ 


#include "usart_driver.h"

void init_uart(void)
{
	PORTD.OUTSET = PIN3_bm;     // TX high
	PORTD.DIRSET = PIN3_bm;     // and output
	PORTD.DIRCLR = PIN2_bm;     // RX is input
	USARTD0.CTRLA = 0x00;		// 0x00:No IRQ, 0x01:Enable Data Register Empty Interrupt Level 
	USARTD0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;// | USART_CLK2X_bm;
	USARTD0.CTRLC = 3|8;
	USARTD0.BAUDCTRLA = 131;	// 12 9600//137 28800// FIXED 135 for 57600  //FIX 131 FOR 115200 WITH CLK2X=0 (((CPU_SPEED) / (16)) / USARTF0BAUDRATE) - 1; //value=207 for CLKper CPU_SPEED=32MHz and desired speed USARTF0BAUDRATE
	USARTD0.BAUDCTRLB = 0xD0;	//4 9600//f0=-1 28800//D0=(-03) 115200//E0=(-2) 57600 
}

void usart_putc(char c)
{
	while (!(USARTD0.STATUS & USART_DREIF_bm));
	USARTD0.DATA = c;
}

char usart_getc(void)
{
	while (!(USARTD0.STATUS & USART_RXCIF_bm));
	return USARTD0.DATA;
}

void usart_sendstring(char *s)
{
	while (*s) {
		usart_putc(*s++);
	}
}