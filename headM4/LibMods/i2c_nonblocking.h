//Instructions: 1) Include this file in i2c_api.c and disable i2c_clear_SI() and i2c_wait_SI() in i2c_api.c.
//2) In main()  call I2C?_queue_create()
//Important note: due to MBED Library bug i2c_write() and i2c_read() in i2c_api.c were changed, in order
//-> to support a repeated start i2c_read after an i2c_write call. If you want repeated start functionality
//-> delete original i2c_api.c and use i2c_api2.c in LibMods folder instead. Also there will be other additions in
//-> i2c_api2.c. All changes are marked with "CHANGED MBED LIBRARY HERE" in comments.
//Instructions if you want to use repeated start: 1) If you set "repeated" argument to true in I2C::write() the
//->  following I2C::read() must also have "repeated" argument set to true. 2) An I2C transaction is not allowed to
//-> start with I2C::read() with "repeated" argument set to true.

//Things to note: 1) Interrupts must be enabled in NVIC too (I2C is enabled only in NVIC), 2) NVIC_SetPriority() is
//-> optional, default priority is 0 (highest), 3) if we want to choose the ISR during runtime, dynamic vectors must
//-> be used (MBED has NVIC_SetVector() implemented for this task), 4) interrupt priority registers for interrupts 32-40 don't appear to LPCExpresso's
//-> Peripheral+ view, but they exist if we check the memory manually

#ifndef I2C_NONBLOCKING_H_
#define I2C_NONBLOCKING_H_

#include "cmsis_os.h"
#include "cmsis.h"
#include "pinmap.h"
#include "error.h"
#include "objects.h"

#define SI 3	///I2C Serial Interrupt flag

typedef struct i2c_s i2c_t;

//If queue.put() is executed inside an ISR, and we have continuous interrupts (not allowing non-ISR code to run),
//->queue uses a size 16 FIFO (independent from what the user sets). Shouldn't happen in a real case scenario.
//Things to note: 1) extern "C" is required if handler executed in C++ code, 2) Interrupt flag should be cleared
//-> or we will enter the ISR continuously, 3) an ISR can't be interrupted by the same interrupt that caused it,
//->only by a higher priority one.
void I2C0_IRQHandler();

void I2C1_IRQHandler();

#ifdef __cplusplus
extern "C" {
#endif

/// Create and Initialize I2C0 interrupt Message Queue
void I2C0_queue_create();

/// Create and Initialize I2C1 interrupt Message Queue
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
