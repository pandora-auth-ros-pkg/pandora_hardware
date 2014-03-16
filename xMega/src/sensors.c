/*
 * sensors.c
 *
 * Created: 3/13/2014 2:59:02 AM
 *  Author: Klpanagi
 */ 

#include "include/sensors.h"

void set_sensors_info(sensors_info *data){
	data->num_Sensors = NUM_SENSORS;
	data->num_Sonars = NUM_SONAR;
	data->num_IR = NUM_TPA;
	data->num_TPA = NUM_IR;
}