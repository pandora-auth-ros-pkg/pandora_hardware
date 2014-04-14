#include "mbed.h"
#include "rtos.h"
#include "LibMods/i2c_nonblocking.h"
#include "grideye.hpp"
#include "CO2.hpp"
//#include "gpdma.h"
//#include "dsp.h"
//#include "USBSerial2.h"
//#include "Serial2.h"
//#include "cmsis_os.h"

#define I2C_ADDR_GND 0b1101000 << 1	//mbed accepts the oversimplified wrong address type TODO DEBUG: check if works OK
#define I2C_ADDR_VDD 0b1101001 << 1	//mbed accepts the oversimplified wrong address type TODO DEBUG: check if works OK

DigitalOut led(LED1);

void led_thread(void const *argument) {
    while (true) {
        // Signal flags that are reported as event are automatically cleared.
        Thread::signal_wait(0x1);
        led = !led;
    }
}

void TriggerTask(void const *args);

int main (void) {
	Thread tCO2(CO2Task);

	CO2Init();

    Thread thread(TriggerTask);

	typedef struct {
		I2C *i2c_obj;
		uint8_t i2c_addr;
	}i2c_sensor_t;

    I2C i2c0(p32,p31);	//sda, scl
    I2C i2c1(p9,p10);

    GridEYEInit(&i2c0, &i2c1);

	i2c_sensor_t temp_sens;

	temp_sens.i2c_obj = &i2c0;
	temp_sens.i2c_addr = I2C_ADDR_GND;
    Thread GridEYECenter(GridEYETask, (void *)&temp_sens);

	temp_sens.i2c_obj = &i2c0;
	temp_sens.i2c_addr = I2C_ADDR_VDD;
    Thread GridEYELeft(GridEYETask, (void *)&temp_sens);

	temp_sens.i2c_obj = &i2c1;
	temp_sens.i2c_addr = I2C_ADDR_GND;
    Thread GridEYERight(GridEYETask, (void *)&temp_sens);

    while (true) {
        Thread::wait(1000);
        thread.signal_set(0x1);
    }
}

void TriggerTask(void const *args) {
//	thread.signal_set(0x1);
	Thread::wait(12);
	CO2Trigger();

	Thread::wait(13);

	Thread::wait(25);

	Thread::wait(40);

	Thread::wait(10);
}
