/** @file
 * @author Nikos Taras
 * @brief Initializes and starts tasks.
 */
#include <sonar.hpp>
#include "mbed.h"
#include "rtos.h"
#include "statistics.hpp"
#include "CO2.hpp"
#include "USB.hpp"
#include "health.hpp"
#include "conf.h"
#include "encoder.hpp"
//#include "gpdma.h"
//#include "dsp.h"

/** @brief Program entry point
 *
 * Initializes and starts tasks.
 */
int main(void) {
    DEBUG_PRINT(("Start\r\n"));

    CO2Init(p17, p18);  //p17=TX, p18=RX
//TODO disable the second i2c bus
    I2C i2c0(p32, p31); //sda, scl
    //I2C i2c1(p9, p10);
    SonarInit(&i2c0);

    EncoderInit(p5,p6,p7,p8); //NC, DO, SCL, CS

    USBInit();

    HealthInit();

#if DEVELOPMENT
    Thread tStatistics(CpuLoadTask, NULL, osPriorityIdle);
#endif

    Thread tUSB(USBTask);

    Thread tCO2Caller(CO2SchedulerTask);

    Thread tSonarCaller(SonarSchedulerTask);

    Thread tEncoderCaller(EncoderSchedulerTask);

    Thread::wait(osWaitForever);
}
