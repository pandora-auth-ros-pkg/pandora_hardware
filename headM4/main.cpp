#include "mbed.h"
#include "rtos.h"
//#include "gpdma.h"
//#include "dsp.h"
//#include "USBSerial2.h"
//#include "Serial2.h"
//#include "cmsis_os.h"

DigitalOut led(LED1);

void led_thread(void const *argument) {
    while (true) {
        // Signal flags that are reported as event are automatically cleared.
        Thread::signal_wait(0x1);
        led = !led;
    }
}

void TriggerTask(void const *args);

int main (void) {
    Thread thread(TriggerTask);
    
    while (true) {
        Thread::wait(1000);
        thread.signal_set(0x1);
    }
}

void TriggerTask(void const *args) {
//	thread.signal_set(0x1);
	Thread::wait(100);
}
