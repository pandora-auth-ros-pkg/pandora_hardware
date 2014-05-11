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

#define PCTX_Busy		0x01	/*!< Busy trasmitting a data packet. Mask to detect data packet transmission state */
#define PCTX_WaitAckNaK	0x02	/*!< Mask used for Waiting Ack/Nak State */ 

#define PCTX_DebugMode	0x04	/*!< IGNORE */
#define PCTX_DebugMode2	0x08	/*!< IGNORE */
#define PCTXLenDebugMode 15		/*!< IGNORE */

extern uint16_t PCTXBufferLen;
extern uint16_t PCTXBufferCrc;

extern volatile uint16_t	PCTXPointer;
extern volatile uint8_t	PCTXFlags;
extern uint8_t *PCTXBuffer;
extern uint16_t PCTXWaitAckNakTimer;

extern void data_transmitService(void);
extern void CheckSensorBuffersForNewData(void);
extern uint16_t PutSensorSeparatorInPCTXBuffer(uint16_t PCTXCurrentPointer);
extern uint16_t CopySensorHeaderToPCTXBuffer(uint8_t SensorNumber, uint16_t PCTXCurrentPointer);
extern uint16_t CopyDataToPCTXBuffer(uint8_t SensorNumber, uint16_t PCTXCurrentPointer);
extern uint16_t copyBatteriesInfo_PCTXBuffer(uint16_t PCTXCurrentPointer);
extern uint16_t copyEncoderInfo_PCTXBuffer(uint16_t PCTXCurrentPointer);
#endif /* PCINTERFACE_H_ */