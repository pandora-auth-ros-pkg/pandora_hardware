/** @file
 * @author Orestis Zachariadis
 * @brief Implements sensor health monitoring functionality
 *
 * @note Replace {sensor_name} in the next paragraph with CO2 or GridEYE or another sensor accordingly.
 *
 * In the beginning of each sensor scanning cycle ( see {sensor_name}SchedulerTask() ) healthy status
 * of {sensor_name} is resetted ( clearHealthy{sensor_name}() is called, which sets {sensor_name}_healthy = 0 ).
 * If {sensor_name} responds in time, Healthy{sensor_name}valueSet() is called, which sets {sensor_name} as
 * healthy ( {sensor_name}_healthy = 1 ). But if {sensor_name} doesn't respond in time, {sensor_name}_healthy
 * stays 0. {sensor_name}HealthTask() is signaled after an appropriate time (timeout) to check if {sensor_name}
 * is healthy or not (checks if {sensor_name}_healthy was set or remained at zero ). If remained at zero, ie
 * {sensor_name} didn't answer in time, {sensor_name}HealthTask() starts monitoring the number of failures and
 *  attempts repair measures.
 *
 * @note Healthy{sensor_name}valueSet() is a direct replacement of USB{sensor_name}valueSet(). Thus we could just
 * bypass the health layer by calling USB{sensor_name}valueSet() instead of Healthy{sensor_name}valueSet() inside
 * the sensor's source file.
 * @par
 * @note To clear any confusion: {sensor_name}valueSet() in {sensor_name}.cpp just checks if the value(s) returned
 * is(are) inside limits as defined by {sensor_name}'s manufacturer and calls either  USB{sensor_name}valueSet() ( if
 * it just needs USB communication ) or Healthy{sensor_name}valueSet() (if it needs health monitoring layer)
 * @par
 * @note While a sensor is considered not healthy, its USB buffer is set to zero.
 */
#include "health.hpp"

/** @name Sensors health status
 * 1 if healty, 0 else */
//@{
static uint8_t CO2_healthy;
static uint8_t GridEYECenter_healthy;
static uint8_t GridEYELeft_healthy;
static uint8_t GridEYERight_healthy;
//@}

/** @name Sensors timeout expired counter
 * Each variable counts the number of times each sensor or peripheral failed to respond in time */
//@{
static uint8_t CO2_FailCount = 0;
static uint8_t I2C0_FailCount = 0;
static uint8_t I2C1_FailCount = 0;
//@}

/** @name Status Leds
 * If a sensor or assigned peripheral works the led is blinking  */
//@{
static DigitalOut CO2_LifeLED(LED3);
static DigitalOut I2C0_LifeLED(LED1);
static DigitalOut I2C1_LifeLED(LED2);
//@}

/** @name Enables / Disables the I2C analog switches
 * Turns on or off the analog switches that control the I2C buses power supply
 * @see MAX4674ESE+ */
//@{
static DigitalOut I2C0_switch(p30);
static DigitalOut I2C1_switch(p29);
//@}

static Mutex WDT_mutex;	///<Mutex that protects Watchdog feeding sequence


void HealthInit() {
	I2C0_switch = 0;	//Active Low
	I2C1_switch = 0;	//Active Low
#if !DEVELOPMENT
    LPC_WDT->TC = (int)( (WDT_MS / 1000) * (500000 / 4) );
    LPC_WDT->MOD = (1 << WDRESET) | (1 << WDEN);	//enable watchdog reset
    WDT_feed();	//A valid feed sequence must be completed after setting WDEN before the Watchdog
    			//-> is capable of generating a reset
#endif
}

void CO2HealthTask(void const *args) {
	while (true) {
		Thread::signal_wait(HEALTH_SIGNAL);

		WDT_feed();

		if (!CO2_healthy) {
			CO2_FailCount++;
			if (CO2_FailCount == 1) {
				USBCO2valueSet(0);
			}
			repairCO2(CO2_FailCount);
		}
	}
}

void GridEYEHealthTask(void const *args) {
	while (true) {
		Thread::signal_wait(HEALTH_SIGNAL);

		WDT_feed();

		if (!GridEYECenter_healthy || !GridEYELeft_healthy) {
			//TODO Do something here, though unlikely to come here
		}
		if (!GridEYECenter_healthy && !GridEYELeft_healthy) {
			I2C0_FailCount++;
			if (I2C0_FailCount == 1) {
				USBGridEYEvaluesZero(GEYE_LEFT);
				USBGridEYEvaluesZero(GEYE_CENTER);
			}
			repairI2C(I2C0_FailCount, I2C_0);
		}
		if (!GridEYERight_healthy) {
			I2C1_FailCount++;
			if (I2C1_FailCount == 1) {
				USBGridEYEvaluesZero(GEYE_RIGHT);
			}
			repairI2C(I2C1_FailCount, I2C_1);
		}
	}
}

void clearHealthyCO2() {
	CO2_healthy = 0;
}

void clearHealthyGridEYE() {
	GridEYECenter_healthy = 0;
	GridEYELeft_healthy = 0;
	GridEYERight_healthy = 0;
}

void HealthyCO2valueSet(float value) {
	CO2_healthy = 1;
	CO2_FailCount = 0;
	USBCO2valueSet(value);
	CO2_LifeLED = !CO2_LifeLED;
}

void HealthyGridEYEvaluesSet(uint8_t values[], uint8_t grideye_num) {
	switch (grideye_num) {
		case GEYE_CENTER:
			GridEYECenter_healthy = 1;
			I2C0_FailCount = 0;
			USBGridEYEvaluesSet(values, grideye_num);
			I2C0_LifeLED = !I2C0_LifeLED;
			break;
		case GEYE_LEFT:
			GridEYELeft_healthy = 1;
			I2C0_FailCount = 0;
			USBGridEYEvaluesSet(values, grideye_num);
			I2C0_LifeLED = !I2C0_LifeLED;
			break;
		case GEYE_RIGHT:
			GridEYERight_healthy = 1;
			I2C1_FailCount = 0;
			USBGridEYEvaluesSet(values, grideye_num);
			I2C1_LifeLED = !I2C1_LifeLED;
			break;
		default:
			return;
	}
}

void repairCO2(uint8_t count) {
	if (count == 3) {
		CO2Trigger();
	}

	//Determines the rate at which the repair measures are repeated (we can't know how long the cause of the problem lasts)
	if (CO2_FailCount > 5) {
		CO2_FailCount = 1;
	}
}

void repairI2C(uint8_t count, int i2c_base) {
	LPC_I2C_TypeDef *i2c_periph = (LPC_I2C_TypeDef *)i2c_base;

	if ( (count >= 3) && (i2c_periph->STAT == 0) ) {
		i2c_periph->CONSET = 1 << I2C_STOP;
		i2c_periph->CONCLR = 1 << I2C_SI;
		return;
	}
	if (count == 3) {
		if (i2c_periph->CONSET & 1 << I2C_START) {
			i2c_periph->CONSET = 1 << I2C_STOP;
		} else {
			i2c_periph->CONSET = 1 << I2C_START;
		}
	} else if (count == 5) {
		i2c_periph->CONSET = 1 << SI;
	} else if (count == 7) {
		i2c_periph->CONSET = 1 << I2C_START;
		i2c_periph->CONSET = 1 << I2C_STOP;
	}

	//Determines the rate at which the repair measures are repeated (we can't know how long the cause of the problem lasts)
	if (I2C0_FailCount > 40) {
		I2C0_FailCount = 1;
	}
}

void WDT_feed() {
	WDT_mutex.lock();
    LPC_WDT->FEED = 0xAA;
    LPC_WDT->FEED = 0x55;
    WDT_mutex.unlock();
}
