#ifndef GRIDEYE_HPP_
#define GRIDEYE_HPP_


#include "mbed.h"
#include "rtos.h"
#include "LibMods/i2c_nonblocking.h"
#include "globals.hpp"

#define I2C_THERM_ADDR 0x0E	///Thermistor Register Starting Address
#define I2C_TEMP_ADDR 0x80	///Temperature Registers Starting Address

#define PIXELS_COUNT 64


void GridEYEInit(I2C *i2c0_obj, I2C *i2c1_obj);

void GridEYETask(void const *args);


#endif /* GRIDEYE_HPP_ */
