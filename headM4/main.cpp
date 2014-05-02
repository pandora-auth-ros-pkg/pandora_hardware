#include "mbed.h"
#include "rtos.h"
#include "statistics.hpp"
#include "grideye.hpp"
#include "CO2.hpp"
#include "USB.hpp"
#include "health.hpp"
//#include "gpdma.h"
//#include "dsp.h"

//Note1: if EALib/sdram.cpp exists (even if we don't include sdram.h) _sbrk() in that file will override every call
//-> in malloc (this includes new) and the external sdram will be used instead of the internal. External is huge
//-> (32MB > 96KB) but slower (load increased from 7% to 11%). Also note that just calling sdram_disableMallocSdram()
//-> will not disable the override instead will make every call to malloc() return with -1, that could lead to HardFault
//-> when creating objects like Serial, I2C etc.

//Note2: Max number of threads allowed are 14. This can be changed in RTX_Conf_CM.c

//Note3: RTOS Timer callback functions are not executed, check issue https://github.com/mbedmicro/mbed/issues/276

int main (void) {
	printf("Start\r\n");

	CO2Init(p17, p18);	//p17=TX, p18=RX

    I2C i2c0(p32,p31);	//sda, scl
    I2C i2c1(p9,p10);
    GridEYEInit(&i2c0, &i2c1);

    USBInit();

    Thread tUSB(USBTask);

    //Comment out next line for performance boost.
    Thread tStatistics(CpuLoadTask, NULL, osPriorityIdle);

    Thread tCO2Caller(CO2SchedulerTask);

    Thread tGridEYECaller(GridEYESchedulerTask);

    Thread::wait(osWaitForever);
}

void mbed_die() {
	__disable_irq();	// dont allow interrupts to disturb the flash pattern

    gpio_t led_1; gpio_init_out(&led_1, LED1);
    gpio_t led_2; gpio_init_out(&led_2, LED2);
    gpio_t led_3; gpio_init_out(&led_3, LED3);
    gpio_t led_4; gpio_init_out(&led_4, LED4);

    while (1) {
        gpio_write(&led_1, 1);
        gpio_write(&led_2, 0);
        gpio_write(&led_3, 0);
        gpio_write(&led_4, 1);

        wait_ms(150);

        gpio_write(&led_1, 0);
        gpio_write(&led_2, 1);
        gpio_write(&led_3, 1);
        gpio_write(&led_4, 0);

        wait_ms(150);

        wait_ms(4000);
        NVIC_SystemReset();
    }
}

extern "C" void HardFault_Handler(void)
{
	mbed_die();
}
extern "C" void MemManage_Handler(void)
{
	mbed_die();
}
extern "C" void BusFault_Handler(void)
{
	mbed_die();
}
extern "C" void UsageFault_Handler(void)
{
	mbed_die();
}
