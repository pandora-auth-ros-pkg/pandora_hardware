/** @file
 * @author Orestis Zachariadis
 * @brief Implements sensor health monitoring functionality
 *
 * @note Replace {sensor_name} in the next paragraph with CO2 or GridEYE or another sensor accordingly.
 *
 * In the beginning of each sensor scanning cycle ( see {sensor_name}SchedulerTask() ) healthy status
 * of {sensor_name} is resetted ( clearHealthy{sensor_name}() is called, which sets {sensor_name}_healthy = 0 ).
 * If {sensor_name} responds in time, Healthy{sensor_name}valueSet() is called, which sets {sensor_name} as
 * healthy ( {sensor_name}_healthy = 1 ). But if {sensor_name} doesn't respond in time, {sensor_name}_healthy
 * stays 0. {sensor_name}HealthTask() is signaled after an appropriate time (timeout) to check if {sensor_name}
 * is healthy or not (checks if {sensor_name}_healthy was set or remained at zero ). If remained at zero, ie
 * {sensor_name} didn't answer in time, {sensor_name}HealthTask() starts monitoring the number of failures and
 *  attempts repair measures.
 *
 * @note Healthy{sensor_name}valueSet() is a direct replacement of USB{sensor_name}valueSet(). Thus we could just
 * bypass the health layer by calling USB{sensor_name}valueSet() instead of Healthy{sensor_name}valueSet() inside
 * the sensor's source file.
 * @par
 * @note To clear any confusion: {sensor_name}valueSet() in {sensor_name}.cpp just checks if the value(s) returned
 * is(are) inside limits as defined by {sensor_name}'s manufacturer and calls either  USB{sensor_name}valueSet() ( if
 * it just needs USB communication ) or Healthy{sensor_name}valueSet() (if it needs health monitoring layer)
 * @par
 * @note While a sensor is considered not healthy, its USB buffer is set to zero.
 */
#include "health.hpp"

/** @name Sensors health status
 * 1 if healty, 0 else */
//@{
static uint8_t CO2_healthy;
//static uint8_t GridEYECenter_healthy;
static uint8_t GridEYELeft_healthy;
//static uint8_t GridEYERight_healthy;
//@}

/** @name Repair loop index
 * Holds the index for the repair  loop. It is used to cycle through the different repair measures until the sensor or
 * peripheral is fixed */
//@{
static uint8_t CO2_FailIndex = 0;
//static uint8_t I2C0_FailIndex = 0;
static uint8_t I2C1_FailIndex = 0;
//@}

/** @name Countdown until a sensor is disabled
 * If a sensor fails to respond in time countdown times, it is disabled */
//@{
static uint8_t CO2_DisableCountdown = DISABLE_COUNTDOWN;
//static uint8_t GridEYECenter_DisableCountdown = DISABLE_COUNTDOWN;
static uint8_t GridEYELeft_DisableCountdown = DISABLE_COUNTDOWN;
//static uint8_t GridEYERight_DisableCountdown = DISABLE_COUNTDOWN;
//@}

/** @name Status Leds
 * If a sensor or assigned peripheral works, the led is blinking. In an I2C where more than one device may be connected,
 * the leds blink faster for every sensor we add */
//@{
static DigitalOut CO2_LifeLED(LED3);
static DigitalOut I2C0_LifeLED(LED1); // Sonars
//static DigitalOut I2C1_LifeLED(LED2); // Grideye
static DigitalOut Enc_LifeLED(LED4);
//@}

/** @name Enables / Disables the I2C analog switches
 * Used to turn on or off the analog switches that control the I2C buses power supply
 * @see MAX4674ESE+ */
//@{
//static DigitalOut I2C0_switch(p30);
//static DigitalOut I2C1_switch(p29);
//@}

static Mutex WDT_mutex; ///<Mutex that protects Watchdog feeding sequence

void HealthInit() {
//    I2C0_switch = 0;    //Active Low
//    I2C1_switch = 0;    //Active Low
#if !DEVELOPMENT
    LPC_WDT->TC = (int) ((WDT_MS / 1000) * (500000 / 4));
    LPC_WDT->MOD = (1 << WDRESET) | (1 << WDEN);    //enable watchdog reset
    WDT_feed();    //A valid feed sequence must be completed after setting WDEN before the Watchdog
                   //-> is capable of generating a reset
#endif
}

void CO2HealthTask(void const *args) {
    while (true) {
        Thread::signal_wait(HEALTH_SIGNAL);

        WDT_feed();

        if (!CO2_healthy && CO2_DisableCountdown) {
            CO2_DisableCountdown--;
            CO2_FailIndex++;
            if (CO2_FailIndex == 1) {
                USBCO2valueSet(0);
            }
            repairCO2(CO2_FailIndex);
        }
    }
}

void GridEYEHealthTask(void const *args) {
    while (true) {
        Thread::signal_wait(HEALTH_SIGNAL);

        WDT_feed();

        //if (!GridEYECenter_healthy && GridEYECenter_DisableCountdown) {
        //    GridEYECenter_DisableCountdown--;
        //}
        //if (!GridEYERight_healthy && GridEYERight_DisableCountdown) {
        //    GridEYERight_DisableCountdown--;
        //}
        //if (!GridEYECenter_healthy && !GridEYERight_healthy) {
         //   I2C0_FailIndex++;
         //   if (I2C0_FailIndex == 1) {
         //       USBGridEYEvaluesZero(GEYE_RIGHT);
         //       USBGridEYEvaluesZero(GEYE_CENTER);
         //   }

            //Normally, when signal_wait() is called the signal flags get cleared automatically. But if the sensor is stuck
            //-> inside the main loop in GridEYETask(), signal flags are set again by GridEYESchedulerTask(). That means
            //-> when a repair manages to unstuck the sensor, the request for new data from the sensor will start
            //-> immediately, without waiting on signal_wait() for signal_set() from SchedulerTask().
            //We clear signal flags before attempting repair, so that in case of successful repair GridEYETask() doesn't
            //-> continue its loop before it takes the OK from GridEYESchedulerTask().
         //   GridEYESignalClear(GEYE_RIGHT);
         //   GridEYESignalClear(GEYE_CENTER);
         //   repairI2C(I2C0_FailIndex, I2C_0);
        //}
        if (!GridEYELeft_healthy && GridEYELeft_DisableCountdown) {
            GridEYELeft_DisableCountdown--;
            I2C1_FailIndex++;
            if (I2C1_FailIndex == 1) {
                USBGridEYEvaluesZero(GEYE_LEFT);
            }
            GridEYESignalClear(GEYE_LEFT);
            repairI2C(I2C1_FailIndex, I2C_1);
        }
    }
}

void clearHealthyCO2() {
    CO2_healthy = 0;
}

void clearHealthyGridEYE() {
   // GridEYECenter_healthy = 0;
    GridEYELeft_healthy = 0;
 //   GridEYERight_healthy = 0;
}

void HealthyCO2valueSet(float value) {
    CO2_healthy = 1;
    CO2_FailIndex = 0;
    CO2_DisableCountdown = DISABLE_COUNTDOWN;
    USBCO2valueSet(value);
    CO2_LifeLED = !CO2_LifeLED;
}

void HealthySonarValueSet(uint16_t value, uint8_t sonar_num){
    USBSonarValuesSet(value, sonar_num);
    I2C0_LifeLED = !I2C0_LifeLED;
}

void HealthyEncoderValueSet(uint16_t reading){
    USBencoderValueSet(reading);
    Enc_LifeLED = !Enc_LifeLED;
}

void HealthyGridEYEvaluesSet(uint8_t values[], uint8_t grideye_num) {
    switch (grideye_num) {
  //  case GEYE_CENTER:
  //      GridEYECenter_healthy = 1;
  //      I2C0_FailIndex = 0;
  //      GridEYECenter_DisableCountdown = DISABLE_COUNTDOWN;
  //      USBGridEYEvaluesSet(values, grideye_num);
   //     I2C0_LifeLED = !I2C0_LifeLED;
  //      break;
  //  case GEYE_RIGHT:
  //      GridEYERight_healthy = 1;
  //      I2C0_FailIndex = 0;
  //      GridEYERight_DisableCountdown = DISABLE_COUNTDOWN;
  //      USBGridEYEvaluesSet(values, grideye_num);
  //      I2C0_LifeLED = !I2C0_LifeLED;
  //      break;
    case GEYE_LEFT:
        GridEYELeft_healthy = 1;
        I2C1_FailIndex = 0;
        GridEYELeft_DisableCountdown = DISABLE_COUNTDOWN;
        USBGridEYEvaluesSet(values, grideye_num);
        //I2C1_LifeLED = !I2C1_LifeLED;
        break;
    default:
        return;
    }
}

uint8_t GridEYEenabled(uint8_t grideye_num) {
    switch (grideye_num) {
   // case GEYE_CENTER:
   //     return GridEYECenter_DisableCountdown;
   // case GEYE_RIGHT:
   //     return GridEYERight_DisableCountdown;
    case GEYE_LEFT:
        return GridEYELeft_DisableCountdown;
    default:
        return 1;
    }
    return 1;
}

uint8_t CO2enabled() {
    return CO2_DisableCountdown;
}

void repairCO2(uint8_t index) {
    if (index == 3) {
        CO2Trigger();
    }

    //Determines the rate at which the repair measures are repeated (we can't know how long the cause of the problem lasts)
    if (CO2_FailIndex > 5) {
        CO2_FailIndex = 1;
    }
}

void repairI2C(uint8_t index, int i2c_base) {
    LPC_I2C_TypeDef *i2c_periph = (LPC_I2C_TypeDef *) i2c_base;

    if ((index >= 3) && (i2c_periph->STAT == 0)) {
        i2c_periph->CONSET = 1 << I2C_STOP;
        i2c_periph->CONCLR = 1 << I2C_SI;
        return;
    }
    if (index == 3) {
        if (i2c_periph->CONSET & 1 << I2C_START) {
            i2c_periph->CONSET = 1 << I2C_STOP;
        } else {
            i2c_periph->CONSET = 1 << I2C_START;
        }
    } else if (index == 5) {
        i2c_periph->CONSET = 1 << I2C_SI;
    } else if (index == 7) {
        i2c_periph->CONSET = 1 << I2C_START;
        i2c_periph->CONSET = 1 << I2C_STOP;
    } //else if (index == 10) {
       // if (i2c_base == I2C_0) {
       //     I2C0_switch = !I2C0_switch; //turn off I2C bus
       //     Thread::wait(10);   //Probably only a few uSeconds are enough to turn off but I didn't test
       //     I2C0_switch = !I2C0_switch; //turn on I2C bus
       // } else
        //if (i2c_base == I2C_1) {
         //   I2C1_switch = !I2C1_switch; //turn off I2C bus
         //   Thread::wait(10);   //Probably only a few uSeconds are enough to turn off but I didn't test
         //   I2C1_switch = !I2C1_switch; //turn on I2C bus
    //    }

       // Thread::wait(60); //Time to enable communication after setup is 50ms according to GridEYE datasheet

        //Resets uC I2C state
       // i2c_periph->CONSET = 1 << I2C_START;
       //i2c_periph->CONSET = 1 << I2C_STOP;
    //}

    //Determines the rate at which the repair measures are repeated (we can't know how long the cause of the problem lasts)
   // if (I2C0_FailIndex > 40) {
    //    I2C0_FailIndex = 1;
    //}
    if (I2C1_FailIndex > 7) {
        I2C1_FailIndex = 1;
    }
}

void WDT_feed() {
    WDT_mutex.lock();
    LPC_WDT->FEED = 0xAA;
    LPC_WDT->FEED = 0x55;
    WDT_mutex.unlock();
}
