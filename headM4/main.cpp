/** @file
 * @author Orestis Zachariadis
 * @brief
 */
#include "mbed.h"
#include "rtos.h"
#include "statistics.hpp"
#include "grideye.hpp"
#include "CO2.hpp"
#include "USB.hpp"
#include "health.hpp"
#include "conf.h"
//#include "gpdma.h"
//#include "dsp.h"

//Note1: if EALib/sdram.cpp exists (even if we don't include sdram.h) _sbrk() in that file will override every call
//-> in malloc (this includes new) and the external sdram will be used instead of the internal. External is huge
//-> (32MB > 96KB) but slower (load increased from 7% to 11%). Also note that just calling sdram_disableMallocSdram()
//-> will not disable the override instead will make every call to malloc() return with -1, that could lead to HardFault
//-> when creating objects like Serial, I2C etc.

//Note2: Max number of threads allowed are 14. This can be changed in RTX_Conf_CM.c

//Note3: RTOS Timer callback functions are not executed, check issue https://github.com/mbedmicro/mbed/issues/276

//Note4: Memory is allocated as can been seen here: https://mbed.org/handbook/RTOS-Memory-Model . Each individual
//-> thread's stack is allocated in Heap (as can been seen in mbed-rtos/rtos/Thread.cpp constructor). There is no
//-> collision detection between heap and main stack. Insufficient stack space usually leads to HardFault.

int main (void) {
	DEBUG_PRINT(("Start\r\n"));

	CO2Init(p17, p18);	//p17=TX, p18=RX

    I2C i2c0(p32,p31);	//sda, scl
    I2C i2c1(p9,p10);
    GridEYEInit(&i2c0, &i2c1);

    USBInit();

    HealthInit();

#if DEVELOPMENT
    Thread tStatistics(CpuLoadTask, NULL, osPriorityIdle);
#endif

    Thread tUSB(USBTask);

    Thread tCO2Caller(CO2SchedulerTask);

    Thread tGridEYECaller(GridEYESchedulerTask);

    Thread::wait(osWaitForever);
}
