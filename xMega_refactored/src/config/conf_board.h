/**
 * \file
 *
 * \brief User board configuration template
 *
 */


#ifndef CONF_BOARD_H
#define CONF_BOARD_H
	#define CONF_BOARD_ENABLE_USARTD0
	// External oscillator settings.
	// Uncomment and set correct values if external oscillator is used.

	// External oscillator frequency
	#define BOARD_XOSC_HZ          8000000UL

	// External oscillator type.
	//!< External clock signal
	//#define BOARD_XOSC_TYPE        XOSC_TYPE_EXTERNAL
	//!< 32.768 kHz resonator on TOSC
	//#define BOARD_XOSC_TYPE        XOSC_TYPE_32KHZ
	//!< 0.4 to 16 MHz resonator on XTALS
	#define BOARD_XOSC_TYPE        XOSC_TYPE_XTAL

	// External oscillator startup time
	#define BOARD_XOSC_STARTUP_US  500
#endif // CONF_BOARD_H

