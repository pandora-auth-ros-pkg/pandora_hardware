/*!
 *	\file sensors.c
 *	\brief System Sensors Service Routines Source file.
 *
 *	Created: 5/9/2014 4:20:52 PM
 *	Author: Panayiotou Kostantinos
 *	Email: glagloui@gmail.com
 */ 

#include "sensors.h"

encoder_struct _encoder;
batteries_struct _batteries;
uint16_t BatteryPSU;
uint16_t BatteryMotor;
uint16_t encoder_value;


const int8_t I2CAddressLUT[I2CSENSNUM][3] = {
	{SRF05tinym, 0x80, 1},		/*! \brief <Left Sonar> */
	{SRF05tinym, 0x88, 2},		/*! \brief <Right Sonar> */
	//{SRF05tinym, 0x84, 2},		/*! \brief <Right Sonar> */
	{SRF05tinym, 0x82, 2},		/*! \brief <Rear Left Sonar> */
	{SRF05tinym, 0x86, 1},		/*! \brief <RIght Rear Sonat> */
};

i2c_sensor_struct SensorModule[I2CSENSNUM];


uint16_t CreateSensorStructs(void)
{
	uint16_t	PCTXBufferLenCr=7;		/* <NPFF + LF + 4 bytes Length + LF> */

	for ( int i = 0 ; i < I2CSENSNUM; i++)
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
			SensorModule[i].WriteBufer[0] = 0x80;			/* <Trigger Command> */
			SensorModule[i].WriteBufer[1] = 0x40;			/* <Read Range Command> */
			SensorModule[i].CurrentState = ReviveState;									/* <Starting in Revive State> */
			SensorModule[i].ReviveStateDuration = SRF05tinyWaitReviveSensorCounts;
			SensorModule[i].IdleState1Duration = SRF05tinyDelayAfterNack;
			break;
			default:
			break;
		}
		SensorModule[i].WaitCount1 = 0;									
		SensorModule[i].ComErrorCounts = 0;
		SensorModule[i].Status.SensStatusVal = 0;
		SensorModule[i].visitstoread = 0;
		SensorModule[i].StartIndexInPCTXBuffer = PCTXBufferLenCr;
		SensorModule[i].PCTXLen = ((SensorModule[i].ReadBufLen+5)*2+1+5)*sizeof(uint8_t);
		PCTXBufferLenCr += SensorModule[i].PCTXLen;
	}
	PCTXBufferLenCr += 13 + 5 + 11;		/* <13 added for battery values, 5 added for CRC, 11 for encoder> */
	return PCTXBufferLenCr;			
	
	
}