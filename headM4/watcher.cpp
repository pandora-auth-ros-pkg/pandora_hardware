/*
 * watcher.cpp
 *
 *  Created on: May 26, 2015
 *      Author: nick
 */

#include "watcher.hpp"

#define WATCHER_TIME 500 // ms
#define USB_COMMUN_TIMEOUT  3000000 // us

static DigitalOut greenLed(LED2);
static DigitalOut blueLed(LED4);
static bool usbOK;


void WatcherInit(){
    greenLed = 0;
    blueLed = 0;
    usbOK = false;
}
void WatcherTask(void const *args) {
    while(true){
        Thread::wait(WATCHER_TIME);
        if (usbActive()){
            blueLed = 1;
            if (us_ticker_read() - getLastReceivedTime() > USB_COMMUN_TIMEOUT){
                greenLed = !greenLed;
                blueLed = 0;
                usbOK = false;
            }
            else{
                usbOK = true;
            }
        }
    }

}

