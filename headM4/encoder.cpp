/*

 * encoder.cpp
 *
 *  Created on: Jan 3, 2015
 *      Author: Nikos Taras
 */

#include "encoder.hpp"

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

