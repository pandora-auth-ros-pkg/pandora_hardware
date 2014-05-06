/** @file
 * @author Orestis Zachariadis
 * @brief
 */
#ifndef GRIDEYE_HPP_
#define GRIDEYE_HPP_


#include "mbed.h"
#include "rtos.h"
#include "LibMods/i2c_nonblocking.h"
#include "health.hpp"
#include "conf.h"


#if ENABLE_RGB_LEDMATRIX

/** @name RGB LED Matrix Colors */
//@{
#define GREEN 0x1C
#define YELLOW 0xFC
#define ORANGE 0xEC
#define RED 0xE0
//@}

#endif

/** @name GridEYE internal registers starting address */
//@{
#define GRIDEYE_I2C_THERM_ADDR 0x0E	///<GridEYE thermistor Register Starting Address
#define GRIDEYE_I2C_TEMP_ADDR 0x80	///<GridEYE temperature Registers Starting Address
//@}

/** @name GridEYE I2C addresses
 * @note mbed accepts the oversimplified wrong address type */
//@{
#define GRIDEYE_I2C_ADDR_GND 0b1101000 << 1		///<Address when AD_SELECT is grounded
#define GRIDEYE_I2C_ADDR_VDD 0b1101001 << 1		///<Address when AD_SELECT is tied to VDD
//@}

/** @name Individual GridEYE sensors id for use in various functions */
//@{
#define GEYE_CENTER 1	///<Center GridEYE identifier
#define GEYE_LEFT 2		///<Left GridEYE identifier
#define GEYE_RIGHT 3	///<Right GridEYE identifier
//@}

#define GRIDEYE_I2C_SIGNAL 0x1	///<Signal to GridEYETask()

#define PIXELS_COUNT 64	///<GridEYE number of pixels

/** @brief Required info to handle each GridEYE sensor */
typedef struct {
	I2C *i2c_obj;	///<I2C object pointer (one pointer per I2C peripheral)
	uint8_t i2c_periph_num;	///<0 for I2C0 peripheral, 1 for I2C1 etc
	uint8_t i2c_addr;	///<I2C address of selected peripheral
	uint8_t grideye_num;	///<Holds the GridEYE sensors id
}grideye_sensor_t;


/** @brief
 *
 * @param
 * @param
 */
void GridEYEInit(I2C *i2c0_obj, I2C *i2c1_obj);

/** @brief
 *
 * @param
 */
void GridEYETask(void const *args);

/** @brief
 *
 * @param
 */
void i2c_lock(uint8_t i2c_periph_num);

/** @brief
 *
 * @param
 */
void i2c_unlock(uint8_t i2c_periph_num);

/** @brief
 *
 * @param
 * @param
 */
void GridEYEvaluesSet(float values[], uint8_t grideye_num);

/** @brief
 *
 */
void GridEYESchedulerTask(void const *args);


#endif /* GRIDEYE_HPP_ */
