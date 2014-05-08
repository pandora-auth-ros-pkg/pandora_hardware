/*
 * FSMs.c
 *
 * Created: 3/12/2012 3:54:41 μμ
 *  Author: dikan
 */ 

#include "FSMs.h"
#include "sensorConf.h"
#include "sensorStruct.h"
#include "xMega.h"

const int8_t I2CAddressLUT[SENSNUM][3] = {
											{SRF05tinym, 0x80, 1},		//Left Sonar
											{SRF05tinym, 0x82, 1},		//Rear Right Sonar
											{SRF05tinym, 0x84, 2},		//Right Sonar
											{SRF05tinym, 0x86, 2},		//Rear Left Sonar
											{SRF05tinym, 0x88, 3},		//Front Sonar
										 };
sensor_struct SensorModule[SENSNUM];
uint8_t SameGroupIDs[127];
int16_t TWIWatchdogTimer=0;
volatile uint8_t CurrentGroup=0;

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
uint16_t CreateSensorStructs(void)
{	
	//edit was 2 , now 7 + 4 CRC
	uint16_t	PCTXBufferLenCr=7;														// NPFF + LF + 4 bytes Length + LF
	//end of edit										
	for ( int i = 0 ; i < SENSNUM; i++)
	{
		SensorModule[i].type = I2CAddressLUT[i][0];
		SensorModule[i].SlaveAddress = I2CAddressLUT[i][1];
		SensorModule[i].Groupmember = I2CAddressLUT[i][2];
		SensorModule[i].SensorID = i;
		switch (SensorModule[i].type)
		{
			case SRF05tinym:
				SensorModule[i].WriteBufLen = SRF05tiny_WRITE_LEN;
				SensorModule[i].ReadBufLen = SRF05tiny_READ_LEN;
				SensorModule[i].ReadBufer = malloc(SRF05tiny_READ_LEN * sizeof(uint8_t));
				SensorModule[i].WriteBufer = malloc(SRF05tiny_WRITE_LEN * sizeof(uint8_t));
				SensorModule[i].WriteBufer[0] = 0x80;			//trigger
				SensorModule[i].WriteBufer[1] = 0x40;			//Read
				SensorModule[i].CurrentState = ReviveState;									//START WITH REVIVE STATE
				SensorModule[i].ReviveStateDuration = SRF05tinyWaitReviveSensorCounts;
				SensorModule[i].IdleState1Duration = SRF05tinyDelayAfterNack;
			break;
			default:
			break;
		}
		SensorModule[i].WaitCount1 = 0;											//WITH DELAY (ms) AFTER INITIALISE
		SensorModule[i].ComErrorCounts = 0;
		SensorModule[i].Status.SensStatusVal = 0;
		SensorModule[i].visitstoread = 0;
		SensorModule[i].StartIndexInPCTXBuffer = PCTXBufferLenCr;
		SensorModule[i].PCTXLen = ((SensorModule[i].ReadBufLen+5)*2+1+5)*sizeof(uint8_t);
		PCTXBufferLenCr += SensorModule[i].PCTXLen;
	}
	return PCTXBufferLenCr + 13 + 5;												//13 added for battery values, 5 added for CRC
}

uint8_t SensorFSMCore(uint8_t SensorNumber)
{
	uint8_t NextSensorID=SensorNumber;
	if(!((PCTXFlags&PCTX_DebugMode)&&(PCTXFlags&PCTX_Busy)))
	{
 		uint8_t CurrentSensorID=FindFirstAvailableSensor(SensorNumber);
		
		switch (SensorModule[CurrentSensorID].type)
		{
			case SRF05tinym:
				NextSensorID=SRF05tinyFSM(CurrentSensorID);
			break;
			default:
			break;
		}	
	}
	NextSensorID=(NextSensorID % SENSNUM);
	return NextSensorID;
}

uint8_t SRF05tinyFSM(uint8_t SensorNumber)
{
	switch (SensorModule[SensorNumber].CurrentState){
//-------------------------------------------------------------------------------------------------------
		case FirstRunState:
		if ((SensorModule[SensorNumber].Status.Used)&&!(SensorModule[SensorNumber].Groupmember==0)){break;}
		if (twiMaster.status==TWIM_STATUS_READY)
		{	
			TWI_MasterWrite(&twiMaster,
							SensorModule[SensorNumber].SlaveAddress,
							(SensorModule[SensorNumber].WriteBufer+0),
							1);
			SensorModule[SensorNumber].Status.Used=1;
			SensorModule[SensorNumber].Status.I2CComErr=0;
			SensorModule[SensorNumber].WaitCount1=SRF05tinyWaitForBusTimeOut;				//Preset TimeOut for this communication
			if(SensorModule[SensorNumber].ComRepeats==0)
				{SensorModule[SensorNumber].ComRepeats=SRF05tinyComRepeatTimeOut;}			//Preset TimeOut for handling Nack and other collisions
			SensorModule[SensorNumber].RollBackState=FirstRunState;							//Repeat State on NACK
			SensorModule[SensorNumber].CurrentState=FirstCommunicationStateWait;			//determine next state: Wait for measurements
			SensorModule[SensorNumber].TimeForNextCycle=SRF05tinyCommunicationCycle;		//Preset Time until next Communication
		}
		break;
//----------------------------------------------------------------------------------------------------
		case FirstCommunicationStateWait:
		if((twiMaster.status == TWIM_STATUS_BUSY))
		{
			if (SensorModule[SensorNumber].WaitCount1== 0)									//if communication timed out
				{SensorNumber=SetSensorInReviveState(SensorNumber);}						//put this sensor in revive state and check the next sensor
		}
		else
		{
			SensorNumber=TWIResultHandler(SensorNumber);									//Check TWI result status, and prepare repeat or revive
			if (twiMaster.result == TWIM_RESULT_OK)											//else If result status ok -> Copy Data from TWI to Sensor Structure
			{
				SensorModule[SensorNumber].CurrentState = SecondCommunicationState;
				SensorNumber++;
			}														//leave this sensor and check the next
		}
		break;
//-------------------------------------------------------------------------------------------------------
		case SecondCommunicationState:
		if (twiMaster.status==TWIM_STATUS_READY)
		{	
			TWI_MasterWriteRead(&twiMaster,
								SensorModule[SensorNumber].SlaveAddress,
								SensorModule[SensorNumber].WriteBufer+1,
								1,														/* send only 1st data of WriteBufer (value=0) to set internal PC of SRF to 0 */
								SensorModule[SensorNumber].ReadBufLen);					//read all 4 (ReadBufLen=4) bytes from SRF235 (Ver,Unused,DataHigh,DataLow)
			SensorModule[SensorNumber].Status.I2CComErr=0;
			SensorModule[SensorNumber].WaitCount1=SRF05tinyWaitForBusTimeOut;
			if(SensorModule[SensorNumber].ComRepeats==0)
				{SensorModule[SensorNumber].ComRepeats=SRF05tinyComRepeatTimeOut;}
			SensorModule[SensorNumber].RollBackState=SecondCommunicationState;
			SensorModule[SensorNumber].CurrentState=SecondCommunicationStateWait;
		}
		break;
//-------------------------------------------------------------------------------------------------------
		case SecondCommunicationStateWait:
		if((twiMaster.status == TWIM_STATUS_BUSY) /*&& (twiMaster.result == TWIM_RESULT_OK)*/)
		{	
			if (SensorModule[SensorNumber].WaitCount1== 0)								//if communication timed out
				{SensorNumber=SetSensorInReviveState(SensorNumber);}							//put this sensor in revive state and check the next sensor
		}
		else
		{
			SensorNumber=TWIResultHandler(SensorNumber);								//Check TWI result status, and prepare repeat or revive
			if (twiMaster.result == TWIM_RESULT_OK)										//else If result status ok ->
			{
				for(int j = 0; j < SensorModule[SensorNumber].ReadBufLen; j++)			//Loop to copy data
					{SensorModule[SensorNumber].ReadBufer[j]=twiMaster.readData[j];}	//from MastersHandler Buffer to SensorBuffer
				SensorModule[SensorNumber].ComRepeats=0;
				if(SensorModule[SensorNumber].ReadBufer[0] & 0x08)						//if SRF04Tiny in internal busy state then repeat communication
					{SensorModule[SensorNumber].CurrentState = SecondCommunicationState;}
				else
				{
					SensorModule[SensorNumber].CurrentState = IdleState;
					if(SensorModule[SensorNumber].ReadBufer[0] & 1)						//check if current data new measurement 
					{SensorModule[SensorNumber].Status.NewData=1;}						//Mark that unreaden value is available
				}																		//determine next state: idle until next communication cycle
				SensorNumber++;															//leave this sensor and check the next
				TWIWatchdogTimer = 0;													//Reset I2C WatchdogTimer
			}														
		}
		break;
//-------------------------------------------------------------------------------------------------------
		case IdleState:
		case IdleState1:
		case ReviveState:
		SensorNumber++;
		break;
		default:
		break;
	}
	return SensorNumber;
}


void TWIWatchdog(void)
{
	TWIWatchdogTimer++;
	if(TWIWatchdogTimer > TWIWatchdogTimeOut)
	{
		PORTD.OUTTGL = 0x20;
		cli();									//irq's off
		while(1);{}								//loop to force WDT reset
	}
}

void WaitI2CStatus(uint8_t SensorNumber)
{
	if(twiMaster.status == TWIM_STATUS_READY)	{SensorNumber= TWIResultHandler(SensorNumber);}

	else
	{
		if (SensorModule[SensorNumber].WaitCount1==0)										//IF I2C timeout then. WaitCount1 is decreased independant every ms in SensorWaitTimeHandler()
			{SensorNumber=SetSensorInReviveState(SensorNumber);}							//leave this sensor and check the next
	}
}

uint8_t TWIResultHandler(uint8_t SensorNumber)
{
	switch (twiMaster.result)
	{
		case TWIM_RESULT_OK:
		break;
		case TWIM_RESULT_UNKNOWN:
		case TWIM_RESULT_NACK_RECEIVED:
			if(SensorModule[SensorNumber].ComRepeats>1){
				//SensorModule[SensorNumber].CurrentState=SensorModule[SensorNumber].RollBackState;
				SensorModule[SensorNumber].CurrentState=IdleState1;
				SensorModule[SensorNumber].WaitCount2=SensorModule[SensorNumber].IdleState1Duration;
				SensorNumber++;}
			else{SensorNumber=SetSensorInReviveState(SensorNumber);}
		break;
		case TWIM_RESULT_FAIL:
		case TWIM_RESULT_ARBITRATION_LOST:
		case TWIM_RESULT_BUFFER_OVERFLOW:
		case TWIM_RESULT_BUS_ERROR:
			SensorNumber=SetSensorInReviveState(SensorNumber);
		break;
		default:
		break;
	}
	return SensorNumber;
}

void SensorWaitTimeHandler(void)		//decrease WaitCount1/ComRepeats/TimeForNextCycle if !0 and 
{
	for ( int i = 0 ; i < SENSNUM; i++)
	{
		if(SensorModule[i].WaitCount1)	{SensorModule[i].WaitCount1--;}
		if(SensorModule[i].WaitCount2)	{SensorModule[i].WaitCount2--;}
		if(SensorModule[i].ComRepeats)	{SensorModule[i].ComRepeats--;}
		if(SensorModule[i].TimeForNextCycle)	{SensorModule[i].TimeForNextCycle--;}
		if((SensorModule[i].CurrentState==IdleState)&&(SensorModule[i].TimeForNextCycle==0))
		{
			SensorModule[i].CurrentState=FirstRunState;
			SensorModule[i].ComRepeats=0;
		}
		if((SensorModule[i].CurrentState==IdleState1)&&(SensorModule[i].WaitCount2==0))
			{SensorModule[i].CurrentState=SensorModule[i].RollBackState;}
		if((SensorModule[i].CurrentState==ReviveState)&&(SensorModule[i].WaitCount1==0))	//If Revive time is over then start sensor again
		{
			SensorModule[i].TimeForNextCycle=0;
			SensorModule[i].CurrentState=FirstRunState;
			SensorModule[i].ComRepeats=0;
		}
	}
}

uint8_t SetSensorInReviveState(uint8_t SensorNumber)	//as is
{
	SensorModule[SensorNumber].ComErrorCounts++;
	//SensorModule[SensorNumber].Status.Used = 1;
	SensorModule[SensorNumber].Status.I2CComErr = 1;
	SensorModule[SensorNumber].CurrentState = ReviveState;
	SensorModule[SensorNumber].WaitCount1 = SensorModule[SensorNumber].ReviveStateDuration;
	twiMaster.interface->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;								//reset I2C
	SensorNumber++;
	return SensorNumber;																	//leave this sensor and check the next
}

void CheckSensorBuffersForNewData(void)
{
	PCTXBuffer[0] = NPFF;											//Packet first byte is always the FormFeed Character (0x0C)
	PCTXBuffer[1] = LF;												//Packet second byte is always the LineFeed Character (0x0A)
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
	for(uint8_t i=0;i<SENSNUM;i++)
	{
		PCTXCurrentPointer = SensorModule[i].StartIndexInPCTXBuffer;
		PCTXCurrentPointer = CopySensorHeaderToPCTXBuffer(i,PCTXCurrentPointer);
		if(SensorModule[i].Status.NewData)
		{
			//timestamp must be copied here from stucture
			SensorModule[i].Status.NewData = 0;
			PCTXCurrentPointer=CopyDataToPCTXBuffer(i,PCTXCurrentPointer);
			if(SensorModule[i].Status.NewData){i--;}										//while copying data, new data came so repeat this copy cycle with valid data
			else{SensorModule[i].Status.NewDataCopied=1;}
		}
		PCTXCurrentPointer = SensorModule[i].StartIndexInPCTXBuffer+SensorModule[i].PCTXLen - 1;
		PCTXCurrentPointer = PutSensorSeparatorInPCTXBuffer(PCTXCurrentPointer);
	}
	//battery "sensor" ID
	PCTXBuffer[PCTXCurrentPointer++] = nybble_chars[((SENSNUM)&0xf0)>>4];
	PCTXBuffer[PCTXCurrentPointer++] = nybble_chars[((SENSNUM)&0x0f)];
	PCTXBuffer[PCTXCurrentPointer++] = ' ';
	//battery "sensor" TYPE
	PCTXBuffer[PCTXCurrentPointer++] = nybble_chars[((Battery)&0xf0)>>4];
	PCTXBuffer[PCTXCurrentPointer++] = nybble_chars[((Battery)&0x0f)];
	PCTXBuffer[PCTXCurrentPointer++] = ' ';
	//battery has no status. Data comes next!!
	PCTXBuffer[PCTXCurrentPointer++] = nybble_chars[(batteryVoltage_PSU&0x0f00)>>8];
	PCTXBuffer[PCTXCurrentPointer++] = nybble_chars[(batteryVoltage_PSU&0x00f0)>>4];
	PCTXBuffer[PCTXCurrentPointer++] = nybble_chars[(batteryVoltage_PSU&0x000f)];
	PCTXBuffer[PCTXCurrentPointer++] = nybble_chars[(batteryVoltage_MOTOR&0x0f00)>>8];
	PCTXBuffer[PCTXCurrentPointer++] = nybble_chars[(batteryVoltage_MOTOR&0x00f0)>>4];
	PCTXBuffer[PCTXCurrentPointer++] = nybble_chars[(batteryVoltage_MOTOR&0x000f)];
	PCTXCurrentPointer = PutSensorSeparatorInPCTXBuffer(PCTXCurrentPointer);				//LF 1 Byte
	
	/************************************************************************/
	/* Encoder PCTX Implementation HERE             
	1. Add to bufferLength ( +3 encoder Id [sensnum+1) ( +3 sensor Type) (+2 value) (+1 LF)
	2.                                                        */
	/************************************************************************/
	
	//edit
	PCTXBufferCrc=0;
	for(int i=0; i<PCTXBufferLen-5; i++)
		PCTXBufferCrc+=(int)PCTXBuffer[i];
	PCTXBuffer[PCTXCurrentPointer++]=nybble_chars[(PCTXBufferCrc&0xf000)>>12];
	PCTXBuffer[PCTXCurrentPointer++]=nybble_chars[(PCTXBufferCrc&0x0f00)>>8];
	PCTXBuffer[PCTXCurrentPointer++]=nybble_chars[(PCTXBufferCrc&0x00f0)>>4];
	PCTXBuffer[PCTXCurrentPointer++]=nybble_chars[(PCTXBufferCrc&0x000f)];
	PCTXBuffer[PCTXCurrentPointer]=EOT;														//End of transmission Character
 	//end of edit
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
		PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].SensorID>>4) & 0x0f];		//High Nibble of SensorID in ASCII
		PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].SensorID) & 0x0f];			//Lo Nibble of SensorID in ASCII
		PCTXBuffer[RealPCTXBufferPointer++]=' ';
		PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].type>>4) & 0x0f];			//High Nibble of SensorType in ASCII
		PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].type) & 0x0f];				//Lo Nibble of SensorType in ASCII
		PCTXBuffer[RealPCTXBufferPointer++]=' ';
		PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].SlaveAddress>>4) & 0x0f];	//High Nibble of SlaveAddress in ASCII
		PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].SlaveAddress) & 0x0f];		//Lo Nibble of SlaveAddress in ASCII
		PCTXBuffer[RealPCTXBufferPointer++]=' ';
		PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].Status.SensStatusVal>>4) & 0x0f];		//High Nibble of Status in ASCII
		PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].Status.SensStatusVal) & 0x0f];			//Lo Nibble of Status in ASCII
		PCTXBuffer[RealPCTXBufferPointer++]=' ';
		PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].CurrentState>>4) & 0x0f];		//High Nibble of Status in ASCII
		PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].CurrentState) & 0x0f];			//Lo Nibble of Status in ASCII
		PCTXBuffer[RealPCTXBufferPointer++]=' ';
	return RealPCTXBufferPointer;
}

uint16_t CopyDataToPCTXBuffer(uint8_t SensorNumber, uint16_t PCTXCurrentPointer)
{
		int16_t RealPCTXBufferPointer = PCTXCurrentPointer;

		for (uint8_t DataPointer=0; DataPointer < SensorModule[SensorNumber].ReadBufLen; DataPointer++)
		{
			PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].ReadBufer[DataPointer]&0xf0) >>4 ];	//High Nibble of Data in ASCII
			PCTXBuffer[RealPCTXBufferPointer++]=nybble_chars[(SensorModule[SensorNumber].ReadBufer[DataPointer]) & 0x0f];		//Lo Nibble of Data in ASCII
		}
		return RealPCTXBufferPointer;
}

void GroupHandler(void)						//check if all sensors of current group ended measuring or are in revive state. If done, initialize next group.
{
	uint8_t NextGroup = CurrentGroup+1;
	if (NextGroup > SENSORGROUPS){NextGroup=1;}


	uint8_t i=0;
	while(!(SameGroupIDs[i]==255))
	{
		WDT_Reset();
		if(!((SensorModule[SameGroupIDs[i]].CurrentState==FirstRunState)&&(SensorModule[SameGroupIDs[i]].Status.Used)))
		{
			if(!(SensorModule[SameGroupIDs[i]].CurrentState==ReviveState))
			{
				NextGroup = CurrentGroup;
				break;
			}
		}
		i++;
	}
	if (!(NextGroup==CurrentGroup))
	{
		CurrentGroup=NextGroup;
		CreateArraySameGroupIDs();
	}
}

void CreateArraySameGroupIDs(void)			//create an array with the IDs of sensors that belong in CurrentGroup (this group will begin measurements) and clear 'Used' flag of Sensors in this group
{
	uint8_t i=0;
	for(uint8_t k=0;k<SENSNUM;k++)
	{
		if(SensorModule[k].Groupmember == CurrentGroup)
		{
			SameGroupIDs[i++] = k;
			SameGroupIDs[i] = 255;
			SensorModule[k].Status.Used = 0;
		}
	}	
PORTA_OUT = (CurrentGroup<<4)&0xF0;
}

uint8_t FindFirstAvailableSensor(uint8_t SensorNumber)
{
	uint8_t i=SensorNumber;
	uint8_t k=i;
	for(uint8_t j=i;j<(i+SENSNUM);j++){
		if(!(SensorModule[j%SENSNUM].CurrentState==ReviveState)){
			if(!(SensorModule[j%SENSNUM].CurrentState==IdleState)){
				if(!(SensorModule[j%SENSNUM].CurrentState==IdleState1)){
						if((SensorModule[j%SENSNUM].Groupmember==0))
						{k=j%SENSNUM;
						break;}
					else
					{if(SensorModule[j%SENSNUM].Groupmember==CurrentGroup){
						if(!((SensorModule[j%SENSNUM].CurrentState==FirstRunState)&&(SensorModule[j%SENSNUM].Status.Used)))
							{k=j%SENSNUM;
							break;}
						}
					}
				}
			}	
		}	
	}
	return	i=k;
}