/** @file
 * @author Orestis Zachariadis
 *
 * @brief Implements I2C communication with interrupts
 *
 * @b Instructions:
 * @li Include this file in i2c_api.c and disable i2c_clear_SI() and i2c_wait_SI() in i2c_api.c.
 * @li Call I2C?_queue_create()
 *
 * @note
 * Important note: due to MBED Library bug, i2c_write() and i2c_read() in i2c_api.c were changed in order
 * to support a repeated start i2c_read after an i2c_write call.
 * @n @n
 * Instructions if you want to use repeated start:
 * @li Delete original i2c_api.c and use i2c_api2.c in LibMods folder instead.
 * @li If you set "repeated" argument to true in I2C::write() the following I2C::read() must also
 * have "repeated" argument set to true.
 * @li An I2C transaction is not allowed to start with I2C::read() with "repeated" argument set to true.
 *
 * @attention As of now only the sequence write -> repeated start -> read is implemented.
 * @note Changes in original i2c_api.c are marked with "CHANGED MBED LIBRARY HERE" in comments.
 */

#ifndef I2C_NONBLOCKING_H_
#define I2C_NONBLOCKING_H_

#include "cmsis_os.h"
#include "cmsis.h"
#include "pinmap.h"
#include "error.h"
#include "objects.h"

#define SI 3	///<I2C Serial Interrupt flag

typedef struct i2c_s i2c_t;


//If queue.put() is executed inside an ISR, and we have continuous interrupts (not allowing non-ISR code to run),
//->queue uses a size 16 FIFO (independent from what the user sets). Shouldn't happen in a real case scenario.

void I2C0_IRQHandler();

void I2C1_IRQHandler();

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Create and Initialize I2C0 interrupt Message Queue */
void I2C0_queue_create();

/** @brief Create and Initialize I2C1 interrupt Message Queue */
void I2C1_queue_create();

#ifdef __cplusplus
}
#endif

/** Clear the Serial Interrupt (SI)
 * @note If we didn't clear pending interrupt, IRQHandler would trigger, even though interrupt flag would be clear
 * (for I2C SI=0 means status 0xF8).\n
 * Interrupt flag must be cleared BEFORE clearing the pending interrupt, or the pending interrupt would be set again.
 * @param obj i2c object
 */
inline void i2c_clear_SI(i2c_t *obj) ;

// Wait until the Serial Interrupt (SI) is set
int i2c_wait_SI(i2c_t *obj) ;

#endif /* I2C_NONBLOCKING_H_ */
