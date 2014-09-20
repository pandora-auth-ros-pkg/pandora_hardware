/*!
 *	\file system.h
 *	\brief System Configurations and Initializations Header file
 *
 *	Created: 1/6/2012 2:09:50 πμ
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

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <stdint.h>

#include "TC_driver.h"
#include "avr_compiler.h"
#include "clksys_driver.h"
#include "twi_master_driver.h"
#include "wdt_driver.h"
#include "usart_driver.h"
#include "sensors.h"

#define BOARD_LED_PORT PORTQ
#define BOARD_LED_PIN 2
/*!	\def CPU_SPEED
 *	\brief uController cpu speed definition
 */
#define CPU_SPEED       32000000UL
/*!	\def BAIDRATE 
 *	\brief TWI Baudrate value (100kHz)
 */
#define BAUDRATE		75000UL					//BEST MEASURED 70000UL
#define TWI_BAUDSETTING TWI_BAUD(CPU_SPEED, BAUDRATE)
/*!	\def USARTD0BAUDRATE
 *	\brief USART baudrate definition
 */
#define USARTD0BAUDRATE 115200	
#define RS232 USARTD0

/*!    \def ADC_OFFSET
	\brief Precalculated ADC offset to be value 153 + 7 =160
*/ 		
#define ADC_OFFSET 160					 

static const char nybble_chars[] = "0123456789ABCDEF";

extern TWI_Master_t twiMaster;   

/*!	\fn void clock_init(void)
 *	\brief xMega System clock configurations and startup.
 *	Written in Assembly.
 */
void clock_init(void);

/*!	\fn void init_uC_clock(void)
 *	\brief xMega System clock configurations and startup.
 *	Written using atmel clock drivers.
 */
void init_uC_clock(void);

/*!	\fn void _startup_system_init(void)
 *	\brief Initialize and configure uController Modules:
 *	uClock.
 *	Watchdog timer.
 *	Timer 0 for Event capturing.
 *	UART. (PC communication)
 *	I2C Interface.
 *	Two Analog to Digital Signal Inputs. (ADCB)
 *	Encoder.
 */
void _startup_system_init(void);
#endif 
