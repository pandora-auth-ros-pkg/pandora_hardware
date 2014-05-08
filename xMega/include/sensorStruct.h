/*
 * sensorStruct.h
 *
 * Created: 3/12/2012 3:54:41 μμ
 *  Author: dikan
 */ 
#ifndef __SENSORSTRUCT_H__
#define __SENSORSTRUCT_H__

#include <stdint.h>

#define SRF05tiny_WRITE_LEN		1	//1st byte:command , 2nd byte:data// command1:data contains delay time to trigger sonar, command2:send me status (1 byte) and data (3 bytes, sonar time + IR value)
#define SRF05tiny_READ_LEN		4	//1st byte STATUS, 2nd..3rd byte reange, 4th byte IR ADC conv Value

typedef union{
	uint8_t SensStatusVal;
	struct
	{	uint8_t Used			:1;	//00000001	;Used in this Group measurement flag
		uint8_t ValueOutOfRange	:1;	//00000010	;Out of range flag
		uint8_t TimeOut			:1;	//00000100	;time-out flag
		uint8_t NewData			:1;	//00001000	;new value flag
		uint8_t NewDataCopied	:1; //00010000	;new Data copied, This will happen only if NewData=1 && PCTX_Busy=0, Then NewData=0
		uint8_t I2CComErr		:1;	//00100000	;sensor com error flag
		uint8_t WaitTrigger		:1;	//01000000	;Wait trigger
		uint8_t TriggerDone		:1;	//10000000	;Trigger done
	};
	} SensorStatusFlags;


typedef struct{
 	uint8_t type;					// static. tpa, srf05tiny, srf235 etc Sensor_Names
	uint8_t SensorID;				// static. counting purposes
	uint8_t SlaveAddress;			// static. unique i2c address
	uint8_t Groupmember;			// static. 0:No group, 1,2,3... means member of this group
	uint8_t *WriteBufer;			// static. predefined -for this project- values to be sent to the sensor
	uint8_t WriteBufLen;			// static. length of ^ 
	uint8_t *ReadBufer;				// dynamic. buffer with sensor measurements data
	uint8_t ReadBufLen;				// static. length of ^
	uint16_t StartIndexInPCTXBuffer;// static. offset (pointer) in PCTXBuffer, where the data of this sensor will start to be copied
	uint8_t PCTXLen;				// static. shows the size of data to be Transmited to PC: PCTXLEN+RS+(ReadBufLen+SensorID+type+SlaveAddress+CurrentState+Status)*2 (ASCII)
	uint8_t CurrentState;			// dynamic. this is the state that will be (or is currently) executed for this sensor
	uint8_t RollBackState;			// dynamic. this will be the state that the sensor will enter if WaitTimeout occurs
	SensorStatusFlags Status;		// dynamic. status flags. see SensorStatusFlags union for more details
	uint8_t NumberOfMeasurement;	// dynamic. used in tpa16 where 16 communications are needed for collecting 16 measurements. this variable counts the measurements
	uint16_t WaitCount1;			// dynamic. used for countdown /ms as I2CTimeOutTimer and ReviveTimeOutTimer
	uint16_t WaitCount2;			// dynamic. used for countdown /ms while in IdleState1 (delay before next communication after NACK received)
	uint16_t ComRepeats;			// dynamic. countdown /ms maximum duration of communications repeats until ACK arrives
	uint16_t ReviveStateDuration;	// static. (ms) how long is the duration for this sensor type in Revive state
	uint16_t TimeForNextCycle;		// static. (ms) minimum time for the next reading of measurements (minimum cycle). After active cycles, remaining time will be spend in IdleState
	uint16_t IdleState1Duration;	// static. (ms) delay for this sensor-type between repeating communication after NACK. Will be loaded in WaitCount2 after Nack 
	uint8_t ComErrorCounts;			// dynamic. Increased every time sensor arrives in ReviveState
	uint8_t visitstoread;			// not used... 
	 
}sensor_struct;

#endif