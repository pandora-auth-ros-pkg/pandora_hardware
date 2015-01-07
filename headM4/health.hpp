/** @file
 * @author Orestis Zachariadis
 * @brief Includes, defines and function prototypes used for health monitoring functionality.
 */
#ifndef HEALTH_HPP_
#define HEALTH_HPP_

#include <sonar.hpp>
#include "CO2.hpp"
#include "USB.hpp"
#include "PeripheralNames.h"
#include "conf.h"

#define HEALTH_SIGNAL 0x1   ///<The signal a sensor's Scheduler Task sends after the timeout period

/** @name I2C Control Register bits */
//@{
#define I2C_SI 3    ///<I2C Serial Interrupt flag
#define I2C_STOP 4  ///<I2C STOP flag
#define I2C_START 5 ///<I2C START flag
//@}

/** @brief Watchdog Timer timeout
 *
 * Watchdog Timer Interval in ms.
 * @note Watchdog uses a dedicated 500kHz oscillator with a fixed divide by 4 pre-scaler.
 * Minimum watchdog time is (1/500000) * 256 * 4 ~= 2 ms.
 * Maximum watchdog time is (1/500000) * 2^24 * 4 ~= 134000 ms
 */
#define WDT_MS 1500 ///<Watchdog timer period in ms.
#define WDEN 0      ///<Watchdog enable bit
#define WDRESET 1   ///<Watchdog reset enable bit

/** @brief Sets the maximum number of times a sensor can fail to respond before it is disabled, max 255 */
#define DISABLE_COUNTDOWN 100

/** @brief Initializes stuff related with health monitoring
 *
 * Currently initiates the Watchdog Timer and the analog switches in the I2C power lines.
 */
void HealthInit();

/** @brief Checks CO2 sensor health status
 *
 *	Checks if CO2 sensor is healthy, counts how many times it failed and attempts recovery measures.
 */
void CO2HealthTask(void const *args);

/** @brief Checks GridEYE sensors health status
 *
 * Checks if GridEYE sensors are healthy, counts how many times they failed and attempts recovery measures.
 */
void SonarHealthTask(void const *args);

/** @brief Clears CO2 healthy status
 *
 * Sets the healthy status of a sensor to zero. We must do this before triggering the sensor.
 * If the sensor fails to respond, healthy status will remain to zero so CO2HealthTask() can check its status
 */
void clearHealthyCO2();

void clearHealthyEncoder();

/** @brief Clears GridEYE healthy status
 *
 * Sets the healthy status of a sensor to zero. We must do this before triggering the sensor.
 * If the sensor fails to respond, healthy status will remain to zero so SonarHealthTask() can check its status.
 */
void clearHealthySonar();

/** @brief Checks if a GridEYE sensor is enabled.
 *
 * Returns the corresponding GridEYE*_DisableCountdown, which is a positive number greater than zero if the sensor is
 * enabled, and zero if it is disabled.
 *
 * @param grideye_num grideye_sensor_t::grideye_num
 *
 * @returns true if selected sensor is enabled, false otherwise
 */
uint8_t SonarEnabled(uint8_t grideye_num);

/** @brief Checks if CO2 sensor is enabled.
 *
 * Returns CO2_DisableCountdown, which is a positive number greater than zero if the sensor is enabled,
 * and zero if it is disabled.
 *
 * @returns true if selected sensor is enabled, false otherwise
 */
uint8_t CO2enabled();

/** @brief Called when a sensor is healthy to send received data to USB
 *
 * This function is called only when we had a healthy response from a sensor. It's used directly in place of
 * USBCO2valueSet() if we want to pass the data through the health layer before sending over USB.
 * Sets sensor healthy status to true, blinks the LifeLED and sends the data to USB buffers.
 *
 * @param value The just received data from the sensor
 */
void HealthyCO2valueSet(float value);

void HealthyEncoderValueSet(uint16_t value);

void HealthyBatteryMotorValueSet(uint16_t value);

void HealthyBatterySupplyValueSet(uint16_t value);

/** @brief Called when a sensor is healthy to send received data to USB
 *
 * This function is called only when we had a healthy response from a sensor. It's used directly in place of
 * USBGridEYEvaluesSet() if we want to pass the data through the health layer before sending over USB.
 * Sets sensor healthy status to true, blinks the LifeLED and sends the data to USB buffers.
 *
 * @param values The just received data from the sensor
 * @param grideye_num grideye_sensor_t::grideye_num
 */
void HealthySonarValuesSet(uint16_t values, uint8_t sonar_num);

/** @brief Attempts to recover CO2 sensor
 *
 * @param count The sensor FailIndex value
 */
void repairCO2(uint8_t index);

/** @brief Attempts to recover I2C bus
 *
 * @param count The sensor FailIndex value
 * @param i2c_base Pointer to appropriate I2C peripheral memory address
 */
void repairI2C(uint8_t index, int i2c_base);

/** @brief Feeds the Watchdog
 *
 * Resets Watchdog timer. Protected with mutexes.
 */
void WDT_feed();

#endif /* HEALTH_HPP_ */
