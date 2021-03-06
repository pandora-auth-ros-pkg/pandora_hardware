/*!
 *	\file pcInterface.c
 *	\brief Pc communication Interface Source file
 *
 *	Created: 5/9/2014 3:24:02 PM
 *	Author:	Panayiotou Kostantinos / Michael Niarchos
 *	Email:	klpanagi@gmail.com
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



#include "pcInterface.h"

uint16_t PCTXBufferLen;
uint16_t PCTXBufferCrc;

volatile uint16_t	PCTXPointer;
volatile uint8_t	PCTXFlags;
uint8_t *PCTXBuffer;
uint16_t	PCTXWaitAckNakTimer;

void data_transmitService(void)
{
	if( (PCTXFlags & PCTX_Busy) && !(PCTXFlags & PCTX_WaitAckNaK) )
	{
		PCTXPointer++;
		if(PCTXPointer<PCTXBufferLen && (USARTD0.STATUS & USART_DREIF_bm))
		{
			usart_putc(PCTXBuffer[PCTXPointer]);
		}
		PCTXPointer %= PCTXBufferLen;								/* <keep PCTXPointer in range [0,PCTXBufferLen]> */
		if(!PCTXPointer)
		{
			/* <Clear PCTX_Busy when all data have been transfered> */
			PCTXFlags &= ~(PCTX_Busy);
			if( !(PCTXFlags&PCTX_DebugMode) )
			{
				PCTXFlags |= (PCTX_WaitAckNaK|PCTX_Busy);			/* <Set WaitAckNak and Busy Flags for UART> */
				PCTXWaitAckNakTimer = WaitAckNakTimeOut;
			}
			USARTD0.STATUS |= USART_DREIF_bm;
			USARTD0.STATUS |= USART_RXCIF_bm;						/* <CLEAR RECEIVED DATA FLAG IN USART> */
		}
	}
	if( (PCTXFlags&PCTX_Busy)&&(PCTXFlags&PCTX_WaitAckNaK) )
	{
		if(PCTXWaitAckNakTimer)
		{
			if(USARTD0.STATUS & USART_RXCIF_bm)
			{
				uint8_t	TempReceive=USARTD0.DATA;
				USARTD0.STATUS |=USART_RXCIF_bm;
				if (TempReceive == UACK)
				{
					BOARD_LED_PORT.OUTCLR = (1<<BOARD_LED_PIN);
					/* <Last packet transmission was successful. Close the service> */
					PCTXFlags &= ~(PCTX_Busy|PCTX_WaitAckNaK);		
				}
				else
				{
					/* <Repeat last transmission!!> */
					PCTXFlags &= ~PCTX_WaitAckNaK;					
					usart_putc(PCTXBuffer[0]);
				}
			}
		}
		else
		{
			/* <Last packet transmission was without response from PC. Close the service and try again with new data> */
			PCTXFlags &= ~(PCTX_Busy|PCTX_WaitAckNaK);				
		}
	}
}

void CheckSensorBuffersForNewData(void)
{
	PCTXBuffer[0] = NPFF;											/* <Packet first byte is always the FormFeed Character (0x0C)> */
	PCTXBuffer[1] = LF;												/* <Packet second byte is always the LineFeed Character (0x0A)> */
	//edit
	//sending packet length = 4 bytes
	PCTXBuffer[2] = nybble_chars[((PCTXBufferLen-7)&0xf000)>>12];
	PCTXBuffer[3] = nybble_chars[((PCTXBufferLen-7)&0x0f00)>>8];
	PCTXBuffer[4] = nybble_chars[((PCTXBufferLen-7)&0x00f0)>>4];
	PCTXBuffer[5] = nybble_chars[((PCTXBufferLen-7)&0x000f)];
	PCTXBuffer[6] = LF;
	//current time (timestamp) must be inserted here
	uint16_t PCTXCurrentPointer = 7;
	//end of edit
	for(uint8_t i=0;i<I2CSENSNUM;i++)
	{
		PCTXCurrentPointer = SensorModule[i].StartIndexInPCTXBuffer;
		PCTXCurrentPointer = CopySensorHeaderToPCTXBuffer(i,PCTXCurrentPointer);
		if(SensorModule[i].Status.NewData)
		{
			/* <timestamp must be copied here from stucture> */
			SensorModule[i].Status.NewData = 0;
			PCTXCurrentPointer = CopyDataToPCTXBuffer(i,PCTXCurrentPointer);
			/* <while copying data, new data came so repeat this copy cycle with valid data> */
			if(SensorModule[i].Status.NewData){i--;}				
			else{SensorModule[i].Status.NewDataCopied=1;}
		}
		PCTXCurrentPointer = SensorModule[i].StartIndexInPCTXBuffer+SensorModule[i].PCTXLen - 1;
		PCTXCurrentPointer = PutSensorSeparatorInPCTXBuffer(PCTXCurrentPointer);					/* <LF 1 byte> */
	}
	
	/* <Copy Batteries info + data in PCTXBuffer> */
	PCTXCurrentPointer = copyBatteriesInfo_PCTXBuffer(PCTXCurrentPointer);
	/* <Copy Encoder info + data in PCTXBuffer> */
	PCTXCurrentPointer = copyEncoderInfo_PCTXBuffer(PCTXCurrentPointer);
	
	PCTXBufferCrc = calculate_CRC(PCTXBufferLen);
	
	PCTXBuffer[PCTXCurrentPointer++]=nybble_chars[(PCTXBufferCrc&0xf000)>>12];
	PCTXBuffer[PCTXCurrentPointer++]=nybble_chars[(PCTXBufferCrc&0x0f00)>>8];
	PCTXBuffer[PCTXCurrentPointer++]=nybble_chars[(PCTXBufferCrc&0x00f0)>>4];
	PCTXBuffer[PCTXCurrentPointer++]=nybble_chars[(PCTXBufferCrc&0x000f)];
	PCTXBuffer[PCTXCurrentPointer]=EOT;											/* <End of transmission Character> */
}

uint16_t calculate_CRC(uint16_t length)
{
	uint16_t crc = 0;
	for(int i=0; i<length-5; i++)	
		{crc += (int)PCTXBuffer[i];}
			
	return crc;
}

uint16_t copyBatteriesInfo_PCTXBuffer(uint16_t PCTXCurrentPointer)
{
	uint16_t pctxCurrentPointer_ = PCTXCurrentPointer;
	
	/* <battery "sensor" ID> */
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[((I2CSENSNUM)&0xf0)>>4];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[((I2CSENSNUM)&0x0f)];
	PCTXBuffer[pctxCurrentPointer_++] = ' ';
	/* <battery "sensor" TYPE> */
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[((Battery)&0xf0)>>4];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[((Battery)&0x0f)];
	PCTXBuffer[pctxCurrentPointer_++] = ' ';
	/* <battery has no status. Data comes next!!> */
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[(_batteries.batteryVoltage_PSU & 0x0f00)>>8];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[(_batteries.batteryVoltage_PSU & 0x00f0)>>4];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[(_batteries.batteryVoltage_PSU & 0x000f)];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[(_batteries.batteryVoltage_MOTOR & 0x0f00)>>8];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[(_batteries.batteryVoltage_MOTOR & 0x00f0)>>4];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[(_batteries.batteryVoltage_MOTOR & 0x000f)];
	pctxCurrentPointer_ = PutSensorSeparatorInPCTXBuffer(pctxCurrentPointer_);						/* <LF 1 Byte> */
	
	return (pctxCurrentPointer_);
}

uint16_t copyEncoderInfo_PCTXBuffer(uint16_t PCTXCurrentPointer)
{
	uint16_t pctxCurrentPointer_ = PCTXCurrentPointer;
	
	/* <Encoder "sensor" ID> */
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[((I2CSENSNUM+1)&0xf0)>>4];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[((I2CSENSNUM+1)&0x0f)];
	PCTXBuffer[pctxCurrentPointer_++] = ' ';
	/* <Encoder "sensor" TYPE> */
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[((RotaryEncoder)&0xf0)>>4];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[((RotaryEncoder)&0x0f)];
	PCTXBuffer[pctxCurrentPointer_++] = ' ';
	/* <Encoder data> */
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[(_encoder.rotation_value_read&0xf000)>>12];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[(_encoder.rotation_value_read&0x0f00)>>8];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[(_encoder.rotation_value_read&0x00f0)>>4];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[(_encoder.rotation_value_read&0x000f)];
	/* <LF 1 Byte> */
	pctxCurrentPointer_ = PutSensorSeparatorInPCTXBuffer(pctxCurrentPointer_);						
	
	return (pctxCurrentPointer_);
}


	

uint16_t PutSensorSeparatorInPCTXBuffer(uint16_t PCTXCurrentPointer)
{
	uint16_t RealPCTXBufferPointer = PCTXCurrentPointer;
	PCTXBuffer[RealPCTXBufferPointer++]=LF;	//=RS
	return RealPCTXBufferPointer;
}


uint16_t CopySensorHeaderToPCTXBuffer(uint8_t SensorNumber, uint16_t PCTXCurrentPointer)
{
	uint16_t RealPCTXBufferPointer = PCTXCurrentPointer;
	PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].SensorID>>4) & 0x0f];		/* <High Nibble of SensorID in ASCII> */
	PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].SensorID) & 0x0f];			/* <Lo Nibble of SensorID in ASCII> */
	PCTXBuffer[RealPCTXBufferPointer++]=' ';
	PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].type>>4) & 0x0f];			/* <High Nibble of SensorType in ASCII> */
	PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].type) & 0x0f];				/* <Lo Nibble of SensorType in ASCII> */
	PCTXBuffer[RealPCTXBufferPointer++]=' ';
	PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].SlaveAddress>>4) & 0x0f];	/* <High Nibble of SlaveAddress in ASCII> */
	PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].SlaveAddress) & 0x0f];		/* <Lo Nibble of SlaveAddress in ASCII> */
	PCTXBuffer[RealPCTXBufferPointer++]=' ';
	PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].Status.SensStatusVal>>4) & 0x0f];		/* <High Nibble of Status in ASCII> */
	PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].Status.SensStatusVal) & 0x0f];			/* <Lo Nibble of Status in ASCII> */
	PCTXBuffer[RealPCTXBufferPointer++]=' ';
	PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].CurrentState>>4) & 0x0f];	/* <High Nibble of Status in ASCII> */
	PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].CurrentState) & 0x0f];		/* <Lo Nibble of Status in ASCII> */
	PCTXBuffer[RealPCTXBufferPointer++]=' ';
	return RealPCTXBufferPointer;
}


uint16_t CopyDataToPCTXBuffer(uint8_t SensorNumber, uint16_t PCTXCurrentPointer)
{
	int16_t RealPCTXBufferPointer = PCTXCurrentPointer;

	for (uint8_t DataPointer=0; DataPointer < SensorModule[SensorNumber].ReadBufLen; DataPointer++)
	{
		/* <High Nibble of Data in ASCII> */
		PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].ReadBufer[DataPointer]&0xf0) >>4 ];
		/* <Low Nibble of Data in ASCII> */
		PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].ReadBufer[DataPointer]) & 0x0f];
	}
	return RealPCTXBufferPointer;
}
