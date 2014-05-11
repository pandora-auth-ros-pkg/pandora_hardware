/*!
 *	\file sensors.c
 *	\brief System Sensors Services Source file.
 *
 *	Created: 5/9/2014 4:20:52 PM
 *	Author: Panayiotou Kostantinos
 *	Email: glagloui@gmail.com
 */ 

#include "sensors.h"

uint16_t batteryVoltage_PSU, batteryVoltage_MOTOR;
encoder_struct _encoder;
batteries_struct _batteries;

/*!< Look Up Table holding information about the sensors on I2C bus. 
 *	Columns: 
 *		0 = SensorType 
 *		1 = Address on
 *		2 = Group that sensor belongs.
 */  
const int8_t I2CAddressLUT[I2CSENSNUM][3] = {
	{SRF05tinym, 0x80, 1},		/*! <Left Sonar> */
	{SRF05tinym, 0x82, 1},		/*! <Rear Right Sonar> */
	{SRF05tinym, 0x84, 2},		/*! <Right Sonar> */
	{SRF05tinym, 0x86, 2},		/*! <Rear Left Sonar> */
	{SRF05tinym, 0x88, 3},		/*! <Front Sonar> */
};

/*!< An array of i2c_sensor_struct objects. Holding i2c sensors info */
i2c_sensor_struct SensorModule[I2CSENSNUM];


uint16_t CreateSensorStructs(void)
{
	//edit was 2 , now 7 + 4 CRC
	uint16_t	PCTXBufferLenCr=7;		/* <NPFF + LF + 4 bytes Length + LF> */
	//end of edit
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
	return PCTXBufferLenCr + 13 + 5 + 10;			/* <13 added for battery values, 5 added for CRC, 10 for encoder> */
	
}