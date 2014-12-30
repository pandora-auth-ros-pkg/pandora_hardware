/*
 * sonar.h
 *
 * Created: 5/18/2014 10:22:23 PM
 *  Author: klpanagi
 */ 


#ifndef SONAR_H_
#define SONAR_H_

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "USI_TWI_Slave.h"

#define M_trig 0x80
#define M_read 0x40
#define M_trig_read 0xC0
#define F_CPU 8000000UL
#define SonarNewData	0x01
#define SonarOvrFl		0x02
#define SonarErr		0x04
#define SonarBusy		0x08
#define SonarEchoHi		0x10
#define SonarEchoLo		0x20
#define TriggerPin	(1<<PB3)
#define EchoPin		(1<<PB1)


typedef union{
	uint16_t Range;
	struct{
		uint8_t RangeLo;
		uint8_t RangeHi;
	};
} RangeStruct;

volatile RangeStruct SonarRange;
unsigned char M_Status;// S_Status;
unsigned char Sonar_H, Sonar_L, IR_data;
volatile uint8_t SonarFlags;

#endif /* SONAR_H_ */