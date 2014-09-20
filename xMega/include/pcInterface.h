/*!
 *	\file pcInterface.h
 *	\brief Pc communication Interface Header file
 *
 *	Created: 5/9/2014 3:24:18 PM
 *	Author: Panayiotou Kostantinos / Niarchos Michael
 *	Email: klpanagi@gmail.com
 */

/***********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2014, P.A.N.D.O.R.A. Team.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the P.A.N.D.O.R.A. Team nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * *********************************************************************/



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
