/** @file
 * @author Orestis Zachariadis
 * @brief Implements GridEYE sensor functionality
 */
#include <sonar.hpp>

static Mutex i2c0_mutex;    ///<The mutex that locks access to I2C0 peripheral


/** @name SonarTask() threads */
//@{
//static Thread *tGridEYECenter;  ///<Thread pointer for center GridEYE sensor's SonarTask()
static Thread *tSonarLeft;    ///<Thread pointer for left GridEYE sensor's SonarTask()
static Thread *tSonarRight;   ///<Thread pointer for right GridEYE sensor's SonarTask()
//@}

static Thread *tSonarHealth;  ///<Thread pointer for SonarHealthTask()

void SonarInit(I2C *i2c0_obj) {
    //Check comment about sdram in Doxygen main page before using new

    i2c0_obj->frequency(100000); //I modified the frequency from 400kHz to 100kHz

    sonar_sensor_t temp_sens1;    //Because we pass starting arguments to threads with a pointer we must be sure that
    sonar_sensor_t temp_sens2;    //-> their memory contents don't change long enough for the threads to copy the data
                                  //-> to local variables. So we create a temporary structure for each thread.
    //TODO Change Addresses
    temp_sens1.i2c_obj = i2c0_obj;
    temp_sens1.i2c_periph_num = 0; //Because we are using i2c0
    temp_sens1.i2c_addr = SONAR_LEFT_I2C_ADDR;
    temp_sens1.sonar_num = SONAR_LEFT;
    tSonarLeft = new Thread(SonarTask, (void *) &temp_sens1);

    temp_sens2.i2c_obj = i2c0_obj;
    temp_sens2.i2c_periph_num = 0;
    temp_sens2.i2c_addr = SONAR_RIGHT_I2C_ADDR;
    temp_sens2.sonar_num = SONAR_RIGHT;
    tSonarRight = new Thread(SonarTask, (void *) &temp_sens2);



    tSonarHealth = new Thread(SonarHealthTask);
    Thread::wait(5);    //We must wait some time before the function ends or temp_sens? will be destroyed
                        //-> before the threads assign them to local variables. (I tested with 1ms and it was OK)
}



void SonarTask(void const *args) {
    const sonar_sensor_t * sonar = (const sonar_sensor_t *) args;

    I2C *i2c_obj = sonar->i2c_obj;
    uint8_t i2c_periph_num = sonar->i2c_periph_num;
    uint8_t i2c_addr = sonar->i2c_addr;
    uint8_t sonar_number = sonar->sonar_num;


    char* data;
    data = new char[4];  //allocate memory for "data" array

    uint16_t dist_reading = 0; //distance reading

    while (1) {
        //TODO Test Without this signal_wait
        Thread::signal_wait(SONAR_I2C_SIGNAL);

        //i2c_lock(i2c_periph_num);
        //i2c_obj->write(i2c_addr, cmd, 1, true); //Repeated start is true in i2c_obj->write, so it must be true in
        //i2c_obj->read(i2c_addr, thermistor_echo, 2, true); //-> the following read, too.
        //i2c_unlock(i2c_periph_num);

        char cmd_request = 0x80; // command to send to the sonar to request a distance reading
        char cmd_read = 0x40;    // command to send to the sonar to receive the reading


        i2c0_lock();
        i2c_obj->write(i2c_addr, &cmd_request, 1);   // request a distance reading from the sonar
        i2c0_unlock();

        //Only 100ms tested...
        wait_ms(50); // wait 50ms for the sonar to calculate the distance

        i2c0_lock();
        i2c_obj->write(i2c_addr, &cmd_read, 1);      // request to receive the reading from the sonar
        i2c0_unlock();

        wait_ms(50);

        i2c0_lock();
        i2c_obj->read(i2c_addr, data, 4);            // receive the reading and store it in the "data" array
        i2c0_unlock();

        //TODO SonarValuesSet()
        dist_reading = (data[2]<<8) + data[3];
        SonarValueSet(dist_reading, sonar_number);
    }
}

void i2c0_lock() {
    i2c0_mutex.lock();
}

void i2c0_unlock() {

    i2c0_mutex.unlock();

}

void SonarSignalClear(uint8_t grideye_num) {
    switch (grideye_num) {
    case SONAR_RIGHT:
        tSonarRight->signal_clear(SONAR_I2C_SIGNAL);
        break;
    case SONAR_LEFT:
        tSonarLeft->signal_clear(SONAR_I2C_SIGNAL);
        break;
    default:
        return;
    }
}

void SonarValueSet(uint16_t value, uint8_t sonar_num) {
    if (value>0 && value<512){
        HealthySonarValuesSet(value, sonar_num);
    }
    else{
        HealthySonarValuesSet(0, sonar_num);
    }

}

void SonarSchedulerTask(void const *args) {
    //I2C sensors in the same I2C bus have maximum distance ie 50ms in a 100ms loop
    while (true) {
        clearHealthySonar();
        if (SonarEnabled(SONAR_LEFT))
            tSonarLeft->signal_set(SONAR_I2C_SIGNAL);
        Thread::wait(25);
        if (SonarEnabled(SONAR_RIGHT))
            tSonarRight->signal_set(SONAR_I2C_SIGNAL);

        Thread::wait(1);
        tSonarHealth->signal_set(HEALTH_SIGNAL);

        Thread::wait(10);
    }
}
