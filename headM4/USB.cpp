/** @file
 * @author Nikos Taras
 * @brief Implements USB communication with the PC.
 */
#include "USB.hpp"

/** @name Buffers that store the values to be send to PC */
//@{
static float uCO2value;	///<usb CO2 value buffer
static uint8_t uEncoderValue;  ///<usb GridEYECenter values buffer
static uint8_t uSonarLeftValue;    ///<usb GridEYECenter values buffer
static uint8_t uSonarRightValue;   ///<usb GridEYECenter values buffer
//@}

static USBSerial *usb;	///<Pointer to the USBSerial class object that implements the USB communication

/**
 * @brief RTOS queue that stores incoming command packets.
 * USBTask() waits for incoming elements to this queue.
 */
static Queue<uint8_t, 20> UsbRecvQueue;

void USBInit() {
    usb = new USBSerial(20);	//blocks if USB not plugged in

    usb->attach(command_recv_isr);
}

void command_recv_isr() {
    uint8_t USBrecv;

    while (usb->available()) {
        USBrecv = usb->_getc();
        UsbRecvQueue.put((uint8_t *) USBrecv);
    }
}
//TODO CHANGE SIZE OF WRITEBLOCKS SENT (is it really 4?)
//TODO CREATE UNIONS (if needed) LIKE THE UNION BELOW FOR THE OTHER SENSORS
//TODO CHANGE RETURN TYPE OF ALL GETTERS
//TODO MODIFY THE CASES IN THE SWITCH BELOW

void USBTask(const void *args) {
    union {
        float CO2value;
        uint8_t CO2value_uint8;
    };
    uint8_t command;

    while (true) {
        command = UsbRecvQueue.get().value.v;

        switch (command) {
        case ENCODER_REQUEST:
            //writeBlock() waits for the host to connect
            usb->writeBlock(USBSonarValuesGet(GEYE_CENTER), PIXELS_COUNT);
            //Because the array we send is a multiple of max packet size (64 bytes) a zero-lenth-packet (ZLP) is
            //-> required after the data packet. If we don't send the ZLP the read() in the PC program would
            //-> wait forever. I tried sending 128 bytes, but this time it worked without the ZLP. So, if read()
            //-> blocks while waiting for a packet multiple of max packet size try ZLP.
            //To send a ZLP the second argument in writeBlock() must be 0. First argument can be anything.
            usb->writeBlock(&command, 0);
            break;
        case SONAR_RIGHT_REQUEST:
            usb->writeBlock(USBSonarValuesGet(SONAR_RIGHT), PIXELS_COUNT);
            usb->writeBlock(&command, 0);
            break;
        case SONAR_LEFT_REQUEST:
            usb->writeBlock(USBSonarValuesGet(SONAR_LEFT), 4);
            usb->writeBlock(&command, 0);
            break;
        case CO2_REQUEST:
            CO2value = USBCO2valueGet();
            usb->writeBlock(&CO2value_uint8, 4);
            break;
        default:
            break;
        }
    }
}

//A mutex may be required to protect set and get , but didn't have any problems without
//void USBSonarValuesSet(uint8_t values[], uint8_t sonar_num) {
//    switch (grideye_num) {
//    case GEYE_CENTER:
//        memcpy((void *) uEncoderValue, (const void *) values, PIXELS_COUNT * sizeof(uint8_t));
//        break;
//    case SONAR_RIGHT:
//        memcpy((void *) uSonarRightValue, (const void *) values, PIXELS_COUNT * sizeof(uint8_t));
//        break;
//    case SONAR_LEFT:
//        memcpy((void *) uSonarLeftValue, (const void *) values, PIXELS_COUNT * sizeof(uint8_t));
//        break;
//    }
//}

//void USBSonarValuesZero(uint8_t sonar_num) {
//    switch (grideye_num) {
//    case GEYE_CENTER:
//        memset(uEncoderValue, 0, PIXELS_COUNT * sizeof(uint8_t));
//        break;
//    case SONAR_RIGHT:
//        memset(uSonarRightValue, 0, PIXELS_COUNT * sizeof(uint8_t));
//        break;
//    case SONAR_LEFT:
//        memset(uSonarLeftValue, 0, PIXELS_COUNT * sizeof(uint8_t));
//        break;
//    }
//}

//A mutex may be required to protect set and get , but didn't have any problems without
//uint8_t  USBSonarValuesGet(uint8_t sonar_num) {
//    switch (grideye_num) {
//    case SONAR_RIGHT:
//        return uSonarRightValue;
//        break;
//    case SONAR_LEFT:
//        return uSonarLeftValue;
//        break;
//    }
//    return uSonarRightValue;    //Shouldn't come here
//}


//getters and setters for CO2
void USBCO2valueSet(float value) {
    uCO2value = value;
}

float USBCO2valueGet() {
    return uCO2value;
}

//getters and setters for encoder
void USBencoderValueSet(float value) {
    uEncoderValue = value;
}

float USBencoderValueGet() {
    return uEncoderValue;
}

//getters and setters for Sonar Left
void USBSonarLeftValueSet(float value) {
    uSonarLeftValue = value;
}

float USBSonarLeftValueGet() {
    return uSonarLeftValue;
}
//getters and setters for Sonar Right
void USBSonarRightValueSet(float value) {
    uSonarRightValue = value;
}

float USBSonarRightValueGet() {
    return uSonarRightValue;
}
