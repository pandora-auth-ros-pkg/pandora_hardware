
#ifndef CO2_HPP_
#define CO2_HPP_

#include "mbed.h"
#include "rtos.h"
#include "dsp.h"
#include "health.hpp"

#include "LibMods/Serial2.h"

/** CO2 Sensor Protocol Instructions */
#define RD   0x13        ///Read
#define DLE 0x10            ///Data Link Escape
#define Var_ID 0x06         ///Variable ID live data simple
#define NAK 0x19            ///Negative Acknowledge
#define DAT 0x1A            ///Single Data Frame
#define EOFF 0x1F          ///End of Frame (Can't use EOF because is already used by the system)
#define Checksum_hi 0x00    ///Checksum High byte
#define Checksum_lo 0x58    ///Checksum low byte

void CO2valueSet(float value);

void CO2Task(void const *args) ;

void RX_isr() ;

void CO2Init(PinName tx, PinName rx) ;

void CO2Trigger() ;

#endif /* CO2_HPP_ */
