/** @file
 * @author Orestis Zachariadis
 * @brief
 */
#include "mbed.h"
#include "rtos.h"
#include "statistics.hpp"
#include "grideye.hpp"
#include "CO2.hpp"
#include "USB.hpp"
#include "health.hpp"
#include "conf.h"
//#include "gpdma.h"
//#include "dsp.h"


/** @brief Program entry point
 *
 * Initializes and starts tasks.
 */
int main (void) {
	DEBUG_PRINT(("Start\r\n"));

	CO2Init(p17, p18);	//p17=TX, p18=RX

    I2C i2c0(p32,p31);	//sda, scl
    I2C i2c1(p9,p10);
    GridEYEInit(&i2c0, &i2c1);

    USBInit();

    HealthInit();

#if DEVELOPMENT
    Thread tStatistics(CpuLoadTask, NULL, osPriorityIdle);
#endif

    Thread tUSB(USBTask);

    Thread tCO2Caller(CO2SchedulerTask);

    Thread tGridEYECaller(GridEYESchedulerTask);

    Thread::wait(osWaitForever);
}
