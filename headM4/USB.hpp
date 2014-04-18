
#ifndef USB_HPP_
#define USB_HPP_

#include "mbed.h"
#include "rtos.h"
#include "LibMods/USBSerial2.h"
#include "CO2.hpp"

#define COMMAND_POLLING_TIME 10	///Time interval to check for incoming commands in milliseconds
#define CO2_REQUEST 0

void USBTask(const void *args);

#endif /* USB_HPP_ */
