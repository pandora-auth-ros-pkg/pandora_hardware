#include "health.hpp"

static uint8_t CO2_health;
static uint8_t GridEYECenter_health;
static uint8_t GridEYELeft_health;
static uint8_t GridEYERight_health;

static uint8_t CO2_FailCount = 0;
static uint8_t I2C0_FailCount = 0;
static uint8_t I2C1_FailCount = 0;

void HealthTask(void const *args) {
	while (true) {
		Thread::signal_wait(HEALTH_SIGNAL);
		if (!CO2_health) {
			CO2_FailCount++;
			if (CO2_FailCount == 1) {
				USBCO2valueSet(0);
			}
			if (CO2_FailCount > 4) {
				CO2Trigger();
			}
		} else if (!GridEYECenter_health || !GridEYELeft_health) {
			//TODO DO something here
		} else if (!GridEYECenter_health && !GridEYELeft_health) {
			I2C0_FailCount++;
			if (I2C0_FailCount == 1) {
				USBGridEYEvaluesZero(GEYE_LEFT);
				USBGridEYEvaluesZero(GEYE_CENTER);
			}
			repairI2C(I2C0_FailCount, I2C_0);
		} else if (!GridEYERight_health) {
			I2C1_FailCount++;
			if (I2C1_FailCount == 1) {
				USBGridEYEvaluesZero(GEYE_RIGHT);
			}
			repairI2C(I2C1_FailCount, I2C_1);
		}
	}
}

void clearHealthy() {
	CO2_health = 0;

	GridEYECenter_health = 0;
	GridEYELeft_health = 0;
	GridEYERight_health = 0;
}

void HealthyCO2(float value) {
	CO2_health = 1;
	CO2_FailCount = 0;
	USBCO2valueSet(value);
}

void HealthyGEyeCenter(uint8_t values[]) {
	GridEYECenter_health = 1;
	I2C0_FailCount = 0;
	USBGridEYEvaluesSet(values, GEYE_CENTER);
}

void HealthyGEyeLeft(uint8_t values[]) {
	GridEYELeft_health = 1;
	I2C0_FailCount = 0;
	USBGridEYEvaluesSet(values, GEYE_LEFT);
}

void HealthyGEyeRight(uint8_t values[]) {
	GridEYERight_health = 1;
	I2C1_FailCount = 0;
	USBGridEYEvaluesSet(values, GEYE_RIGHT);
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