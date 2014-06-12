/*!
 *	\file pcInterface.c
 *	\brief Pc communication Interface Source file
 *
 *	Created: 5/9/2014 3:24:02 PM
 *	Author:	Panayiotou Kostantinos
 *	Email:	glagloui@gmail.com
 */ 

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
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[(encoder_value&0xf000)>>12];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[(encoder_value&0x0f00)>>8];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[(encoder_value&0x00f0)>>4];
	PCTXBuffer[pctxCurrentPointer_++] = nybble_chars[(encoder_value&0x000f)];
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