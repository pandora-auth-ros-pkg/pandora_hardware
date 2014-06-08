/*!
 *	\file sensors.h
 *	\brief System Sensors Services Header file.
 *
 *	Created: 5/9/2014 11:46:04 PM
 *	Author: dikan / Panayiotou Konstantinos
 *	Email: glagloui@gmail.com
 */ 

#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <stdint.h>
#include <stdlib.h>

#include "encoder.h"

/*!	\def SRF05tiny_WRITE_LEN
 *	\brief Sonar write length 1st byte:command , 2nd byte:data// command1:data contains delay time to trigger sonar, command2:send me status (1 byte) and data (3 bytes, sonar time + IR value)
 */
#define SRF05tiny_WRITE_LEN		1
/*!	\def SRF05tiny_READ_LEN
 *	\brief Sonar read length. 1st byte STATUS, 2nd..3rd byte range, 4th byte IR ADC convertion Value 
 */
#define SRF05tiny_READ_LEN		4	
/*!	\def SRF05tinyWaitForBusTimeOut
 *	\brief Value in milliseconds 
 */
#define SRF05tinyWaitForBusTimeOut			15			
/*!	\def SRF05tinyComRepeatTimeOut
 *	\brief Value in milliseconds 
 */
#define SRF05tinyComRepeatTimeOut			30			
/*!	\def SRF05tinyWaitReviveSensorCounts
 *	\brief Value in milliseconds 
 */
#define SRF05tinyWaitReviveSensorCounts		500			
#define SRF05tinyCommunicationCycle			100			
/*!	\def SRF05tinyDelayAfterNack
 *	\brief Value in milliseconds 
 */
#define SRF05tinyDelayAfterNack				5			

/*!	\union SensorStatusFlags
 */
typedef union{
	uint8_t SensStatusVal;
	struct
	{	uint8_t Used			:1;	/* <00000001	;Used in this Group measurement flag> */
		uint8_t ValueOutOfRange	:1;	/* <00000010	;Out of range flag> */
		uint8_t TimeOut			:1;	/* <00000100	;time-out flag> */
		uint8_t NewData			:1;	/* <00001000	;new value flag> */
		uint8_t NewDataCopied	:1; /* <00010000	;new Data copied, This will happen only if NewData=1 && PCTX_Busy=0, Then NewData=0> */
		uint8_t I2CComErr		:1;	/* <00100000	;sensor com error flag> */
		uint8_t WaitTrigger		:1;	/* <01000000	;Wait trigger> */
		uint8_t TriggerDone		:1;	/* <10000000	;Trigger done> */
	};
} SensorStatusFlags;

/*!	\struct i2c_sensor_struct
 *	\brief i2c sensors general info
 */ 
typedef struct{
	uint8_t type;					/*!< static. Sensor Type */
	uint8_t SensorID;				/*!< static. Sensor ID number */
	uint8_t SlaveAddress;			/*!< static. Unique i2c address */
	uint8_t Groupmember;			/*!< static. 0:No group, 1,2,3... means member of this group */
	uint8_t *WriteBufer;			/*!< static. predefined -for this project- values to be sent to the sensor */
	uint8_t WriteBufLen;			/*!< static. Sensor Tx Buffer Length */
	uint8_t *ReadBufer;				/*!< dynamic. buffer with sensor measurements data */
	uint8_t ReadBufLen;				/*!< static. Sensor Rx Buffer Length. */
	uint16_t StartIndexInPCTXBuffer;/*!< static. offset (pointer) in PCTXBuffer, where the data of this sensor will start to be copied */
	uint8_t PCTXLen;				/*!< static. shows the size of data to be Transmited to PC: PCTXLEN+RS+(ReadBufLen+SensorID+type+SlaveAddress+CurrentState+Status)*2 (ASCII) */
	uint8_t CurrentState;			/*!< dynamic. this is the state that will be (or is currently) executed for this sensor */
	uint8_t RollBackState;			/*!< dynamic. this will be the state that the sensor will enter if WaitTimeout occurs */
	SensorStatusFlags Status;		/*!< dynamic. status flags. see SensorStatusFlags union for more details */
	uint8_t NumberOfMeasurement;	/*!< dynamic. used in tpa16 where 16 communications are needed for collecting 16 measurements. this variable counts the measurements */
	uint16_t WaitCount1;			/*!< dynamic. used for countdown /ms as I2CTimeOutTimer and ReviveTimeOutTimer */
	uint16_t WaitCount2;			/*!< dynamic. used for countdown /ms while in IdleState1 (delay before next communication after NACK received) */
	uint16_t ComRepeats;			/*!< dynamic. countdown /ms maximum duration of communications repeats until ACK arrives */
	uint16_t ReviveStateDuration;	/*!< static. (ms) how long is the duration for this sensor type in Revive state */
	uint16_t TimeForNextCycle;		/*!< static. (ms) minimum time for the next reading of measurements (minimum cycle). After active cycles, remaining time will be spend in IdleState */
	uint16_t IdleState1Duration;	/*!< static. (ms) delay for this sensor-type between repeating communication after NACK. Will be loaded in WaitCount2 after Nack */
	uint8_t ComErrorCounts;			/*!< dynamic. Increased every time sensor arrives in ReviveState */
	uint8_t visitstoread;			/*!< Not in Use */
	
}i2c_sensor_struct;

/*!  \struct batteries_struct
 *	\brief Batteries voltage level measurements structure.
 *	Vars for voltage levels measurements of Electronics-PSU battery and Motor-Servo battery.
 */
typedef struct{
	uint16_t batteryVoltage_PSU;
	uint16_t batteryVoltage_MOTOR;
}batteries_struct;

/*!	\enum Sensor_Types
 *	\brief  Enumerated sensor types on xMega.b
 *	Currently own SRF05 Sonar Sensors, Battery voltage level Measurements and Rotary Encoder.
 */
enum Sensor_Types{
	SRF05tinym,						
	Battery,						
	RotaryEncoder					
};

/*!	\enum  SensorStates
 *	\brief Enumerated States for I2C sensors
 */
enum SensorStates{
	ReviveState,					/*! Revive State <0> */
	FirstRunState,					/*! First Run State <1> */
	FirstCommunicationState,		/*! First Communication State <2> */
	FirstCommunicationStateWait,	/*! First Communication Wait State <3> */
	SecondCommunicationState,		/*! Second Communication State <4> */
	SecondCommunicationStateWait,	/*! Second Communication Wait State <5> */
	IdleState1,						/*! Idle State 1 <6> */
	IdleState2,						/*! Idle State 2 <7> */
	IdleState						/*! IdleState <8> */
};

/*!	\def I2CSENSNUM 
 *	\brief Number of sensors on i2c bus. Currently 4 Sonars 
 */
#define I2CSENSNUM 4	
/*!	\def SENSORGROUPS
 *	\brief I2C sensors groups. Group0 is group-free. 3 means group0 + 3 more groups  
 */	
#define SENSORGROUPS 2	
/*!	\def SENSNUM 
 *	\brief (4*Sonars on I2C bus) (Batteries as 1 sensor) (1*encoder) 
 */
#define SENSNUM 6			

/*!	\var I2CAddressLUT  Look Up Table holding information about the sensors on I2C bus. 
 *	Columns: 0 = SensorType. 1 = Address on. 2 = Group that sensor belongs.
 */  
extern const int8_t I2CAddressLUT[I2CSENSNUM][3];	
/*! \var SensorModule An array of i2c_sensor_struct objects. Holding i2c sensors info. */
extern i2c_sensor_struct SensorModule[I2CSENSNUM];
extern batteries_struct _batteries;
extern uint16_t BatteryPSU;
extern uint16_t BatteryMotor;

extern	encoder_struct _encoder;
extern uint16_t encoder_value;

uint16_t CreateSensorStructs(void);
#endif