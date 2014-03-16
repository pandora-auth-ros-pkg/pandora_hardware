/*
 * sensors.h
 *
 * Created: 3/12/2014 10:41:19 PM
 *  Author: Klpanagi
 */ 
#include <stdint.h>

#ifndef SENSORS_H_
#define SENSORS_H_


	#define NUM_SENSORS 8
	#define NUM_SONAR 5
	#define NUM_TPA 3
	#define NUM_IR 0

typedef struct{
	uint8_t size; //Size of the Configuration packet
	uint8_t num_Sensors; //Number of sensors attached
	uint8_t num_Sonars; //Number of Sonar Sensors attached
	uint8_t num_IR; //Number of IR Sensors attached
	uint8_t num_TPA; //Number of TPA Sensors attached
}sensors_info;

enum Sensor_Types{
	TPA81,			//used 2014
	TPA16,			//not used 2014
	SRF05tinym,		//used 2014
	MLX90614,		//not used 2014
	SMBUSuni,
	SRF235,			//not used 2014
	COMPtiny,
	Battery			//NOT I2C SENSOR
};

#endif /* SENSORS_H_ */


extern void set_sensors_info(sensors_info *data);