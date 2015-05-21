/** @file
 * @author Nikos Taras
 * @brief Implements USB communication with the PC.
 */
#include "USB.hpp"

/** @name Buffers that store the values to be send to PC */
//@{
static float uCO2value;	///<usb CO2 value buffer
static uint16_t uEncoderValue;  ///<usb GridEYECenter values buffer
static uint16_t uSonarLeftValue;    ///<usb GridEYECenter values buffer
static uint16_t uSonarRightValue;   ///<usb GridEYECenter values buffer
static uint16_t uBatteryMotorValue;
static uint16_t uBatterySupplyValue;
static uint8_t uGridEYECenterValues[PIXELS_COUNT];  ///<usb GridEYECenter values buffer

Mutex co2_m;
Mutex encoder_m;
Mutex battery_m;
Mutex geye_m;
Mutex sonar_m;
Mutex usb_m;


//@}

// TODO Fill up the queue on purpose.

static USBSerial *usb;	///<Pointer to the USBSerial class object that implements the USB communication

/**
 * @brief RTOS queue that stores incoming command packets.
 * USBTask() waits for incoming elements to this queue.
 */
static Queue<uint8_t, 128> UsbRecvQueue;

void USBInit() {
    usb = new USBSerial(128);	//blocks if USB not plugged in
    usb->attach(command_recv_isr);
}

void command_recv_isr() {
    uint8_t USBrecv;
    uint8_t i=0;
    while (usb->available()) {

        // DEBUG_PRINT(("R\n"));
        USBrecv = usb->_getc();
        UsbRecvQueue.put((uint8_t *) USBrecv);
        // DEBUG_PRINT(("W\n"));
    }
}

//TODO MODIFY THE CASES IN THE SWITCH BELOW
//TODO UPDATE FUNCTION NAMES IN HEADER FILE(S)

void USBTask(const void *args) {

    //used for converting float to uint8_t
    union {
        float CO2value;
        uint8_t CO2value_uint8;
    };

    //used for converting uint16_t to uint8_t
    union {
        uint16_t value16bit;
        uint8_t value8bit;
    };


    uint8_t command;
    DEBUG_PRINT(("USBTASK\r\n"));
    while (true) {
        command = UsbRecvQueue.get().value.v;
        DEBUG_PRINT(("R"));
        if (command>0 && command<8){
            //should return ACK
                        value16bit = USB_ACK;
                        usb->writeBlock(&value8bit,2);
                        usb->writeBlock(&command, 0);
        }
        else{
            //should return NACK
                        value16bit = USB_NACK;
                        usb->writeBlock(&value8bit,2);
                        usb->writeBlock(&command, 0);
                        continue;
        }

        switch (command) {

        case GEYE_CENTER_REQUEST:
                    //writeBlock() waits for the host to connect
                    usb->writeBlock(USBGridEYEvaluesGet(GEYE_LEFT), PIXELS_COUNT);
                    usb->writeBlock(&command, 0);
                    break;
        case ENCODER_REQUEST:

            value16bit = USBencoderValueGet();
            //writeBlock() waits for the host to connect
            usb->writeBlock(&value8bit, 2);
            //Because the array we send is a multiple of max packet size (64 bytes) a zero-lenth-packet (ZLP) is
            //-> required after the data packet. If we don't send the ZLP the read() in the PC program would
            //-> wait forever. I tried sending 128 bytes, but this time it worked without the ZLP. So, if read()
            //-> blocks while waiting for a packet multiple of max packet size try ZLP.
            //To send a ZLP the second argument in writeBlock() must be 0. First argument can be anything.
            usb->writeBlock(&command, 0);
            break;
        case SONAR_RIGHT_REQUEST:

            value16bit = USBSonarRightValueGet();
            //usb->writeBlock(USBSonarValuesGet(SONAR_RIGHT), PIXELS_COUNT);
            usb->writeBlock(&value8bit, 2);
            usb->writeBlock(&command, 0);
            break;
        case SONAR_LEFT_REQUEST:

            value16bit = USBSonarLeftValueGet();
            //usb->writeBlock(USBSonarValuesGet(SONAR_LEFT), 4);
            usb->writeBlock(&value8bit, 2);
            usb->writeBlock(&command, 0);
            break;
        case CO2_REQUEST:
            CO2value = USBCO2valueGet();
            usb->writeBlock(&CO2value_uint8, 4);
            usb->writeBlock(&command, 0);
            break;
        case BATTERY_MOTOR_REQUEST:

            value16bit = USBbatteryMotorValueGet();
            usb->writeBlock(&value8bit, 2);
            usb->writeBlock(&command, 0);
            break;
        case BATTERY_SUPPLY_REQUEST:

            value16bit = USBbatterySupplyValueGet();
            usb->writeBlock(&value8bit, 2);
            usb->writeBlock(&command, 0);
            break;
        default:
            break;
        }
        DEBUG_PRINT(("W"));
        DEBUG_PRINT(("%d",sizeof(UsbRecvQueue)));
    }
}

//A mutex may be required to protect set and get , but didn't have any problems without
void USBGridEYEvaluesSet(uint8_t values[], uint8_t grideye_num) {
    switch (grideye_num) {
    case GEYE_LEFT:
        memcpy((void *) uGridEYECenterValues, (const void *) values, PIXELS_COUNT * sizeof(uint8_t));
        break;
    }
}

void USBGridEYEvaluesZero(uint8_t grideye_num) {
    switch (grideye_num) {
    case GEYE_LEFT:
        memset(uGridEYECenterValues, 0, PIXELS_COUNT * sizeof(uint8_t));
        break;
    }
}

//A mutex may be required to protect set and get , but didn't have any problems without
uint8_t * USBGridEYEvaluesGet(uint8_t grideye_num) {
    switch (grideye_num) {
    case GEYE_LEFT:
        return uGridEYECenterValues;
        break;
    }
    return uGridEYECenterValues;    //Shouldn't come here
}

//A mutex may be required to protect set and get , but didn't have any problems without
void USBSonarValuesSet(uint16_t values, uint8_t sonar_num) {

    switch (sonar_num) {
    case SONAR_RIGHT:
        uSonarRightValue = values;
        break;
    case SONAR_LEFT:
        uSonarLeftValue = values;
        break;
    }
}


//getters and setters for CO2
void USBCO2valueSet(float value) {
    uCO2value = value;
}

float USBCO2valueGet() {
    return uCO2value;
}

//getters and setters for encoder
void USBencoderValueSet(uint16_t value) {
    uEncoderValue = value;
}

uint16_t USBencoderValueGet() {
    return uEncoderValue;
}

//getters and setters for battery
void USBbatteryMotorValueSet(uint16_t value) {
    uBatteryMotorValue = value;
}

uint16_t USBbatteryMotorValueGet() {
    return uBatteryMotorValue;
}

void USBbatterySupplyValueSet(uint16_t value) {
    uBatterySupplyValue = value;
}

uint16_t USBbatterySupplyValueGet() {
    return uBatterySupplyValue;
}


//getters  for Sonar Left


uint16_t USBSonarLeftValueGet() {
    return uSonarLeftValue;
}
//getters for Sonar Right


uint16_t USBSonarRightValueGet() {
    return uSonarRightValue;
}
