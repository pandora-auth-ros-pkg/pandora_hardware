/*!
 *	\file fsm.h
 *	\brief System FSM services Header file.
 *
 * Created: 5/9/2014 8:27:15 PM
 *  Author: dikan / Panayiotou Konstantinos
 * Email:	glagloui@gmail.com 	
 */ 
#ifndef fsm_H_
#define fsm_H_

#include <stdint.h>
#include "system.h"
#include "sensors.h"
#include "pcInterface.h"

#define Tick200us	0x01
#define Tick400us	0x02
#define Tick1ms		0x04
#define Tick200ms	0x08
#define Tick5ms		0x10
#define Tick10ms	0x20
#define Tick50ms	0x40
#define Tick100ms	0x80


#define TWIWatchdogTimeOut	500			/*!< \def TWI software Wathcdog Time out value (500ms) */


extern volatile uint8_t CurrentGroup;
extern volatile uint16_t ticks;
extern volatile uint8_t	TCTimingFlags;
extern volatile uint8_t	NextSensor;

uint8_t SetSensorInReviveState(uint8_t SensorNumber);
uint8_t I2CSensorFSMCore(uint8_t SensorNumber);
uint8_t SRF05tinyFSM(uint8_t SensorNumber);
void WaitI2CStatus(uint8_t SensorNumber);
uint8_t TWIResultHandler(uint8_t SensorNumber);
void SensorWaitTimeHandler(void);
void TWIWatchdog(void);
void GroupHandler(void);
void CreateArraySameGroupIDs(void);
uint8_t FindFirstAvailableSensor(uint8_t SensorNumber);

void pandora_fsm(void);
#endif /* fsm_H_ */
