/** @file
 * @author Orestis Zachariadis
 *
 * @brief Includes, defines and function prototypes for interrupt based I2C communication implementation
 *
 * @b Instructions:
 * @li Include this file in i2c_api.c and disable %i2c_clear_SI() and %i2c_wait_SI() in i2c_api.c.
 * @li Call I2C?_queue_create() ()
 * @li Make a @b thread that will use the functions of the MBED I2C api (http://mbed.org/handbook/I2C)
 * to communicate with a sensor (api is @b exactly the same). Because as a thread it will be put
 * into WAITING state while waiting for SI interrupt and leave the processor free for other tasks.
 *
 * @n
 * Also check i2c_api.c file preamble comments.
 */

#ifndef I2C_NONBLOCKING_H_
#define I2C_NONBLOCKING_H_

#include "cmsis_os.h"
#include "cmsis.h"
#include "pinmap.h"
#include "error.h"
#include "objects.h"

#define SI 3    ///<I2C Serial Interrupt flag

typedef struct i2c_s i2c_t; ///<Contains a member, i2c, which points to i2c peripheral memory address

/** @brief I2C0 Interrupt Handler
 *
 * Disables interrupts
 * @note We must disable interrupts because we don't clean the SI flag yet.
 * Otherwise the ISR would retrigger infinitely.
 *
 * Then adds an element to I2C0_queue
 * @note Doesn't matter what element, we use the queue only as signaling mechanism. We can't use
 * RTOS signals because they can notify only specific threads.
 */
void I2C0_IRQHandler();

/** @brief I2C1 Interrupt Handler
 *
 * Disables interrupts
 * @note We must disable interrupts because we don't clean the SI flag yet.
 * Otherwise the ISR would retrigger infinitely.
 *
 * Then adds an element to I2C1_queue
 * @note Doesn't matter what element, we use the queue only as signaling mechanism. We can't use
 * RTOS signals because they can notify only specific threads.
 */
void I2C1_IRQHandler();

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Create and Initialize I2C0 interrupt Message Queue
 *
 * Also enables corresponding I2C interrupt. It is called from i2c_init() in i2c_api.c.
 */
void I2C0_queue_create();

/** @brief Create and Initialize I2C1 interrupt Message Queue
 *
 * Also enables corresponding I2C interrupt. It is called from i2c_init() in i2c_api.c.
 */
void I2C1_queue_create();

#ifdef __cplusplus
}
#endif

/** @brief Clears the Serial Interrupt (SI)
 *
 * Clears SI and enables interrupts
 * @note If we didn't clear pending interrupt, IRQHandler would trigger, even though interrupt flag would be clear
 * (for I2C SI=0 means status 0xF8).\n
 * Interrupt flag must be cleared BEFORE clearing the pending interrupt, or the pending interrupt would be set again.
 * @param obj pointer to i2c_t struct of the corresponding I2c peripheral
 */
inline void i2c_clear_SI(i2c_t *obj);

/** @brief Wait for Serial Interrupt (SI)
 *
 * Waits an interrupt of the corresponding I2c peripheral to fill the queue so it can continue execution. While
 * it waits the thread is put into WAITING state, so the CPU is free for other tasks.
 * @param obj pointer to i2c_t struct of the corresponding I2c peripheral
 * @return 0 if successful
 */
int i2c_wait_SI(i2c_t *obj);

#endif /* I2C_NONBLOCKING_H_ */
