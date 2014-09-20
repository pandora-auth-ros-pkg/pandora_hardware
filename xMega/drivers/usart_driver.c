/*!
 *	\file usart_driver.c
 *	\brief USART driver Source file
 *
 *	Created: 5/6/2014 1:01:48  pm
 *	Author: Panayiotou Konstantinos
 *	Email: klpanagi@gmail.com
 */ 

/***********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2014, P.A.N.D.O.R.A. Team.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the P.A.N.D.O.R.A. Team nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * *********************************************************************/


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

void usart_putc(char c)
{
	while (!(RS232.STATUS & USART_DREIF_bm));
	RS232.DATA = c;
}

char usart_getc(void)
{
	while (!(RS232.STATUS & USART_RXCIF_bm));
	return RS232.DATA;
}

void usart_sendstring(char *s)
{
	while (*s) {
		usart_putc(*s++);
	}
}
