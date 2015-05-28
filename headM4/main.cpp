/** @file
 * @author Orestis Zachariadis
 * @brief Initializes and starts tasks.
 */
#include "mbed.h"
#include "rtos.h"
#include "statistics.hpp"
#include "grideye.hpp"
#include "CO2.hpp"
#include "USB.hpp"
#include "health.hpp"
#include "conf.h"
#include "encoder.hpp"
#include "battery.hpp"
#include "watcher.hpp"
//#include "gpdma.h"
//#include "dsp.h"

/** @brief Program entry point
 *
 * Initializes and starts tasks.
 */
int main(void) {
    DEBUG_PRINT(("Start\r\n"));
// TODO HealthySonarValuesSet in order to use DisableCountdown.
    WatcherInit();

    CO2Init(p17, p18);  //p17=TX, p18=RX

    I2C i2c0(p32, p31); //sda, scl  (Sonars)
    I2C i2c1(p9, p10);  //sda, scl  (Grideye)
    GridEYEInit(&i2c1);

    SonarInit(&i2c0);

    EncoderInit(p11, p12, p13,p14); //NN, DO, SCL, CS

    BatteryInit(p15,p16); //Motor Battery In, Supply Battery in (measurements)

    USBInit();

    HealthInit();

#if DEVELOPMENT
    //Thread tStatistics(CpuLoadTask, NULL, osPriorityIdle);
#endif

    Thread tWatcher(WatcherTask);

    Thread tUSB(USBTask);

    Thread tCO2Caller(CO2SchedulerTask);

    //Thread tGridEYECaller(GridEYESchedulerTask);

    Thread tSonarCaller(SonarSchedulerTask);

    Thread tEncoderCaller(EncoderSchedulerTask);

    Thread tBatteryCaller(BatterySchedulerTask);

    Thread::wait(osWaitForever);
}
