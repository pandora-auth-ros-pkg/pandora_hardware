/*
 * watcher.cpp
 *
 *  Created on: May 26, 2015
 *      Author: nick
 */

#include "watcher.hpp"

#define WATCHER_TIME 500 // ms
#define USB_COMMUN_TIMEOUT  2000000 // us

static DigitalOut greenLed(LED2);

static bool usbOK;


void WatcherInit(){
    greenLed = 0;
    usbOK = false;
}
void WatcherTask(void const *args) {
    while(true){
        Thread::wait(WATCHER_TIME);
        if (usbActive()){
            if (us_ticker_read() - getLastReceivedTime() > USB_COMMUN_TIMEOUT){
                greenLed = 1;
                usbOK = false;
                #if !DEVELOPMENT
                    NVIC_SystemReset();  //reset ARM if USB communication is lost.
                #endif
            }
            else{
                usbOK = true;
            }
        }
    }

}

