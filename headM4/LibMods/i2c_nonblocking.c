/** @file
 * @author Orestis Zachariadis
 * @brief Implements I2C communication with interrupts
 */
#include "i2c_nonblocking.h"

/**@brief Creates I2C0_queue Message Queue Definition.
 * @see http://www.keil.com/pack/doc/cmsis_rtx/modules.html*/
static osMessageQDef(I2C0_queue, 1, uint32_t)
;

/**@brief An one element queue that gets a value when an interrupt to the corresponding I2c peripheral occurs */
static osMessageQId I2C0_queue;

/**@brief Creates I2C1_queue Message Queue Definition.
 * @see http://www.keil.com/pack/doc/cmsis_rtx/modules.html*/
static osMessageQDef(I2C1_queue, 1, uint32_t)
;

/**@brief An one element queue that gets a value when an interrupt to the corresponding I2c peripheral occurs */
static osMessageQId I2C1_queue;

void I2C0_IRQHandler() {
    NVIC_DisableIRQ(I2C0_IRQn);
    osMessagePut(I2C0_queue, (uint32_t) 1, 0);
}

void I2C1_IRQHandler() {
    NVIC_DisableIRQ(I2C1_IRQn);
    osMessagePut(I2C1_queue, (uint32_t) 11, 0);
}

void I2C0_queue_create() {
    I2C0_queue = osMessageCreate(osMessageQ(I2C0_queue), NULL);
    NVIC_EnableIRQ(I2C0_IRQn);
}

void I2C1_queue_create() {
    I2C1_queue = osMessageCreate(osMessageQ(I2C1_queue), NULL);
    NVIC_EnableIRQ(I2C1_IRQn);
}

inline void i2c_clear_SI(i2c_t *obj) {
    obj->i2c->CONCLR = 1 << SI;
    if (obj->i2c == (LPC_I2C_TypeDef *) I2C_0) {
        NVIC_ClearPendingIRQ(I2C0_IRQn);
        NVIC_EnableIRQ(I2C0_IRQn);
    } else if (obj->i2c == (LPC_I2C_TypeDef *) I2C_1) {
        NVIC_ClearPendingIRQ(I2C1_IRQn);
        NVIC_EnableIRQ(I2C1_IRQn);
    }
}

int i2c_wait_SI(i2c_t *obj) {
    if (obj->i2c == (LPC_I2C_TypeDef *) I2C_0) {
        osMessageGet(I2C0_queue, osWaitForever);    //TODO: well, maybe forever is too long
    } else if (obj->i2c == (LPC_I2C_TypeDef *) I2C_1) {
        osMessageGet(I2C1_queue, osWaitForever);    //TODO: well, maybe forever is too long
    }
    return 0;
}
