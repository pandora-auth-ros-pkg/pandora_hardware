/** @file
 * @author Taras Nikos
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
Serial pc(USBTX,USBRX); // tx, rx
//DigitalIn DO(p6);
//DigitalOut SCL(p7);
//DigitalOut CS(p8);



/** @brief Program entry point
 *
 * Initializes and starts tasks.
 */
int main(void) {
    DEBUG_PRINT(("Start\r\n"));

    // Setup the spi for 10 bit data, high steady state clock,
    // second edge capture, with a 1MHz clock rate
    spi.format(12,3);
    spi.frequency(500000);
    // Chip must be deselected
    //CS = 1;
    //SCL = 1;

    uint16_t value = 0;

    while(1){

    pc.printf("Just entered loop again\r\n");
    wait(1);

//    uint16_t read_value = 0;
//
//        CS = 0;
//        wait_us(2);
//
//        for(int i=0;i<11;i++){
//            SCL = 0; /* <First falling edge according to SSI protocol> */
//            wait_us(1);
//            SCL = 1;
//            wait_us(1);
//            read_value = (read_value<<1) | DO;
//
//        }
//        CS = 1;
//        pc.printf("I received: %d\r\n", read_value);
//        read_value = 0;


         //Select the device by setting chip select low
        cs = 0;

         //Send a dummy byte to receive the contents of the shift register
        value = spi.write(0b0);

        // Deselect the device
        cs = 1;
        value = value & 0b001111111111;
        pc.printf("I received: %d\r\n", value);
        value = 0;
    }

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
