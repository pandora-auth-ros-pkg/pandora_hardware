/*!
 *	\file pcInterface.h
 *	\brief Pc communication Interface Header file
 *
 *	Created: 5/9/2014 3:24:18 PM
 *	Author: Panayiotou Kostantinos
 *	Email: glagloui@gmail.com
 */ 


#ifndef PCINTERFACE_H_
#define PCINTERFACE_H_

#include "system.h"
#include "sensors.h"

/*!	\def PCTX_Busy
 *	\brief Busy transmitting a data packet. Mask to detect data packet transmission state
 */
#define PCTX_Busy		0x01	
/*!	\def PCTX_WaitAckNaK
 *	\brief Mask used for Waiting Ack/Nak State
 */
#define PCTX_WaitAckNaK	0x02
/*!	\def PCTX_DebugMode
 *	\brief Debug Mode ON Mask.
 */
#define PCTX_DebugMode	0x04
/*!	\def PCTX_DebugMode2
 *	\brief Debug Mode 2 ON Mask.
 */
#define PCTX_DebugMode2	0x08
/*!	\def PCTXLenDebugMode
 *	\brief PcTx Buffer Length in debug mode.
 */	
#define PCTXLenDebugMode 15	

extern uint16_t PCTXBufferLen;
extern uint16_t PCTXBufferCrc;

extern volatile uint16_t	PCTXPointer;
extern volatile uint8_t	PCTXFlags;
extern uint8_t *PCTXBuffer;
extern uint16_t PCTXWaitAckNakTimer;

/*!	\fn void data_transmitService(void)
 *	\brief Data transmission service routine. Checks UART data transmission availability and sends ONE byte from PCTXBUFFER if true;
 */
extern void data_transmitService(void);
extern void CheckSensorBuffersForNewData(void);

/*!	\fn uint16_t PutSensorSeparatorInPCTXBuffer(uint16_t PCTXCurrentPointer)
 *	\brief Add Sensor seperator character (LF = 0x0A) into PCTX software buffer.
 *	Length = 1 byte ( = 1 character)
*/
extern uint16_t PutSensorSeparatorInPCTXBuffer(uint16_t PCTXCurrentPointer);

/*!	\fn uint16_t CopySensorHeaderToPCTXBuffer(uint8_t SensorNumber, uint16_t PCTXCurrentPointer)
 *	\brief  Copies I2C sensors Header in PCTX Buffer
 *	Header length (bytes) = 15
*/
extern uint16_t CopySensorHeaderToPCTXBuffer(uint8_t SensorNumber, uint16_t PCTXCurrentPointer);

/*!	\fn uint16_t CopyDataToPCTXBuffer(uint8_t SensorNumber, uint16_t PCTXCurrentPointer)
 *	\brief  Copies I2C sensors Data in PCTX Buffer
 *	Length: 8 bytes per sensor
*/
extern uint16_t CopyDataToPCTXBuffer(uint8_t SensorNumber, uint16_t PCTXCurrentPointer);

/*!	\fn  uint16_t copyBatteriesInfo_PCTXBuffer(uint16_t PCTXCurrentPointer)
 *	\brief Copies Batteries Voltage level Measurements into PCTX software Buffer.
 *	Copy Batteries info + data in PCTXBuffer.
 *	Header = BatterySensorID (3 bytes) + BatterySensorType (3 bytes).
 *	Value = BatteryMotor (3bytes) + BatteryPSU (3 bytes).
 *	LineFeed = LF character (0x0A).
*/ 
extern uint16_t copyBatteriesInfo_PCTXBuffer(uint16_t PCTXCurrentPointer); 

/*!	\fn  uint16_t copyEncoderInfo_PCTXBuffer(uint16_t PCTXCurrentPointer)
 *	\brief Copies Encoder Rotary value into PCTX Software Buffer.
 *	Copy Encoder info + data in PCTXBuffer.	<10 Bytes>
 *	Header = EncoderSensorID (3 bytes) + EncoderSensorType (3 bytes).
 *	Value = Encoder (3bytes) (3*Nybble)
 *	LineFeed = LF character (0x0A).
*/ 
extern uint16_t copyEncoderInfo_PCTXBuffer(uint16_t PCTXCurrentPointer);

/*!	\fn uint16_t calculate_CRC(uint16_t length)
 *	\brief Dummy Data package CRC extraction
 */
uint16_t calculate_CRC(uint16_t length);

#endif /* PCINTERFACE_H_ */