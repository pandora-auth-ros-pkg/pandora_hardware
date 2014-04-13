#include "i2c_nonblocking.h"

static osMessageQDef(I2C_QUEUE, 1, uint32_t);
static osMessageQId  I2C_QUEUE;

//static int dok;

void I2C1_IRQHandler(){
	NVIC_DisableIRQ(I2C_IRQN);
	osMessagePut(I2C_QUEUE, (uint32_t)1, 0);
}

inline void i2c_clear_SI(i2c_t *obj) {
	obj->i2c->CONCLR = 1 << SI;
	NVIC_ClearPendingIRQ(I2C_IRQN);
	NVIC_EnableIRQ(I2C_IRQN);
}

int i2c_wait_SI(i2c_t *obj) {
	osEvent evt = osMessageGet(I2C_QUEUE, osWaitForever);	//well, maybe forever is too long
    return 0;
}
