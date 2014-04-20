
#ifndef USB_HPP_
#define USB_HPP_

#include "mbed.h"
#include "rtos.h"
#include "LibMods/USBSerial2.h"
#include "CO2.hpp"
#include "grideye.hpp"

#define COMMAND_POLLING_TIME 10	///Time interval to check for incoming commands in milliseconds

#define GEYE_CENTER_REQUEST 1
#define GEYE_LEFT_REQUEST 2
#define GEYE_RIGHT_REQUEST 3
#define CO2_REQUEST 4

void USBInit();

void USBTask(const void *args);

#endif /* USB_HPP_ */
