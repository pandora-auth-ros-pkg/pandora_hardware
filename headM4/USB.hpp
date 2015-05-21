/** @file
 * @author Nikos Taras
 * @brief Includes, defines and function prototypes used for USB functionality.
 */
#ifndef USB_HPP_
#define USB_HPP_

#include <sonar.hpp>
#include "mbed.h"
#include "rtos.h"
#include "USBSerial.h"
#include "grideye.hpp"

/** @name Possible incoming commands */
//@{
#define ENCODER_REQUEST 1        ///<Requests temperature values from encoder
#define SONAR_LEFT_REQUEST 2     ///<Requests temperature values from left Sonar
#define SONAR_RIGHT_REQUEST 3    ///<Requests temperature values from right Sonar
#define CO2_REQUEST 4           ///<Requests CO2 volume value
#define BATTERY_MOTOR_REQUEST 5 ///<Requests Motor Battery measurement value
#define BATTERY_SUPPLY_REQUEST 6 ///<Requests Supply Battery measurement value
#define GEYE_CENTER_REQUEST 7   ///<Requests temperature values from center GridEYE
//@}

#define USB_TIMEOUT 3000   ///<Time in millisecond until reset (usb blocked).

#define USB_NACK 0xFFF1
#define USB_ACK 0x0001

#define MAX_USB_BUFFER_SIZE 4

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



void USBSonarValuesSet(uint16_t values, uint8_t sonar_num);



//getters and setters for CO2
void USBCO2valueSet(float value);

float USBCO2valueGet();

//getters and setters for encoder
void USBencoderValueSet(uint16_t value);

uint16_t USBencoderValueGet();

//getters and setters for battery
void USBbatteryMotorValueSet(uint16_t value);

uint16_t USBbatteryMotorValueGet();

void USBbatterySupplyValueSet(uint16_t value);

uint16_t USBbatterySupplyValueGet();

//getters and setters for Sonar Left


uint16_t USBSonarLeftValueGet();
//getters and setters for Sonar Right


uint16_t USBSonarRightValueGet();

#endif /* USB_HPP_ */
