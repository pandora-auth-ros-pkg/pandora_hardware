/*
 * general_include.h
 *
 * Created: 1/6/2012 2:09:50 πμ
 *  Author: Thomas
 */ 


#ifndef GENERAL_INCLUDE_H_
#define GENERAL_INCLUDE_H_

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

/*! CPU speed 32MHz, TWI BAUDRATE 100kHz and Baudrate Register Settings */
//#define F_CPU			32000000UL
#define CPU_SPEED       32000000UL
#define BAUDRATE		75000UL					//BEST MEASURED 70000UL
#define TWI_BAUDSETTING TWI_BAUD(CPU_SPEED, BAUDRATE)
#define USARTF0BAUDRATE 115200

static const char nybble_chars[] = "0123456789ABCDEF";

extern TWI_Master_t twiMaster;    /*!< TWI master module. */

void clock_init(void);
void init_uC_clock(void);

#endif /* GENERAL_INCLUDE_H_ */