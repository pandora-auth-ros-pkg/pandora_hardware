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
//#include "gpdma.h"
//#include "dsp.h"



SPI spi(p5, p6, p7); // mosi, miso, sclk or NC, DO, SCL
DigitalOut cs(p8);   //CS


/** @brief Program entry point
 *
 * Initializes and starts tasks.
 */
int main(void) {
    DEBUG_PRINT(("Start\r\n"));
    while(1){
    wait_ms(100);

    // Chip must be deselected
        cs = 1;

        // Setup the spi for 10 bit data, high steady state clock,
        // second edge capture, with a 1MHz clock rate
        spi.format(10,3);
        spi.frequency(10000);

        // Select the device by seting chip select low
        cs = 0;

        // Send a dummy byte to receive the contents of the WHOAMI register
        uint16_t value = spi.write(0b0000000000);
        printf("I received: %d\r\n", value);

        // Deselect the device
        cs = 1;

//    CO2Init(p17, p18);  //p17=TX, p18=RX

//    I2C i2c0(p32, p31); //sda, scl
//    I2C i2c1(p9, p10);
//    GridEYEInit(&i2c0, &i2c1);

//    USBInit();

//    HealthInit();

//#if DEVELOPMENT
//    Thread tStatistics(CpuLoadTask, NULL, osPriorityIdle);
//#endif

//    Thread tUSB(USBTask);

//    Thread tCO2Caller(CO2SchedulerTask);

//    Thread tGridEYECaller(GridEYESchedulerTask);

//    Thread::wait(osWaitForever);
}
