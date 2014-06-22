/*
 * battery.h
 *
 * Created: 6/20/2014 6:33:27 PM
 *  Author: Kostantinos
 */ 


#ifndef BATTERY_H_
#define BATTERY_H_

#include "adc_driver.h"
/*!    \def ADC_OFFSET
	\brief Precalculated ADC offset to be value 153 + 7 =160 = 1.086Volt
*/ 		
#define ADC_OFFSET 160	/*<1.086V>*/	 

/*!  \struct batteries_struct
 *	\brief Batteries voltage level measurements structure.
 *	Vars for voltage levels measurements of Electronics-PSU battery and Motor-Servo battery.
 */
typedef struct{
	uint16_t batteryVoltage_PSU;
	uint16_t batteryVoltage_MOTOR;
}batteries_struct;

/*!	\fn void init_ADC(void)
 *	\brief Initialize two ADC inputs ( Battery Voltage Level Measuments )
 */
void init_ADC(void);



#endif /* BATTERY_H_ */