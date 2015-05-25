/*
 * watcher.hpp
 *
 *  Created on: May 26, 2015
 *      Author: nick
 */


#ifndef WATCHER_HPP_
#define WATCHER_HPP_

#include "USB.hpp"
#include "PeripheralNames.h"
#include "conf.h"
#include "mbed.h"

void WatcherInit();
void WatcherTask(void const *args);




#endif /* WATCHER_HPP_ */
