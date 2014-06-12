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

/*!	\def Tick200us
 *	\brief Real Time FSM service timer 200us tick Mask.
 */
#define Tick200us	0x01
/*!	\def Tick400us
 *	\brief Real time FSM service timer 400us tick Mask.
 */
#define Tick400us	0x02
/*!	\def Tick1ms
 *	\brief Real Time FSM service timer 1ms tick Mask.
 */
#define Tick1ms		0x04
/*!	\def Tick200ms
 *	\brief Real Time FSM service timer 200ms tick Mask.
 */
#define Tick200ms	0x08
/*!	\def Tick5ms
 *	\brief Real Time FSM service timer 5ms tick Mask.
 */
#define Tick5ms		0x10
/*!	\def Tick10ms
 *	\brief Real Time FSM service timer 10ms tick Mask.
 */
#define Tick10ms	0x20
/*!	\def Tick50ms
 *	\brief Real Time FSM service timer 50ms tick Mask.
 */
#define Tick50ms	0x40
/*!	\def Tick100ms
 *	\brief Real Time FSM service timer 100ms tick Mask.
 */
#define Tick100ms	0x80

/*!	\def TWIWatchdogTimeOut
 *	\brief TWI software Wathcdog Time out value (500ms) 
 */
#define TWIWatchdogTimeOut	500			


extern volatile uint8_t CurrentGroup;
extern volatile uint16_t ticks;
extern volatile uint8_t	TCTimingFlags;
extern volatile uint8_t	NextSensor;

/*!	\fn uint8_t SetSensorInReviveState(uint8_t SensorNumber)
 *	\brief
 *	Sets I2C sensor in revive state and resets I2C bus.
 *	Increases Communication Errors counter for the specific sensor.
 *	Punish that sensor on a wait state.
 */
uint8_t SetSensorInReviveState(uint8_t SensorNumber);

/*!	\fn uint8_t I2CSensorFSMCore(uint8_t SensorNumber)
 *	\brief FSM core for I2C communications. Lasts 15usec MAX.
 *	\param SensorNumber Current Sensor Number.
 */
uint8_t I2CSensorFSMCore(uint8_t SensorNumber);

/*!	\fn uint8_t SRF05tinyFSM(uint8_t SensorNumber)
 *	\brief SRF05 Sonars Service FSM.
 *	\param SensorNumber Current Sensor Number.
 */
uint8_t SRF05tinyFSM(uint8_t SensorNumber);
void WaitI2CStatus(uint8_t SensorNumber);

/*!	\fn uint8_t TWIResultHandler(uint8_t SensorNumber)
 *	\brief I2C bus results handler. Checks for bus errors and data transmission errors.
 *	\param SensorNumber Current Sensor Number.
 */
uint8_t TWIResultHandler(uint8_t SensorNumber);

/*!	\fn void SensorWaitTimeHandler(void)
 *	\brief
 *	Handle waiting time for "punished" sensors on I2C bus
 *	Decrease WaitCount1/ComRepeats/TimeForNextCycle if !0 
 */
void SensorWaitTimeHandler(void);

/*!	\fn void TWIWatchdog(void)
 *	\brief Two Wire Interface (I2C) software watchdog.
 *	
 */
void TWIWatchdog(void);

/*!	\fn void GroupHandler(void)
 *	\brief
 *	Check if all sensors in "CurrentGroup" ended measuring or are in revive state. 
 *	If true, initialize next group to be serviced.
 */
void GroupHandler(void);

/*!	\fn void CreateArraySameGroupIDs(void)
 *	\brief
 *	Create an array with the IDs of sensors that belong in CurrentGroup (this group will begin measurements) 
 *	and clear 'Used' flag of Sensors in this group.
 */
void CreateArraySameGroupIDs(void);

/*!	\fn uint8_t FindFirstAvailableSensor(uint8_t SensorNumber)
 *	Select next available sensor to be serviced on I2C bus.
 *	\param SensorNumber Current Sensor Number.
 */
uint8_t FindFirstAvailableSensor(uint8_t SensorNumber);

/*!     \fn void pandora_fsm(void)
 *	\brief Timeslots Services Handling and main FSM routine.
 */
void pandora_fsm(void);
#endif /* fsm_H_ */
