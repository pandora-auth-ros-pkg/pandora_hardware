#ifndef MBED_SERIAL2_H
#define MBED_SERIAL2_H

#include "platform.h"

#if DEVICE_SERIAL

#include "Stream.h"
#include "SerialBase.h"
#include "serial_api.h"
#include "Serial.h"

namespace mbed {

/** A serial port (UART) for communication with other serial devices
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
	 */
	int putcNB(int c){
		serial_t obj = this->_serial;
//		while (!serial_writable(&obj));
		obj.uart->THR = c;
		return c;
	}

	/** A non-blocking version of _getc().
	 *
	 * @return Returns the character from the serial port.
	 * @note Based on serial_gettc() in serial_api.c . To use replace Serial
	 * object declarations with Serial2
	 */
	int getcNB() {
		serial_t obj = this->_serial;
	    return obj.uart->RBR;
	}

/*####  If you want to make every I/O function Non-Blocking uncomment the following  #####*/
/*protected:
    virtual int _getc(){
		serial_t obj = this->_serial;
	    return obj.uart->RBR;
	}

    virtual int _putc(int c){
		serial_t obj = this->_serial;
		obj.uart->THR = c;
		return c;
	}*/

};

} // namespace mbed

#endif

#endif
