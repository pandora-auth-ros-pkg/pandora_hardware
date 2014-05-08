/*
 * PandoraFSM.h
 *
 * Created: 1/1/2013 1:45:32 πμ
 *  Author: dikan
 */ 


#ifndef PANDORAFSM_H_
#define PANDORAFSM_H_

#include "system.h"

// Globals
uint16_t PCTXBufferLen;
uint16_t PCTXBufferCrc;

#define Tick200us	0x01
#define Tick400us	0x02
#define Tick1ms		0x04
#define Tick200ms	0x08
#define Tick5ms		0x10
#define Tick10ms	0x20
#define Tick50ms	0x40
#define Tick100ms	0x80

volatile uint8_t	TCTimingFlags;
volatile uint8_t	NextSensor;
volatile uint16_t	PCTXPointer;
volatile uint8_t	PCTXFlags;

#define PCTX_Busy		0x01
#define PCTX_WaitAckNaK	0x02
#define PCTX_DebugMode	0x04
#define PCTX_DebugMode2	0x08

#define PCTXLenDebugMode 15

//#define _PCTX_DEBUG_ON
//#define _GROUP_HANDLING_DEBUG_ON

uint16_t batteryVoltage_PSU, batteryVoltage_MOTOR;
//uint16_t gADC_CH0_ZeroOffset,gADC_CH1_ZeroOffset;		// Calibration
uint8_t *PCTXBuffer;

#endif /* PANDORAFSM_H_ */