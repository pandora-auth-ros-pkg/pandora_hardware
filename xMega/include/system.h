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

/*! CPU speed 32MHz, TWI BAUDRATE 100kHz and Baudrate Register Settings */
#define CPU_SPEED       32000000UL				/*! <uController CPU speed definition> */
#define BAUDRATE		75000UL					//BEST MEASURED 70000UL
#define TWI_BAUDSETTING TWI_BAUD(CPU_SPEED, BAUDRATE)
#define USARTD0BAUDRATE 115200					/*! <UART Baudrate definition> */

#define RS232 USARTD0							

static const char nybble_chars[] = "0123456789ABCDEF";

extern TWI_Master_t twiMaster;   

void clock_init(void);
void init_uC_clock(void);
void _startup_system_init(void);
#endif 