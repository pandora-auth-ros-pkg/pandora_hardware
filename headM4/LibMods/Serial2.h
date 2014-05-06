/** @file Serial2.h
 * @author Orestis Zachariadis
 * @brief
 */
#ifndef MBED_SERIAL2_H
#define MBED_SERIAL2_H

#include "platform.h"

#if DEVICE_SERIAL

#include "Stream.h"
#include "SerialBase.h"
#include "serial_api.h"
#include "Serial.h"

#define NONBLOCKING 0	//Makes every I/O function Non-Blocking

namespace mbed {

/** @class Serial2 Serial2.h "LibMods/Serial2.h"
 * @brief A serial port (UART) for communication with other serial devices
 *
 * Can be used for Full Duplex communication, or Simplex by specifying
 * one pin as NC (Not Connected)
 *
 * Example:
 * @code
 * // Print "Hello World" to the PC
 *
 * #include "mbed.h"
 *
 * Serial2 pc(USBTX, USBRX);
 *
 * int main() {
 *     pc.printf("Hello World\n");
 * }
 * @endcode
 */
class Serial2 : public Serial {

public:
    /** Create a Serial port, connected to the specified transmit and receive pins
     *
     *  @param tx Transmit pin
     *  @param rx Receive pin
     *
     *  @note
     *    Either tx or rx may be specified as NC if unused
     */
	Serial2(PinName tx, PinName rx, const char *name=NULL) :  Serial(tx, rx, name=NULL) {}

	/** A non-blocking version of _putc().
	 *
	 * @param c An integer char.
	 * @return c On successful completion, returns back the same character
	 * @note Based on serial_putc() in serial_api.c . To use replace Serial
	 * object declarations with Serial2
	 * @warning Care should be taken that there is empty space in the 16 byte FIFO before putting new characters
	 */
	int putcNB(int c){
		serial_t obj = this->_serial;
		obj.uart->THR = c;
		return c;
	}

	/** A non-blocking version of _getc().
	 *
	 * @return Returns the character from the serial port.
	 * @note Based on serial_getc() in serial_api.c . To use replace Serial
	 * object declarations with Serial2
	 * @warning Care should be taken that there is an available character in the 16 byte FIFO before calling this function
	 */
	int getcNB() {
		serial_t obj = this->_serial;
	    return obj.uart->RBR;
	}

protected:
#if NONBLOCKING
    virtual int _getc(){
		serial_t obj = this->_serial;
	    return obj.uart->RBR;
	}

    virtual int _putc(int c){
		serial_t obj = this->_serial;
		obj.uart->THR = c;
		return c;
	}
#endif

};

} // namespace mbed

#endif

#endif
