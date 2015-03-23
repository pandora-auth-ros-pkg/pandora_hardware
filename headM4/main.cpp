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
#include "battery.hpp"
#include "grideye.hpp"
//#include "gpdma.h"
//#include "dsp.h"

/** @brief Program entry point
 *
 * Initializes and starts tasks.
 */
int main(void) {
   // DEBUG_PRINT(("Start\r\n"));

    CO2Init(p17, p18);  //p17=TX, p18=RX

    I2C i2c0(p32, p31); //sda, scl
    I2C i2c1(p9, p10);

    GridEYEInit(&i2c1);

    BatteryInit(p15,p16); //Motor Battery In, Supply Battery in (measurements)

    SonarInit(&i2c0);

    EncoderInit(p11, p12, p13,p14); //NN, DO, SCL, CS

    USBInit();

    HealthInit();

#if DEVELOPMENT
    Thread tStatistics(CpuLoadTask, NULL, osPriorityIdle);
#endif

    Thread tUSB(USBTask);

    Thread tBatteryCaller(BatterySchedulerTask);

    Thread tCO2Caller(CO2SchedulerTask);

    //Thread tSonarCaller(SonarSchedulerTask);

    Thread tEncoderCaller(EncoderSchedulerTask);

    Thread tGridEYECaller(GridEYESchedulerTask);

    Thread::wait(osWaitForever);
}
