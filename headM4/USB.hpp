
#ifndef USB_HPP_
#define USB_HPP_

#include "mbed.h"
#include "rtos.h"
#include "LibMods/USBSerial2.h"
#include "grideye.hpp"

#define COMMAND_POLLING_TIME 10	///Time interval to check for incoming commands in milliseconds

#define GEYE_CENTER_REQUEST 1
#define GEYE_LEFT_REQUEST 2
#define GEYE_RIGHT_REQUEST 3
#define CO2_REQUEST 4

void USBInit();

void USBTask(const void *args);

void USBGridEYEvaluesSet(uint8_t values[], uint8_t grideye_num);

void USBGridEYEvaluesZero(uint8_t grideye_num);

uint8_t * USBGridEYEvaluesGet(uint8_t grideye_num);

void USBCO2valueSet(float value);

float USBCO2valueGet();

#endif /* USB_HPP_ */
