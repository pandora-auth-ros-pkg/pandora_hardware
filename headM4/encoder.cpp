/*

 * encoder.cpp
 *
 *  Created on: Jan 3, 2015
 *      Author: Nikos Taras
 */

#include "encoder.hpp"

//static SPI *encoderSPI; ///<Pointer to the Serial2 class object that implements the CO2 sensor

//These are used in order to show the used ARM pins in main()
static PinName NN;  //Not Needed, MOSI spi
static PinName DO;  //Data out, MISO spi
static PinName SCL; //SPI Clock
static DigitalOut *CS; // Chip Select, this pin is Defined Here!

#define ENCODER_OFFSET 445


void EncoderInit(PinName pin1, PinName pin2, PinName pin3, PinName pin4){
    NN = pin1;
    DO = pin2;
    SCL = pin3;
    CS = new DigitalOut(pin4);
    *CS = 1;  //CS becomes 0 in order to communicate with the encoder

}

//TODO Modify EncoderValueSet and create Health for Encoder

void receiveEncoderData() {
    //Check comment about sdram in Doxygen main page before using new
    SPI encoderSPI(NN, DO, SCL);
    //The mosi pin of the SPI is not needed. So it's Not Connected (NC)
    //encoderSPI = new SPI(NC, DO, SCL);
    // Setup the spi for 12 bit data, mode 3.
    // In fact, the encoder communicates with 10 bits SPI,
    // but we have to use 12 bit and the first two must be
    // thrown away, according to the encoder datasheet.
    encoderSPI.format(12,3);
    encoderSPI.frequency(500000);

    uint16_t encoderReading = 5000;
    //Select the device by setting chip select low
    *CS = 0;

    //Send a dummy byte to receive the contents of the shift register
    encoderReading = encoderSPI.write(0b0);

    // Deselect the device
    *CS = 1;

    // Here we keep only the last 10bits of the SPI communication according
    // to the encoder datasheet.
    encoderReading &= 0b001111111111;
    if (encoderReading == 5000){
        EncoderValueSet(0);
    }
    else
        EncoderValueSet(encoderReading);

}

void EncoderValueSet(uint16_t reading){
    if (reading > 0 && reading <4096)
        HealthyEncoderValueSet(reading);
    else
        USBencoderValueSet(0);

}

void EncoderSchedulerTask(void const *args) {
    while (true) {

        Thread::wait(100);
        receiveEncoderData();

    }
}

