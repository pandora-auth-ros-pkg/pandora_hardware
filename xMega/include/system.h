/*!
 *	\file system.h
 *	\brief System Configurations and Initializations Header file
 *
 *	Created: 1/6/2012 2:09:50 πμ
 *	Author: Panayiotou Konstantinos
 *	Email: glagloui@gmail.com
 */ 


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