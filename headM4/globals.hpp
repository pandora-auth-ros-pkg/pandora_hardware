
#ifndef GLOBALS_HPP_
#define GLOBALS_HPP_

#define GRIDEYE_I2C_ADDR_GND 0b1101000 << 1	//mbed accepts the oversimplified wrong address type TODO DEBUG: check if works OK
#define GRIDEYE_I2C_ADDR_VDD 0b1101001 << 1	//mbed accepts the oversimplified wrong address type TODO DEBUG: check if works OK

#define GRIDEYE_I2C_SIGNAL 0x1

typedef struct {
	I2C *i2c_obj;
	uint8_t i2c_addr;
}i2c_sensor_t;


#endif /* GLOBALS_HPP_ */
