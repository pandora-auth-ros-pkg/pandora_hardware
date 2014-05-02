#include "health.hpp"

static uint8_t CO2_health;
static uint8_t GridEYECenter_health;
static uint8_t GridEYELeft_health;
static uint8_t GridEYERight_health;

static uint8_t CO2_FailCount = 0;
static uint8_t I2C0_FailCount = 0;
static uint8_t I2C1_FailCount = 0;

static DigitalOut CO2_LifeLED(LED3);
static DigitalOut I2C0_LifeLED(LED1);
static DigitalOut I2C1_LifeLED(LED2);


void CO2HealthTask(void const *args) {
	while (true) {
		Thread::signal_wait(HEALTH_SIGNAL);
		if (!CO2_health) {
			CO2_FailCount++;
			if (CO2_FailCount == 1) {
				USBCO2valueSet(0);
			}
			if (CO2_FailCount > 5) {
				CO2_FailCount = 1;
			}
			repairCO2(CO2_FailCount);
		}
	}
}

void GridEYEHealthTask(void const *args) {
	while (true) {
		Thread::signal_wait(HEALTH_SIGNAL);
		if (!GridEYECenter_health || !GridEYELeft_health) {
			//TODO DO something here
		}
		if (!GridEYECenter_health && !GridEYELeft_health) {
			I2C0_FailCount++;
			if (I2C0_FailCount == 1) {
				USBGridEYEvaluesZero(GEYE_LEFT);
				USBGridEYEvaluesZero(GEYE_CENTER);
			}
			if (I2C0_FailCount > 40) {	//reset counter so that repair cycle starts from beginning
				I2C0_FailCount = 1;
			}
			repairI2C(I2C0_FailCount, I2C_0);
		}
		if (!GridEYERight_health) {
			I2C1_FailCount++;
			if (I2C1_FailCount == 1) {
				USBGridEYEvaluesZero(GEYE_RIGHT);
			}
			if (I2C1_FailCount > 40) {	//reset counter so that repair cycle starts from beginning
				I2C1_FailCount = 1;
			}
			repairI2C(I2C1_FailCount, I2C_1);
		}
	}
}

void clearHealthyCO2() {
	CO2_health = 0;
}

void clearHealthyGridEYE() {
	GridEYECenter_health = 0;
	GridEYELeft_health = 0;
	GridEYERight_health = 0;
}

void HealthyCO2valueSet(float value) {
	CO2_health = 1;
	CO2_FailCount = 0;
	USBCO2valueSet(value);
	CO2_LifeLED = !CO2_LifeLED;
}

void HealthyGridEYEvaluesSet(uint8_t values[], uint8_t grideye_num) {
	switch (grideye_num) {
		case GEYE_CENTER:
			GridEYECenter_health = 1;
			I2C0_FailCount = 0;
			USBGridEYEvaluesSet(values, grideye_num);
			I2C0_LifeLED = !I2C0_LifeLED;
			break;
		case GEYE_LEFT:
			GridEYELeft_health = 1;
			I2C0_FailCount = 0;
			USBGridEYEvaluesSet(values, grideye_num);
			I2C0_LifeLED = !I2C0_LifeLED;
			break;
		case GEYE_RIGHT:
			GridEYERight_health = 1;
			I2C1_FailCount = 0;
			USBGridEYEvaluesSet(values, grideye_num);
			I2C1_LifeLED = !I2C1_LifeLED;
			break;
		default:
			return;
	}
}

void repairCO2(uint8_t count) {
	if (count >2) {
		CO2Trigger();
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
}
