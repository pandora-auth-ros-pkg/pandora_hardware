/*!
 *	\file usart_driver.h
 *	\brief USART driver Header File
 *
 *	Created: 5/6/2014 4:28:29 pm
 *	Author: Panayiotou Konstantinos
 *	Email: klpanagi@gmail.com / klpanagi@auth.gr
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


#ifndef USART_DRIVER_H_
#define USART_DRIVER_H_

#include "system.h"

/************************************************************/
/*************** <ASCII CONTROL CHARACTERS> *****************/ 	
	/*! \def SOH
	 *  \brief <Start of Heading> Character
	 */  
	#define SOH		0x01
	/*! \def STX
	 *  \brief <Start of Text> Character
	 */  
	#define STX		0x02
	/*! \def ETX
	 *  \brief <End of Text> Character
	 */  
	#define ETX		0x03
	/*! \def EOT
	 *  \brief <End of Transmission> Character
	 */  
	#define EOT		0x04
	/*! \def UACK
	 *  \brief <USART Acknowledge> Character
	 */  
	#define UACK	0x06
	/*! \def LF
	 *  \brief <Line Feed> Character
	 */  
	#define LF		0x0a	
	/*! \def UNAk
	 *  \brief <USART Negative Acknowledge> Character
	 */  
	#define UNAk	0x15	
	/*! \def GS
	 *  \brief <Group Seperator> Character
	 */  
	#define GS		0x1D
	/*! \def RS
	 *  \brief <Record Seperator> Character
	 */  
	#define RS		0x1E
	/*! \def US
	 *  \brief <Unit Seperator> Character
	 */  
	#define US		0x1F
	/*! \def NPFF
	 *  \brief <New Page, Form Feed> Character
	 */  
	#define NPFF	0x0C
/**************************************************************/

/*!	\def WaitAckNakTimeOut
 *	\brief < Max Time for ACK/NAK response (ms) >
 */	  
#define WaitAckNakTimeOut	10000

/*!	\fn void init_uart(void)
 * 	\brief Initialization routine for uart.
 */
void init_uart(void);

/*!	\fn void usart_putc(char c)
 *	\brief Transmits a character over USART 
 *	\param c Character to be send.
 *
 * 	This function gets as input a character and sends it over USART.
 */
void usart_putc(char c);

/*! \fn  char usart_getc(void)
 *	\brief Returns a char from USART RX Buffer (Blocking routine)
 *	
 *	BLocking Function waiting for data to arrive in USART Module 
 */
char usart_getc(void);

/*! \fn void usart_sendstring(char *s)
 *	\brief Sends a series of characters (string) over USART
 *	\param s Characer Vector (String)
 *
 *	Uses usart_putc() to send the data.
 */
void usart_sendstring(char *s);

#endif /* USART_H_ */
