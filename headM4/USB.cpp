/** @file
 * @author Orestis Zachariadis
 * @brief
 */
#include "USB.hpp"

static float uCO2value;
static uint8_t uGridEYECenterValues[PIXELS_COUNT];
static uint8_t uGridEYELeftValues[PIXELS_COUNT];
static uint8_t uGridEYERightValues[PIXELS_COUNT];

//I copied USBSerial.h to USBSerial2.h. Only changed the constructor so we can
//input the circular buffer size (buf). It would be a good idea for the buffer
//size to be a multiple of the incoming packet size. Haven't tested for packets
//bigger than 64 byte.
//The circular buffer fills up transparently with a callback, so we don't have
//to worry about missing packets. (Check USBSerial::EP2_OUT_callback())
static USBSerial *usb;

static Queue<uint8_t, 20> UsbRecvQueue;

void USBInit() {
	usb = new USBSerial(20);	//blocks if USB not plugged in

	usb->attach(command_recv_isr);
}

void command_recv_isr() {
	uint8_t USBrecv;

	while (usb->available()) {
		USBrecv = usb->_getc();
		UsbRecvQueue.put((uint8_t *)USBrecv);
	}
}

//USBTask could be made as interrupt callback
void USBTask(const void *args) {
	union {
		float CO2value;
		uint8_t CO2value_uint8;
	};
	uint8_t command;

	while (true) {
		command = UsbRecvQueue.get().value.v;

		switch (command) {
			case GEYE_CENTER_REQUEST:
				//writeBlock() waits for the host to connect
				usb->writeBlock(USBGridEYEvaluesGet(GEYE_CENTER), PIXELS_COUNT);
				//Because the array we send is a multiple of max packet size (64 bytes) a zero-lenth-packet (ZLP) is
				//-> required after the data packet. If we don't send the ZLP the read() in the PC program would
				//-> wait forever. I tried sending 128 bytes, but this time it worked without the ZLP. So, if read()
				//-> blocks while waiting for a packet multiple of max packet size try ZLP.
				//To send a ZLP the second argument in writeBlock() must be 0. First argument can be anything.
				usb->writeBlock(&command, 0);
				break;
			case GEYE_LEFT_REQUEST:
				usb->writeBlock(USBGridEYEvaluesGet(GEYE_LEFT), PIXELS_COUNT);
				usb->writeBlock(&command, 0);
				break;
			case GEYE_RIGHT_REQUEST:
				usb->writeBlock(USBGridEYEvaluesGet(GEYE_RIGHT), PIXELS_COUNT);
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

void USBGridEYEvaluesSet(uint8_t values[], uint8_t grideye_num) {
	switch (grideye_num) {
		case GEYE_CENTER:
			memcpy((void *)uGridEYECenterValues, (const void *)values, PIXELS_COUNT * sizeof(uint8_t));
			break;
		case GEYE_LEFT:
			memcpy((void *)uGridEYELeftValues, (const void *)values, PIXELS_COUNT * sizeof(uint8_t));
			break;
		case GEYE_RIGHT:
			memcpy((void *)uGridEYERightValues, (const void *)values, PIXELS_COUNT * sizeof(uint8_t));
			break;
	}
}

void USBGridEYEvaluesZero(uint8_t grideye_num) {
	switch (grideye_num) {
		case GEYE_CENTER:
			memset(uGridEYECenterValues, 0, PIXELS_COUNT * sizeof(uint8_t));
			break;
		case GEYE_LEFT:
			memset(uGridEYELeftValues, 0, PIXELS_COUNT * sizeof(uint8_t));
			break;
		case GEYE_RIGHT:
			memset(uGridEYERightValues, 0, PIXELS_COUNT * sizeof(uint8_t));
			break;
	}
}

uint8_t * USBGridEYEvaluesGet(uint8_t grideye_num) {
	switch (grideye_num) {
		case GEYE_CENTER:
			return uGridEYECenterValues;
			break;
		case GEYE_LEFT:
			return uGridEYELeftValues;
			break;
		case GEYE_RIGHT:
			return uGridEYERightValues;
			break;
	}
	return uGridEYECenterValues;	//Shouldn't come here
}

void USBCO2valueSet(float value) {
	uCO2value = value;
}

float USBCO2valueGet() {
	return uCO2value;
}
