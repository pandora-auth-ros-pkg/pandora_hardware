/*
 * sensorConf.h
 *
 * Created: 3/12/2012 3:54:41 μμ
 *  Author: dikan
 */ 

#ifndef __SENSORCONF_H__
#define __SENSORCONF_H__

enum Sensor_Types{
	SRF05tinym,
	Battery,									//	NOT I2C SENSOR	(ADCB)
	RotaryEncoder								//	SSI communication protocol
};

#define SENSNUM 5								// Additional there will be at the end of the measurements the Battery Voltage(s) conversion(s)
#define SENSORGROUPS 3							// Group0 is group-free. 3 means group0 + 3 more groups 
extern const int8_t I2CAddressLUT[SENSNUM][3];	// DO NOT FORGET TO CONFIGURE THE TABLE IN FSM.C

#endif