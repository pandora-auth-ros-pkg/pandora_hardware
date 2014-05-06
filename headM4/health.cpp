/** @file
 * @author Orestis Zachariadis
 * @brief
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

static Mutex WDT_mutex;	///<Mutex that protects Watchdog feeding sequence


void HealthInit() {
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
