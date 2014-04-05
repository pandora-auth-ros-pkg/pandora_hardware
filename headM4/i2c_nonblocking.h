//Instructions: 1) Include this file in i2c_api.c and disable i2c_clear_SI() and i2c_wait_SI() in i2c_api.c.
//2) In main() cmsis_os.h must be included, i2c interrupt enabled and an appropriate (C version) CMSIS RTOS queue created.

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

#define SI 3

#define I2C_IRQN I2C0_IRQn
#define I2C_QUEUE I2C0_queue

extern osMessageQId  I2C_QUEUE;


//If queue.put() is executed inside an ISR, and we have continuous interrupts (not allowing non-ISR code to run),
//->queue uses a size 16 FIFO (independent from what the user sets). Shouldn't happen in a real case scenario.
//Things to note: 1) extern "C" is required if handler executed in C++ code, 2) Interrupt flag should be cleared
//-> or we will enter the ISR continuously, 3) an ISR can't be interrupted by the same interrupt that caused it,
//->only by a higher priority one.
void I2C1_IRQHandler(){
	NVIC_DisableIRQ(I2C_IRQN);
	osMessagePut(I2C_QUEUE, (uint32_t)1, 0);
}


/** Clear the Serial Interrupt (SI)
 * @note If we didn't clear pending interrupt, IRQHandler would trigger, even though interrupt flag would be clear
 * (for I2C SI=0 means status 0xF8).\n
 * Interrupt flag must be cleared BEFORE clearing the pending interrupt, or the pending interrupt would be set again.
 * @param obj i2c object
 */
static inline void i2c_clear_SI(i2c_t *obj) {
	obj->i2c->CONCLR = 1 << SI;
	NVIC_ClearPendingIRQ(I2C_IRQN);
	NVIC_EnableIRQ(I2C_IRQN);
}

// Wait until the Serial Interrupt (SI) is set
static int i2c_wait_SI(i2c_t *obj) {
	osEvent evt = osMessageGet(I2C_QUEUE, osWaitForever);	//well, maybe forever is too long
    return 0;
}


#endif /* I2C_NONBLOCKING_H_ */
