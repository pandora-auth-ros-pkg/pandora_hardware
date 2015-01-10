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

static int nSamples; //counts samples position. first sample in position 0.


static uint16_t * motorSamples;
static uint16_t * supplySamples;

#define BATTERY_OFFSET 190
#define MAX_SAMPLES 10




void BatteryInit(PinName pin1, PinName pin2){
    MB = pin1;
    SB = pin2;

    nSamples = 0;

    motorSamples = new uint16_t[20];  //much more than needed
    supplySamples = new uint16_t[20]; //much more than needed. actual = 10

    ADCmotor = new AnalogIn(MB);
    ADCsupply = new AnalogIn(SB);
}

void receiveBatteryData() {

    uint16_t batteryMotorReading;
    uint16_t batterySupplyReading;

    float reading1, reading2;

    reading1 = ADCmotor->read();
    reading2 = ADCsupply->read();

    batteryMotorReading = (uint16_t) 4096*reading1;
    batterySupplyReading =(uint16_t) 4096*reading2;

    //batteryMotorReading = ADCmotor->read_u16();
    //batterySupplyReading = ADCsupply->read_u16();

    //batteryMotorReading &= 0x0FFF;  // we want the 12bytes
    //batterySupplyReading &= 0x0FFF; //May not be needed

    addSamples(batteryMotorReading, batterySupplyReading);


}



void addSamples(uint16_t motorReading, uint16_t supplyReading){

    motorSamples[nSamples] = motorReading;
    supplySamples[nSamples] = supplyReading;

    uint16_t temp = 0;  //needed for samples sorting

    if (nSamples>0){              //if it is not the first sample
        if (motorSamples[nSamples-1] > motorSamples[nSamples]){ // sorting Motor from small to bigger
            temp = motorSamples[nSamples-1];
            motorSamples[nSamples-1] = motorSamples[nSamples];
            motorSamples[nSamples] = temp;
        }
        if (supplySamples[nSamples-1] > supplySamples[nSamples]){ // sorting Supply from small to bigger
            temp = supplySamples[nSamples-1];
            supplySamples[nSamples-1] = supplySamples[nSamples];
            supplySamples[nSamples] = temp;
        }
    }

    nSamples++;                   //Now nSamples variable holds the actual samples number
    if (nSamples == MAX_SAMPLES){
    adcFilter();
    }

}

void adcFilter(){
    //samples are now sorted. Sample Arrays are full with MAX_SAMPLES samples
    //filter just returns the median. We can calculate an average around median.
    uint16_t temp;
    temp = (motorSamples[3]+motorSamples[4]+motorSamples[5]) / 3;
    BatteryMotorValueSet(temp);
    temp = (supplySamples[3]+supplySamples[4]+supplySamples[5]) / 3;
    BatterySupplyValueSet(temp);

    nSamples = 0;
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

        Thread::wait(50);
        receiveBatteryData();

    }
}


