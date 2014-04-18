#include "USB.hpp"

//I copied USBSerial.h to USBSerial2.h. Only changed the constructor so we can
//input the circular buffer size (buf). It would be a good idea for the buffer
//size to be a multiple of the incoming packet size. Haven't tested for packets
//bigger than 64 byte.
//The circular buffer fills up transparently with a callback, so we don't have
//to worry about missing packets.
static USBSerial usb(20);

Serial pcu(USBTX, USBRX);

void USBTask(const void *args) {
	union {
		float CO2value;
		uint8_t CO2value_uint8;
	};
	uint8_t command;

	while (true) {
		Thread::wait(COMMAND_POLLING_TIME);
		if (usb.readable()) {
			command = usb._getc();

			switch (command) {
				case 0:
					CO2value = CO2valueGet();
					usb.writeBlock(&CO2value_uint8, 4);
					break;
				default:
					break;
			}

		}

	}
}
