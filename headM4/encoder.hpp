/*
 * encoder.hpp
 *
 *  Created on: Jan 3, 2015
 *      Author: Nikos Taras
 */

#ifndef ENCODER_HPP_
#define ENCODER_HPP_

#include "mbed.h"
#include "rtos.h"
#include "health.hpp"


void EncoderInit(PinName pin1, PinName pin2, PinName pin3, PinName pin4);

void receiveEncoderData();

void EncoderValueSet(uint16_t reading);

void EncoderSchedulerTask(void const *args);



#endif /* ENCODER_HPP_ */
