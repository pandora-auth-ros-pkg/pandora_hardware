#include "mbed.h"
#include "rtos.h"
#include "statistics.hpp"
#include "grideye.hpp"
#include "CO2.hpp"
#include "USB.hpp"
#include "health.hpp"
//#include "gpdma.h"
//#include "dsp.h"

//Note1: if sdram.cpp is not commented out _sbrk() in that file will override every call in malloc (this includes new)
//-> and the external sdram will be used instead of the internal. External is huge (32MB > 96KB) but slower (load
//-> increased from 7% to 11%). Also note that just calling sdram_disableMallocSdram() will not disable the override
//-> instead will make every call to malloc() return with -1, that could lead to HardFault when creating objects
//-> like Serial, I2C etc.

//Note2: Max number of threads allowed are 14. This can be changed in RTX_Conf_CM.c

//Note3: RTOS Timer callback is not executed, check issue https://github.com/mbedmicro/mbed/issues/276

int main (void) {
	CO2Init(p17, p18);	//p17=TX, p18=RX

    I2C i2c0(p32,p31);	//sda, scl
    I2C i2c1(p9,p10);
    GridEYEInit(&i2c0, &i2c1);

    USBInit();

    Thread tUSB(USBTask);

    Thread tStatistics(CpuLoadTask, NULL, osPriorityIdle);

    Thread tCO2Caller(CO2SchedulerTask);

    Thread tGridEYECaller(GridEYESchedulerTask);

    Thread::wait(osWaitForever);
}
