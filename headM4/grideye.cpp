/** @file
 * @author Orestis Zachariadis
 * @brief Implements GridEYE sensor functionality
 */
#include "grideye.hpp"


static Mutex i2c1_mutex;    ///<The mutex that locks access to I2C1 peripheral

/** @name GridEYETask() threads */
//@{
static Thread *tGridEYECenter;  ///<Thread pointer for center GridEYE sensor's GridEYETask()

//@}

static Thread *tGridEYEHealth;  ///<Thread pointer for GridEYEHealthTask()

void GridEYEInit(I2C *i2c1_obj) {
    //Check comment about sdram in Doxygen main page before using new

    i2c1_obj->frequency(400000);
    Thread::wait(200); //Needed???

    grideye_sensor_t temp_sens1;    //Because we pass starting arguments to threads with a pointer we must be sure that


    temp_sens1.i2c_obj = i2c1_obj;
    temp_sens1.i2c_periph_num = 1;
    temp_sens1.i2c_addr = GRIDEYE_I2C_ADDR_GND;
    temp_sens1.grideye_num = GEYE_CENTER;
    tGridEYECenter = new Thread(GridEYETask, (void *) &temp_sens1);



    tGridEYEHealth = new Thread(GridEYEHealthTask);

    Thread::wait(20);    //We must wait some time before the function ends or temp_sens? will be destroyed
                        //-> before the threads assign them to local variables. (I tested with 1ms and it was OK)
}                       //changed from 5

void GridEYETask(void const *args) {
    const grideye_sensor_t * geye = (const grideye_sensor_t *) args;

    I2C *i2c_obj = geye->i2c_obj;
    uint8_t i2c_periph_num = geye->i2c_periph_num;
    uint8_t i2c_addr = geye->i2c_addr;
    uint8_t grideye_num = geye->grideye_num;

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

#if ENABLE_RGB_LEDMATRIX

    DigitalOut SPI_ss(p8);  ///Slave Select
    SPI_ss = 1;  //Make sure the RG matrix is deactivated, maybe this should be first line executed
    SPI RGB_LEDMatrix(p5,p6,p7);  /// mosi, miso, sclk

#endif

    while (1) {
        Thread::signal_wait(GRIDEYE_I2C_SIGNAL);

        cmd[0] = GRIDEYE_I2C_THERM_ADDR;
        i2c1_lock();
        i2c_obj->write(i2c_addr, cmd, 1, true); //Repeated start is true in i2c_obj->write, so it must be true in
        i2c_obj->read(i2c_addr, thermistor_echo, 2, true); //-> the following read, too.
        i2c1_unlock();

        if (therm_echo_uint16 & 0x800) {  //if negative
            thermistor_value = -0.0625 * (0x7FF & therm_echo_uint16);
        } else {	//else if positive
            thermistor_value = 0.0625 * (0x7FF & therm_echo_uint16);
        }

        //If temper_echo remains unchanged after an i2c_obj->read(), indicates sensor fault.
        //-> So, if temper_echo_uint16[0] stays 0 after i2c_obj->read() the sensor is probably faulty.
        //-> Because a temperature value of 0 is OutOfBounds HealthyGridEYEvaluesSet() will not be triggered and
        //-> the sensor will be correctly registered as not healthy.
        //Only the first element of temper_echo_uint16 is zeroed to save processing time.
        temper_echo_uint16[0] = 0;

        cmd[0] = GRIDEYE_I2C_TEMP_ADDR;
        i2c1_lock();
        i2c_obj->write(i2c_addr, cmd, 1, true);
        i2c_obj->read(i2c_addr, temper_echo, 2 * PIXELS_COUNT, true);
        i2c1_unlock();

        for (int i = 0; i < PIXELS_COUNT; ++i) {
            if (temper_echo_uint16[i] & 0x800) {  //if negative
                temper_values[i] = 0.25 * (int16_t) (0xF000 | temper_echo_uint16[i]);
            } else {    //else if positive
                temper_values[i] = 0.25 * (0x7FF & temper_echo_uint16[i]);
            }
        }
        for(int i=0;i<PIXELS_COUNT;i++)
        DEBUG_PRINT(("TEMPERATURE[%d]: %f\r\n",i,temper_values[i]));

        GridEYEvaluesSet(temper_values, grideye_num);

#if ENABLE_RGB_LEDMATRIX

        if (grideye_num == GEYE_CENTER) {
            char ledArray[64];
            int celsius;

            RGB_LEDMatrix.format(8, 0);
            RGB_LEDMatrix.frequency(125000);

            //Determine LED Color for Pixel
            for (int pixel = 0; pixel < PIXELS_COUNT; ++pixel) {
                celsius = temper_values[pixel];
                if (celsius < 30) {
                    ledArray[pixel] = GREEN;
                } else if (celsius >= 30 && celsius < 32) {
                    ledArray[pixel] = YELLOW;
                } else if (celsius >= 32 && celsius < 34) {
                    ledArray[pixel] = ORANGE;
                } else if (celsius >= 34) {
                    ledArray[pixel] = RED;
                }
            }

            //Transfer LED Data
            SPI_ss = 0;
            wait_us(500);
            RGB_LEDMatrix.write(0x26);  //Resets RGBMatrixBackpack index. (see sparkfun's github). It shouldn't be needed
                                        //-> but it doesn't work without it...
            for (int pixel = 0; pixel < PIXELS_COUNT; ++pixel) {
                RGB_LEDMatrix.write(ledArray[pixel]);
            }
            wait_us(500);
            SPI_ss = 1;
        }

#endif
    }
}

void i2c1_lock() {
    i2c1_mutex.lock();
}

void i2c1_unlock() {
    i2c1_mutex.unlock();
}

void GridEYESignalClear(uint8_t grideye_num) {
    switch (grideye_num) {
    case GEYE_CENTER:
        tGridEYECenter->signal_clear(GRIDEYE_I2C_SIGNAL);
        break;
    default:
        return;
    }
}

void GridEYEvaluesSet(float values[], uint8_t grideye_num) {
    uint8_t GridEYEvalues[PIXELS_COUNT];
    uint8_t OutOfBounds = 0;

    for (int i = 0; i < PIXELS_COUNT; ++i) {
        if (values[i] > 0 && values[i] < 80) {
            GridEYEvalues[i] = (uint8_t) (values[i] + 0.5); //rounding to nearest Celsius degree
        } else {
            OutOfBounds = 1;
        }
    }

    if (!OutOfBounds) {
        HealthyGridEYEvaluesSet(GridEYEvalues, grideye_num);
    }
}

void GridEYESchedulerTask(void const *args) {
    //I2C sensors in the same I2C bus have maximum distance ie 50ms in a 100ms loop
    Thread::wait(300);
    while (true) {
        //clearHealthyGridEYE();

        //if (GridEYEenabled(GEYE_CENTER))
            tGridEYECenter->signal_set(GRIDEYE_I2C_SIGNAL);


        Thread::wait(40);
        //tGridEYEHealth->signal_set(HEALTH_SIGNAL);

        Thread::wait(10);
        Thread::wait(50);
    }
}
