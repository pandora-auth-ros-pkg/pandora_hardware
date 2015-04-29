/** @file
 * @author Orestis Zachariadis
 * @brief Includes, defines and function prototypes used for the GridEYE sensor
 */
#ifndef SONAR_HPP_
#define SONAR_HPP_
//This file will now hold sound sensors
#include "mbed.h"
#include "rtos.h"
#include "health.hpp"
#include "conf.h"



/** @name GridEYE internal registers starting address */
//@{
//#define GRIDEYE_I2C_THERM_ADDR 0x0E ///<GridEYE thermistor Register Starting Address
//#define GRIDEYE_I2C_TEMP_ADDR 0x80  ///<GridEYE temperature Registers Starting Address
//@}

/** @name GridEYE I2C addresses
 * @note mbed accepts the oversimplified wrong address type */
//@{
#define SONAR_LEFT_I2C_ADDR 0x80     ///<Address when AD_SELECT is grounded
#define SONAR_RIGHT_I2C_ADDR 0x86    ///<Address when AD_SELECT is tied to VDD
//@}

/** @name Individual GridEYE sensors id for use in various functions */
//@{
//#define GEYE_CENTER 1   ///<Center GridEYE identifier
#define SONAR_LEFT 1     ///<Left GridEYE identifier
#define SONAR_RIGHT 2    ///<Right GridEYE identifier
//@}

#define SONAR_I2C_SIGNAL 0x2  ///<Signal to SonarTask()

//#define PIXELS_COUNT 64 ///<GridEYE number of pixels

/** @struct sonar_sensor_t
 * @brief Required info to handle each GridEYE sensor */
typedef struct {
    I2C *i2c_obj;   ///<I2C object pointer (one pointer per I2C peripheral)
    uint8_t i2c_periph_num; ///<0 for I2C0 peripheral, 1 for I2C1 etc
    uint8_t i2c_addr;   ///<I2C address of selected peripheral
    uint8_t sonar_num;    ///<Holds the GridEYE sensors id
} sonar_sensor_t;

/** @brief GridEYE sensors initialization
 *
 * Initialize GridEYE sensors communication and required threads
 *
 * @param i2c0_obj I2C pointer of I2C0 peripheral
 * @param i2c1_obj I2C pointer of I2C1 peripheral
 */
void SonarInit(I2C *i2c0_obj);



/** @brief Gets temperature data from GridEYE sensors
 *
 * Gets temperature and thermistor data from GridEYE sensors. Also sends temperature data to
 * RGB Led Matrix if enabled in conf.h
 *
 * @param args A pointer to the sonar_sensor_t with the required info to handle each GridEYE sensor
 */
void SonarTask(void const *args);

/** @brief Locks access to I2C bus
 *
 * Uses mutexes to allow only one sensor of each I2C peripheral request and receive data at the same time
 *
 * @param i2c_periph_num sonar_sensor_t::i2c_periph_num of the current sensor
 */
void i2c0_lock();

/** @brief Unlocks access to I2C bus
 *
 * Uses mutexes to allow only one sensor of each I2C peripheral request and receive data at the same time
 *
 * @param i2c_periph_num sonar_sensor_t::i2c_periph_num of the current sensor
 */
void i2c0_unlock();

/** @brief Clears the signal from the indicated GridEYE sensor
 *
 * Clears the signal that is set from SonarSchedulerTask().
 *
 * @param sonar_num sonar_sensor_t::sonar_num
 */
void SonarSignalClear(uint8_t grideye_num);

/** @brief Sets the buffer of each GridEYE sensor so that it will be available for transfer
 *
 * Checks if temperature readings is inside their manufacturer's specified limits, rounds each temperature value
 * so it can fit to a byte and sets the buffer of each GridEYE sensor that will be send over USB after it passes
 * through the health monitoring layer.
 *
 * @param values The values that was just read from the GridEYE sensors
 * @param sonar_num sonar_sensor_t::sonar_num
 */
void SonarValueSet(uint16_t value, uint8_t sonar_num);

/** @brief Schedules when we acquire data from the GridEYE sensors
 *
 * Manages GridEYE sensors triggering period and signals GridEYEHealthTask() after a timeout period to check if there
 * was any response.
 */
void SonarSchedulerTask(void const *args);

#endif /* SONAR_HPP_ */
