/** @file
 * @author Orestis Zachariadis
 * @brief Includes, defines and function prototypes used for USB functionality.
 */
#ifndef USB_HPP_
#define USB_HPP_

#include "mbed.h"
#include "rtos.h"
#include "USBSerial.h"
#include "grideye.hpp"

/** @name Possible incoming commands */
//@{
#define GEYE_CENTER_REQUEST 1	///<Requests temperature values from center GridEYE
#define GEYE_LEFT_REQUEST 2		///<Requests temperature values from left GridEYE
#define GEYE_RIGHT_REQUEST 3	///<Requests temperature values from right GridEYE
#define CO2_REQUEST 4			///<Requests CO2 volume value
//@}

/** @brief Initializes USB communication
 *
 * @note It would be a good idea that USBSerial is initialized with a number multiple of the incoming
 * packet size (number of bytes). Check USBSerial2.h
 */
void USBInit();

/** @brief ISR that is called when a USB packet is received.
 *
 *  Copies incoming packets from USBSerial CircBuffer to the RTOS queue that controls USB communication
 */
void command_recv_isr();

/** @brief Interprets incoming USB commands and responds accordingly
 *
 */
void USBTask(const void *args);

/** @brief Refreshes GridEYE USB buffers with new values
 *
 * Refreshes the usb buffer of the GridEYE sensor indicated by grideye_num.
 *
 * @param values New values to write to GridEYE USB buffer
 * @param grideye_num grideye_sensor_t::grideye_num
 */
void USBGridEYEvaluesSet(uint8_t values[], uint8_t grideye_num);

/** @brief Zeroes GridEYE USB buffers
 *
 * Zeroes out the usb buffer of the GridEYE sensor indicated by grideye_num. @n
 * We zero the buffer when the sensor failed to respond before timeout period. When a sensor is
 * not healthy its buffer remains zero.
 *
 * @param grideye_num grideye_sensor_t::grideye_num
 */
void USBGridEYEvaluesZero(uint8_t grideye_num);

/** @brief Read GridEYE USB buffers
 *
 * Used by USBTask to read the GridEYE USB buffer indicated by grideye_num.
 * @param grideye_num grideye_sensor_t::grideye_num
 */
uint8_t * USBGridEYEvaluesGet(uint8_t grideye_num);

/** @brief Refreshes CO2 USB buffer with new values
 *
 * @param value New value to write to CO2 USB buffer
 */
void USBCO2valueSet(float value);

/** @brief Read CO2 USB buffer
 *
 * Used by USBTask to read the CO2 USB buffer.
 */
float USBCO2valueGet();

#endif /* USB_HPP_ */
