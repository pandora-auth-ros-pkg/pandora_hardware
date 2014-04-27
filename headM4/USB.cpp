#include "USB.hpp"

//I copied USBSerial.h to USBSerial2.h. Only changed the constructor so we can
//input the circular buffer size (buf). It would be a good idea for the buffer
//size to be a multiple of the incoming packet size. Haven't tested for packets
//bigger than 64 byte.
//The circular buffer fills up transparently with a callback, so we don't have
//to worry about missing packets.
static USBSerial *usb;

void USBInit() {
	usb = new USBSerial(20);	//blocks if USB not plugged in
}
//USBTask could be made as interrupt callback
void USBTask(const void *args) {
	union {
		float CO2value;
		uint8_t CO2value_uint8;
	};
	uint8_t command;

	while (true) {
		Thread::wait(COMMAND_POLLING_TIME);
		if (usb->readable()) {
			command = usb->_getc();

			switch (command) {
				case GEYE_CENTER_REQUEST:
					//writeBlock() waits for the host to connect
					usb->writeBlock(GridEYEvaluesGet(GEYE_CENTER), PIXELS_COUNT);
					//Because the array we send is a multiple of max packet size (64 bytes) a zero-lenth-packet (ZLP) is
					//-> required after the data packet. If we don't send the ZLP the read() in the PC program would
					//-> wait forever. I tried sending 128 bytes, but this time it worked without the ZLP. So, if read()
					//-> blocks while waiting for a packet multiple of max packet size try ZLP.
					//To send a ZLP the second argument in writeBlock() must be 0. First argument can be anything.
					usb->writeBlock(&command, 0);
					break;
				case GEYE_LEFT_REQUEST:
					usb->writeBlock(GridEYEvaluesGet(GEYE_LEFT), PIXELS_COUNT);
					usb->writeBlock(&command, 0);
					break;
				case GEYE_RIGHT_REQUEST:
					usb->writeBlock(GridEYEvaluesGet(GEYE_RIGHT), PIXELS_COUNT);
					usb->writeBlock(&command, 0);
					break;
				case CO2_REQUEST:
					CO2value = CO2valueGet();
					usb->writeBlock(&CO2value_uint8, 4);
					break;
				default:
					break;
			}

		}

	}
}
