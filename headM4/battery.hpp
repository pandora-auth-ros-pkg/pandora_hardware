/*
 * battery.hpp
 *
 *  Created on: Jan 7, 2015
 *      Author: Nikos Taras
 */

#ifndef BATTERY_HPP_
#define BATTERY_HPP_

#include "mbed.h"
#include "rtos.h"
#include "health.hpp"

void BatteryInit(PinName pin1, PinName pin2);

void receiveBatteryData();

void BatteryMotorValueSet(uint16_t reading);

void BatterySupplyValueSet(uint16_t reading);

void BatterySchedulerTask(void const *args);



#endif /* BATTERY_HPP_ */
