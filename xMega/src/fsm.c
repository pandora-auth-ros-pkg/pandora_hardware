/*!
 *	\file fsm.c
 *	\brief System FSM services Source file.
 *
 *	Created: 5/9/2014 9:53:52 PM
 *	Author: dikan / Panayiotou Konstantinos
 *	Email: glagloui@gmail.com
 */ 
#include "fsm.h"

uint8_t SameGroupIDs[127];
int16_t TWIWatchdogTimer=0;
volatile uint8_t CurrentGroup=0;
volatile uint16_t	ticks; 
volatile uint8_t	TCTimingFlags;
volatile uint8_t	NextSensor;


void pandora_fsm(void)
{
	while(1)
	{
		if(TCTimingFlags & Tick200us)	/* <every 200us> */
		{
			TCTimingFlags&=~Tick200us; 
			/* <PUT HERE CODE TO BE SERVICED EVERY 200us> */
		}
		if(TCTimingFlags & Tick400us)	/* <every 400us> */
		{
			TCTimingFlags&=~Tick400us; 
			/* <PUT HERE CODE TO BE SERVICED EVERY 400us> */
		}
		if(TCTimingFlags & Tick1ms)		/* <every 1ms> */
		{
			TCTimingFlags&=~Tick1ms; 
			/* <PUT HERE CODE TO BE SERVICED EVERY 1ms> */
			//SensorWaitTimeHandler();							//Wait Time HANDLER OF SENSOR FSM (Timeout-Idle Timer-Revive Timer etc)
			//TWIWatchdog();
		}
		if(TCTimingFlags & Tick5ms)		/* <every 5ms> */
		{
			TCTimingFlags&=~Tick5ms; 
			/* <PUT HERE CODE TO BE SERVICED EVERY 5ms> */
			if(!(PCTXFlags&PCTX_Busy))	{CheckSensorBuffersForNewData();}
		}
		if(TCTimingFlags & Tick10ms)	/* <every 10ms> */
		{
			TCTimingFlags&=~Tick10ms;
			/* <PUT HERE CODE TO BE SERVICED EVERY 10ms> */
		}
		if(TCTimingFlags & Tick50ms)	/* <every 50ms> */
		{
			TCTimingFlags&=~Tick50ms; 
			/* <PUT HERE CODE TO BE SERVICED EVERY 50ms> */
		}
		if(TCTimingFlags & Tick100ms)	/* <every 100ms> */
		{
			TCTimingFlags&=~Tick100ms; 
			/* <PUT HERE CODE TO BE SERVICED EVERY 100ms> */
			/* <timestamp here?!> */
			_batteries.batteryVoltage_MOTOR = ADC_ResultCh_GetWord_Unsigned(&ADCA.CH0, 0) - ADC_OFFSET;		/* <Read 12 bit value and save as data> */
			_batteries.batteryVoltage_PSU = ADC_ResultCh_GetWord_Unsigned(&ADCA.CH1, 0) - ADC_OFFSET ;			/* <Read 12 bit value and save as data> */

			//get_encoder_values(&_encoder);
			cli();
			//encoder_value = ( ((read_encoder() - ENCODER_OFFSET) >= 0 ) && ((read_encoder() - ENCODER_OFFSET) <= 1024) ) ? (read_encoder() - ENCODER_OFFSET) : 2048 ;
			//encoder_value = read_encoder();
			get_encoder_values(&_encoder);
			sei();			
		}
		if(TCTimingFlags & Tick200ms)	/* <Every 200ms> */
		{
			TCTimingFlags&=~Tick200ms; 
			/* <PUT HERE CODE TO BE SERVICED EVERY 200ms> */
			if(!(PCTXFlags&PCTX_Busy))
			{
				
				BOARD_LED_PORT.OUTSET = (1<<BOARD_LED_PIN);
				PCTXPointer=0;
				PCTXFlags|=PCTX_Busy;
				RS232.DATA = PCTXBuffer[0];
				
			}
		}
	}
}


uint8_t I2CSensorFSMCore(uint8_t SensorNumber)
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
	NextSensorID=(NextSensorID % I2CSENSNUM);
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
			SensorModule[SensorNumber].WaitCount1=SRF05tinyWaitForBusTimeOut;				/* <Preset TimeOut for this communication> */
			if(SensorModule[SensorNumber].ComRepeats==0)
				{SensorModule[SensorNumber].ComRepeats=SRF05tinyComRepeatTimeOut;}			/* <Preset TimeOut for handling Nack and other collisions> */
			SensorModule[SensorNumber].RollBackState=FirstRunState;							/* <Repeat State on NACK> */
			SensorModule[SensorNumber].CurrentState=FirstCommunicationStateWait;			/* <Determine next state: Wait for measurements> */
			SensorModule[SensorNumber].TimeForNextCycle=SRF05tinyCommunicationCycle;		/* <Preset Time until next Communication> */
		}
		break;
//----------------------------------------------------------------------------------------------------
		case FirstCommunicationStateWait:
		if( twiMaster.status == TWIM_STATUS_BUSY )
		{
			/* <If communication timed out> */
			if (SensorModule[SensorNumber].WaitCount1== 0)
				/* <Put this sensor in revive state and check the next sensor> */									
				{SensorNumber=SetSensorInReviveState(SensorNumber);}						
		}
		else
		{
			SensorNumber=TWIResultHandler(SensorNumber);									/* <Check TWI result status, and prepare repeat or revive> */
			if (twiMaster.result == TWIM_RESULT_OK)											/* <If result status ok change to "Copy Data from TWI to Sensor Structure" STATE> */
			{
				/* <Leave this sensor and check the next> */
				SensorModule[SensorNumber].CurrentState = SecondCommunicationState;
				SensorNumber++;									
			}														
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
								SensorModule[SensorNumber].ReadBufLen);					/* <Read all 4 (ReadBufLen=4) bytes from SRF235 (Ver,Unused,DataHigh,DataLow)> */
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
		if(twiMaster.status == TWIM_STATUS_BUSY /*&& (twiMaster.result == TWIM_RESULT_OK)*/)
		{	
			if (SensorModule[SensorNumber].WaitCount1== 0)								/* <If communication timed out> */
				{SensorNumber=SetSensorInReviveState(SensorNumber);}					/* <Put this sensor in revive state and check the next sensor> */
		}
		else
		{
			SensorNumber=TWIResultHandler(SensorNumber);								/* <Check TWI result status, and prepare Repeat or Revive States> */
			if (twiMaster.result == TWIM_RESULT_OK)										/* <If result status flag is active> */
			{
				for(int j = 0; j < SensorModule[SensorNumber].ReadBufLen; j++)			/* <Loop to copy data> */
					{SensorModule[SensorNumber].ReadBufer[j]=twiMaster.readData[j];}	/* <Copy data from MastersHandler Buffer to Sensors Software Module Buffer> */
				SensorModule[SensorNumber].ComRepeats=0;
				if(SensorModule[SensorNumber].ReadBufer[0] & 0x08)						/* <if SRF04Tiny in internal busy state then repeat communication> */
					{SensorModule[SensorNumber].CurrentState = SecondCommunicationState;}
				else
				{
					SensorModule[SensorNumber].CurrentState = IdleState;
					if(SensorModule[SensorNumber].ReadBufer[0] & 1)						/* <Check for new measurement captures> */
						{SensorModule[SensorNumber].Status.NewData=1;}					/* <Mark that unread value is available> */
				}																		/* <Determine next state: idle until next communication cycle> */
				SensorNumber++;															/* <Leave this sensor and check the next> */
				TWIWatchdogTimer = 0;													/* <Reset I2C WatchdogTimer> */
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
		/* <PORTD.OUTTGL = 0x20;> */
		cli();									/* <Clear Global Interrupts> */
		while(1){}								/* <Loop to force WDT to reset the uController> */
	}
}

void WaitI2CStatus(uint8_t SensorNumber)
{
	if(twiMaster.status == TWIM_STATUS_READY)	{SensorNumber= TWIResultHandler(SensorNumber);}

	else
	{
		/* <IF I2C timeout then. WaitCount1 is decreased independant every ms in SensorWaitTimeHandler()> */
		if (SensorModule[SensorNumber].WaitCount1==0)
			/* <Leave this sensor and check the next> */
			{SensorNumber=SetSensorInReviveState(SensorNumber);}							
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
			if(SensorModule[SensorNumber].ComRepeats>1)
			{
				/* <SensorModule[SensorNumber].CurrentState=SensorModule[SensorNumber].RollBackState;> */
				SensorModule[SensorNumber].CurrentState=IdleState1;
				SensorModule[SensorNumber].WaitCount2=SensorModule[SensorNumber].IdleState1Duration;
				SensorNumber++;
			} 
			else {SensorNumber=SetSensorInReviveState(SensorNumber);}
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


void SensorWaitTimeHandler(void)		
{
	for ( int i = 0 ; i < I2CSENSNUM; i++)
	{
		if(SensorModule[i].WaitCount1)	{SensorModule[i].WaitCount1--;}
		if(SensorModule[i].WaitCount2)	{SensorModule[i].WaitCount2--;}
		if(SensorModule[i].ComRepeats)	{SensorModule[i].ComRepeats--;}
		if(SensorModule[i].TimeForNextCycle)	{SensorModule[i].TimeForNextCycle--;}
		if((SensorModule[i].CurrentState==IdleState)&&(SensorModule[i].TimeForNextCycle==0))
		{
			SensorModule[i].CurrentState = FirstRunState;
			SensorModule[i].ComRepeats=0;
		}
		if((SensorModule[i].CurrentState==IdleState1)&&(SensorModule[i].WaitCount2==0))
			{SensorModule[i].CurrentState = SensorModule[i].RollBackState;}
		/* <If Revive time is over then start sensor again> */		
		if((SensorModule[i].CurrentState==ReviveState)&&(SensorModule[i].WaitCount1==0))
		{
			SensorModule[i].TimeForNextCycle = 0;
			SensorModule[i].CurrentState = FirstRunState;
			SensorModule[i].ComRepeats = 0;
		}
	}
}


 
uint8_t SetSensorInReviveState(uint8_t SensorNumber)
{
	SensorModule[SensorNumber].ComErrorCounts++;
	//SensorModule[SensorNumber].Status.Used = 1;
	SensorModule[SensorNumber].Status.I2CComErr = 1;
	SensorModule[SensorNumber].CurrentState = ReviveState;
	SensorModule[SensorNumber].WaitCount1 = SensorModule[SensorNumber].ReviveStateDuration;
	twiMaster.interface->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;								/* <reset I2C> */
	SensorNumber++;
	return SensorNumber;																	/* <Leave this sensor and check for the next one> */
}

void GroupHandler(void)					
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


void CreateArraySameGroupIDs(void)			
{
	uint8_t i=0;
	for(uint8_t k=0;k<I2CSENSNUM;k++)
	{
		if(SensorModule[k].Groupmember == CurrentGroup)
		{
			SameGroupIDs[i++] = k;
			SameGroupIDs[i] = 255;
			SensorModule[k].Status.Used = 0;
		}
	}	
}


uint8_t FindFirstAvailableSensor(uint8_t SensorNumber)
{
	uint8_t i=SensorNumber;
	uint8_t k=i;
	for(uint8_t j=i;j<(i+I2CSENSNUM);j++){
		if(!(SensorModule[j%I2CSENSNUM].CurrentState==ReviveState)){
			if(!(SensorModule[j%I2CSENSNUM].CurrentState==IdleState)){
				if(!(SensorModule[j%I2CSENSNUM].CurrentState==IdleState1)){
						if( SensorModule[j%I2CSENSNUM].Groupmember ==0 )
						{k=j%I2CSENSNUM;
						break;}
					else
					{if(SensorModule[j%I2CSENSNUM].Groupmember==CurrentGroup){
						if(!((SensorModule[j%I2CSENSNUM].CurrentState==FirstRunState)&&(SensorModule[j%I2CSENSNUM].Status.Used)))
							{k=j%I2CSENSNUM;
							break;}
						}
					}
				}
			}	
		}	
	}
	return	i=k;
}