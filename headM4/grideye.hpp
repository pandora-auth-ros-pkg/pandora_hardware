#ifndef GRIDEYE_HPP_
#define GRIDEYE_HPP_


#include "mbed.h"
#include "rtos.h"
#include "LibMods/i2c_nonblocking.h"
#include "health.hpp"

#define ENABLE_RGB_LEDMATRIX 0

#if ENABLE_RGB_LEDMATRIX

#define GREEN 0x1C
#define RED 0xE0
#define ORANGE 0xFC

#endif

#define GRIDEYE_I2C_THERM_ADDR 0x0E	///GridEYE thermistor Register Starting Address
#define GRIDEYE_I2C_TEMP_ADDR 0x80	///GridEYE temperature Registers Starting Address

#define GRIDEYE_I2C_ADDR_GND 0b1101000 << 1	//mbed accepts the oversimplified wrong address type TODO DEBUG: check if works OK
#define GRIDEYE_I2C_ADDR_VDD 0b1101001 << 1	//mbed accepts the oversimplified wrong address type TODO DEBUG: check if works OK

#define GEYE_CENTER 1
#define GEYE_LEFT 2
#define GEYE_RIGHT 3

#define GRIDEYE_I2C_SIGNAL 0x1

#define PIXELS_COUNT 64

typedef struct {
	I2C *i2c_obj;
	uint8_t i2c_periph_num;
	uint8_t i2c_addr;
	uint8_t grideye_num;
}i2c_sensor_t;

void GridEYEInit(I2C *i2c0_obj, I2C *i2c1_obj);

void GridEYETask(void const *args);

void GridEYEvaluesSet(float values[], uint8_t grideye_num);

void i2c_lock(uint8_t i2c_periph_num);

void i2c_unlock(uint8_t i2c_periph_num);

#endif /* GRIDEYE_HPP_ */
