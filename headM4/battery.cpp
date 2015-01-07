/*
 * battery.cpp
 *
 *  Created on: Jan 7, 2015
 *      Author: Nikos Taras
 */

#include "battery.hpp"


static PinName MB;  //Motor Battery voltage pin
static PinName SB;  //Supply Battery voltage pin

static AnalogIn *ADCmotor;  //Pointer to the ADC that measures motor battery voltage
static AnalogIn *ADCsupply; //Pointer to the ADC that measures supply battery voltage



#define BATTERY_OFFSET 445


void BatteryInit(PinName pin1, PinName pin2){
    MB = pin1;
    SB = pin2;

    ADCmotor = new AnalogIn(MB);
    ADCsupply = new AnalogIn(SB);
}

void receiveBatteryData() {

    uint16_t batteryMotorReading;
    uint16_t batterySupplyReading;

    batteryMotorReading = ADCmotor->read_u16();
    batterySupplyReading = ADCsupply->read_u16();

    batteryMotorReading &= 0x0FFF;  //This is needed because we want the 12bytes
    batterySupplyReading &= 0x0FFF;


    BatteryMotorValueSet(batteryMotorReading);
    BatterySupplyValueSet(batterySupplyReading);

}

void BatteryMotorValueSet(uint16_t reading){
     //TODO Find upper and bottom limits for Battery measurement
     HealthyBatteryMotorValueSet(reading);
}

void BatterySupplyValueSet(uint16_t reading){
     //TODO Find upper and bottom limits for Battery measurement
     HealthyBatterySupplyValueSet(reading);
}

void BatterySchedulerTask(void const *args) {
    while (true) {

        Thread::wait(100);
        receiveBatteryData();

    }
}


