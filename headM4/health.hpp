#ifndef HEALTH_HPP_
#define HEALTH_HPP_

#include "grideye.hpp"
#include "CO2.hpp"
#include "USB.hpp"
#include "PeripheralNames.h"
#include "conf.h"


#define HEALTH_SIGNAL 0x1

#define I2C_SI 3	///I2C Serial Interrupt flag
#define I2C_STOP 4	///I2C STOP flag
#define I2C_START 5	///I2C START flag

/**
 * Watchdog Timer Interval in ms.
 * @note Watchdog uses a dedicated 500kHz oscillator with a fixed divide by 4 pre-scaler.
 * Minimum watchdog time is (1/500000) * 256 * 4 ~= 2 ms.
 * Maximum watchdog time is (1/500000) * 2^24 * 4 ~= 134000 ms
 */
#define WDT_MS 1500
#define WDEN 0	///Watchdog enable bit
#define WDRESET 1	///Watchdog reset enable bit


void HealthInit();

void CO2HealthTask(void const *args);

void GridEYEHealthTask(void const *args);

void clearHealthyCO2() ;

void clearHealthyGridEYE();

void HealthyCO2valueSet(float value);

void HealthyGridEYEvaluesSet(uint8_t values[], uint8_t grideye_num);

void repairCO2(uint8_t count);

void repairI2C(uint8_t count, int i2c_base);

void WDT_feed();

#endif /* HEALTH_HPP_ */
