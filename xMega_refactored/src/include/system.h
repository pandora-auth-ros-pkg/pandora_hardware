/*
 * system.h
 *
 * Created: 3/22/2014 5:14:09 PM
 *  Author: klpanagi
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <stdint.h>

/*! CPU speed 32MHz, TWI BAUDRATE 100kHz and Baudrate Register Settings */
//#define F_CPU			32000000UL
#define CPU_SPEED       32000000UL
#define BAUDRATE		75000UL					//BEST MEASURED 70000UL
#define TWI_BAUDSETTING TWI_BAUD(CPU_SPEED, BAUDRATE)
#define USARTF0BAUDRATE 115200

static const char nybble_chars[] = "0123456789ABCDEF";

void clock_init(void);

#endif /* SYSTEM_H_ */