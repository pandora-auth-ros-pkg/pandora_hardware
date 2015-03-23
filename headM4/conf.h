/** @file
 * @author Orestis Zachariadis
 * @brief Configuration file
 */
#ifndef CONF_H_
#define CONF_H_

/** @brief Choose if we compile stuff appropriate for development or deployment enviroment
 *
 * If 1 we are in development phase eg watchdog disabled, cpu load monitoring enabled, HardFault leads to blinking
 * leds instead of resetting the processor etc */
#define DEVELOPMENT 1

/** @brief Enables debug output to USB Virtual Serial Port
 * @see http://mbed.org/handbook/SerialPC*/
#define DEBUG_OUT 0

/** @brief Expression to print debug messages to USB Virtual Serial Port
 * @note DEBUG_OUT should be enabled
 *
 * Example:
 * @code DEBUG_PRINT(("Text %d\r\n", variable));
 * @endcode
 * @note Double parentheses required */
#define DEBUG_PRINT(x)  if (DEBUG_OUT) printf x

/** @brief Enables RGB LED Matrix for GridEYE temperature data output */
#define ENABLE_RGB_LEDMATRIX 1

#endif /* CONF_H_ */
