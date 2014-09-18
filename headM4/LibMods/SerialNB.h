/** @file SerialNB.h
 * @author Orestis Zachariadis
 * @brief Expands http://mbed.org/handbook/Serial with non-blocking functionality
 *
 * @note
 * Both putc and printf() use serial_putc() in serial_api.c.
 * @n
 * @n
 * Transmit FIFO size is 16. If we transmit new data and FIFO is full the new data gets lost.
 * Same goes for Receive FIFO according to user manual (Overrun Error).
 * @n
 * @n
 * UART peripheral starts to transmit immediately after feeding it. If we don't
 * feed with new data fast enough and transmit buffer gets empty we have a THRE
 * interrupt.
 * @n
 * @n
 * Required time to transmit n bytes is (1/Baud)*(1+8+1)*n
 */

#ifndef MBED_SERIALNB_H
#define MBED_SERIALNB_H

#include "platform.h"

#include "Stream.h"
#include "SerialBase.h"
#include "serial_api.h"
#include "Serial.h"

#define NONBLOCKING 0	///<Makes EVERY serial I/O function with this port Non-Blocking

namespace mbed {

/**
 * @brief A Non-Blocking serial port (UART) for communication with other serial devices
 *
 * Can be used for Full Duplex communication, or Simplex by specifying
 * one pin as NC (Not Connected). Inherits mbed::Serial to implement Non-Blocking functionality.
 *
 * Example:
 * @code
 * // Print "Hello World" to the PC
 *
 * #include "mbed.h"
 * #include "LibMods/SerialNB.h"
 *
 * SerialNB pc(USBTX, USBRX);
 *
 * int main() {
 *     pc.printf("Hello World\n");
 * }
 * @endcode
 */
class SerialNB: public Serial {

public:
    /** Create a Serial port, connected to the specified transmit and receive pins
     *
     *  @param tx Transmit pin
     *  @param rx Receive pin
     *  @param name NULL
     *
     *  @note
     *    Either tx or rx may be specified as NC if unused
     */
    SerialNB(PinName tx, PinName rx, const char *name = NULL) :
            Serial(tx, rx, name = NULL) {
    }

    /** A non-blocking version of _putc().
     *
     * @param c An integer char.
     * @return c On successful completion, returns back the same character
     * @note Based on serial_putc() in serial_api.c . To use replace Serial
     * object declarations with Serial2
     * @warning Care should be taken that there is empty space in the 16 byte outbound
     * FIFO before putting new characters
     */
    int putcNB(int c) {
        serial_t obj = this->_serial;
        obj.uart->THR = c;
        return c;
    }

    /** A non-blocking version of _getc().
     *
     * @return Returns the character from the serial port.
     * @note Based on serial_getc() in serial_api.c . To use replace Serial
     * object declarations with Serial2
     * @warning Care should be taken that there is an available character in the 16 byte inbound
     * FIFO before calling this function
     */
    int getcNB() {
        serial_t obj = this->_serial;
        return obj.uart->RBR;
    }

protected:
#if NONBLOCKING
    virtual int _getc() {
        serial_t obj = this->_serial;
        return obj.uart->RBR;
    }

    virtual int _putc(int c) {
        serial_t obj = this->_serial;
        obj.uart->THR = c;
        return c;
    }
#endif

};

} // namespace mbed

#endif
