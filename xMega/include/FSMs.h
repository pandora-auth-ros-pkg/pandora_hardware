/*
 * FSMs.h
 *
 * Created: 4/12/2012 4:30:37 μμ
 *  Author: dikan
 */ 


#ifndef FSMS_H_
#define FSMS_H_

#include <stdint.h>
#include "system.h"

enum SensorStates{
	ReviveState,					//0
	FirstRunState,					//1
	FirstCommunicationState,		//2
	FirstCommunicationStateWait,	//3
	SecondCommunicationState,		//4
	SecondCommunicationStateWait,	//5
	IdleState1,						//6
	IdleState2,						//7
	IdleState						//8
	};

#define SRF05tinyWaitForBusTimeOut			15			//ms
#define SRF05tinyComRepeatTimeOut			30			//ms
#define SRF05tinyWaitReviveSensorCounts		500			//ms
#define SRF05tinyCommunicationCycle			100			//ms
#define SRF05tinyDelayAfterNack				5			//ms

#define TWIWatchdogTimeOut					500			//ms


//sensor_struct SensorModule[SENSNUM];

uint16_t CreateSensorStructs(void);

uint8_t SetSensorInReviveState(uint8_t SensorNumber);
	
uint8_t SensorFSMCore(uint8_t SensorNumber);

uint8_t SRF05tinyFSM(uint8_t SensorNumber);

void WaitI2CStatus(uint8_t SensorNumber);

uint8_t TWIResultHandler(uint8_t SensorNumber);

void SensorWaitTimeHandler(void);

void TWIWatchdog(void);

void CheckSensorBuffersForNewData(void);

uint16_t PutSensorSeparatorInPCTXBuffer(uint16_t PCTXCurrentPointer);

uint16_t CopySensorHeaderToPCTXBuffer(uint8_t SensorNumber, uint16_t PCTXCurrentPointer);

uint16_t CopyDataToPCTXBuffer(uint8_t SensorNumber, uint16_t PCTXCurrentPointer);

void GroupHandler(void);

void CreateArraySameGroupIDs(void);

uint8_t FindFirstAvailableSensor(uint8_t SensorNumber);

#endif /* FSMS_H_ */
