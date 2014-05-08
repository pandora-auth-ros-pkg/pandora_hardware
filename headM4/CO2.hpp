/** @file
 * @author Orestis Zachariadis
 * @brief Includes, defines and function prototypes used for the CO2 sensor
 */
#ifndef CO2_HPP_
#define CO2_HPP_

#include "mbed.h"
#include "rtos.h"
#include "health.hpp"

#include "LibMods/Serial2.h"


/** @name CO2 Sensor Protocol Instructions */
//@{
#define RD   0x13        ///<Read
#define DLE 0x10            ///<Data Link Escape
#define Var_ID 0x06         ///<Variable ID live data simple
#define NAK 0x19            ///<Negative Acknowledge
#define DAT 0x1A            ///<Single Data Frame
#define EOFF 0x1F          ///<End of Frame (Can't use EOF because is already used by the system)
#define Checksum_hi 0x00    ///<Checksum High byte
#define Checksum_lo 0x58    ///<Checksum low byte
//@}

/** @brief CO2 sensor initialization
 *
 * Initialize CO2 sensor communication and required threads */
void CO2Init(PinName tx, PinName rx) ;

/**
 * @brief Live Data Simple request to CO2 sensor
 *
 * @note We use the fact that uC UART FIFO is 16 bytes long and we are sending this 7 byte
 * message periodically after a long period of time (from UART's point of view) to
 * achieve maximum performance without using DMA or interrupts.
 */
void CO2Trigger();

/** @brief CO2 character receive ISR
 *
 * Grabs the incoming character and puts it in a queue for CO2ReceiverTask() to receive
 */
void RX_isr();

/** @brief CO2 incoming characters handling
 *
 *	Takes each consecutive incoming character and passes them through a FSM to create the data packet.
 */
void CO2ReceiverTask(void const *args);

/** @brief Sets the CO2 buffer so that it will be available for transfer
 *
 * Checks if gas reading is inside it's manufacturer's specified limits and sets the CO2 buffer that will be
 * send over USB after it passes through the health monitoring layer.
 *
 * @param value The value that was just read from the CO2 sensor
 */
void CO2valueSet(float value);

/** @brief Schedules when we trigger the CO2 sensor
 *
 * Manages CO2 sensor triggering period and signals CO2HealthTask() after a timeout period to check if there
 * was any response.
 */
void CO2SchedulerTask(void const *args);


#endif /* CO2_HPP_ */
