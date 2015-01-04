/*

 * encoder.cpp
 *
 *  Created on: Jan 3, 2015
 *      Author: Nikos Taras
 */

#include "encoder.hpp"

static SPI *encoderSPI; ///<Pointer to the Serial2 class object that implements the CO2 sensor

void EncoderInit(PinName NC, PinName DO, PinName SCL, PinName CS) {   //mosi, miso, sclk or NC, DO, SCL and CS
    //Check comment about sdram in Doxygen main page before using new

    //The mosi pin of the SPI is not needed. So it's Not Connected (NC)
    encoderSPI = new SPI(NC, DO, SCL);
    // Setup the spi for 12 bit data, mode 3.
    // In fact, the encoder communicates with 10 bits SPI,
    // but we have to use 12 bit and the first two must be
    // thrown away, according to the encoder datasheet.
    encoderSPI.format(12,3);
    encoderSPI.frequency(500000);
    co2uart->attach(&RX_isr, Serial::RxIrq);

    tCO2 = new Thread(CO2ReceiverTask);
    tCO2Health = new Thread(CO2HealthTask);
}

void EncoderSchedulerTask(void const *args) {
    while (true) {
       // clearHealthyCO2();

       // if (CO2enabled())
       //     CO2Trigger();

        //The readings in the sensor are calculated every 500ms which is the rate of the infrared source within the
        //-> sensor. During the cycles when the signals are being tracked, the sensor will not respond to the
        //-> communication requests. The requests will be queued and dealt with in the next cycle. There is no "flush"
        //->  command to clear the queue. It is not suggested to pool the sensor more often than 500ms. The time for
        //-> the sensor to respond may also vary by a few ms depends on the internal operations. @official response
        //This means it will take a random time up to 500ms for the sensor to answer.
        Thread::wait(500);

        //Thread::wait(100);  //Timeout time.

        //tCO2Health->signal_set(HEALTH_SIGNAL);

        //Thread::wait(10);
    }
}

