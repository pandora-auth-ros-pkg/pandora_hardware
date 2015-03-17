/** @file
 * @author Taras Nikos
 * @brief Initializes and starts tasks.
 */
#include "mbed.h"
#include "rtos.h"
//#include "statistics.hpp"
//#include "grideye.hpp"
//#include "CO2.hpp"
//#include "USB.hpp"
//#include "health.hpp"
//#include "conf.h"
//#include "gpdma.h"
//#include "dsp.h"


#define PIXELS_COUNT 64
#define GRIDEYE_I2C_THERM_ADDR 0x0E ///<GridEYE thermistor Register Starting Address
#define GRIDEYE_I2C_TEMP_ADDR 0x80
#define GRIDEYE_I2C_ADDR_GND 0b1101000 << 1

I2C i2c1_obj(p9, p10);


int main(void) {
    i2c1_obj.frequency(100000);
    Thread::wait(200);
    //printf("Start2\r\n");
    //Thread::wait(100);


    uint8_t addr = GRIDEYE_I2C_ADDR_GND;

    char cmd[2];

        union {
            char thermistor_echo[2];    //1 LSB = 0.0625 C , result 12-bit as signed absolute value
            uint16_t therm_echo_uint16; //little endian
        };
        float thermistor_value;

        union {
            char temper_echo[2 * PIXELS_COUNT]; //1 LSB = 0.25 C , result 12-bit as 2's complement
            uint16_t temper_echo_uint16[PIXELS_COUNT];  //little endian
        };

        float temper_values[PIXELS_COUNT];

        int ret;


        //cmd[0] = GRIDEYE_I2C_THERM_ADDR;
        //printf("Before write1\r\n");
        //int ret = i2c1_obj.write(addr, cmd, 1, true); //Repeated start is true in i2c_obj->write, so it must be true in
        //printf("After write 1 = %d\r\n", ret);
        //ret = i2c1_obj.read(addr, thermistor_echo, 2, true); //-> the following read, too.
        //printf("After read 1 = %d\r\n", ret);

        //if (therm_echo_uint16 & 0x800) {  //if negative
        //    thermistor_value = -0.0625 * (0x7FF & therm_echo_uint16);
        //} else {    //else if positive
        //        thermistor_value = 0.0625 * (0x7FF & therm_echo_uint16);
       // }

                //If temper_echo remains unchanged after an i2c_obj->read(), indicates sensor fault.
                //-> So, if temper_echo_uint16[0] stays 0 after i2c_obj->read() the sensor is probably faulty.
                //-> Because a temperature value of 0 is OutOfBounds HealthyGridEYEvaluesSet() will not be triggered and
                //-> the sensor will be correctly registered as not healthy.
                //Only the first element of temper_echo_uint16 is zeroed to save processing time.


       cmd[0] = GRIDEYE_I2C_TEMP_ADDR;


       //printf("Before write2\r\n");
       ret = i2c1_obj.write(addr, cmd, 1,true);
       //printf("After write2 = %d\r\n",ret);

       ret = i2c1_obj.read(addr, temper_echo, 2 * PIXELS_COUNT,true);
       //printf("After read2 = %d\r\n",ret);


       for(int i=0;i<PIXELS_COUNT;i++){
           printf("Temper_echo [%d] = %x\r\n",i,*(int*)&temper_echo_uint16[i]);
       }

                for (int i = 0; i < PIXELS_COUNT; ++i) {
                    if (temper_echo_uint16[i] & 0x800) {  //if negative
                        temper_values[i] = 0.25 * (int16_t) (0xF000 | temper_echo_uint16[i]);
                    } else {    //else if positive
                        temper_values[i] = 0.25 * (0x7FF & temper_echo_uint16[i]);
                    }
                }

        for(int i=0;i<PIXELS_COUNT;i++){
            printf("Temp value[%d] = %f\r\n",i,temper_values[i]);
        }

        Thread::wait(1000);


}
