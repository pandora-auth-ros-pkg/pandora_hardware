/*
 * system.c
 *
 * Created: 3/22/2014 7:27:27 PM
 *  Author: klpanagi
 */ 

#include "include/system.h"

void clock_init(void)
{
	__asm__ ("cli");
	CCP = 0xD8;					// ???????????
	OSC.PLLCTRL=0xC4;			// C for selecting external XTAL as input for the PLL, 4 is the PLL factor

	// remove these lines if PC7 is used
	PORTCFG.CLKEVOUT=0x01;		//PC7 will clock out the CPU clock
	PORTC.DIRSET=0x80;
	// *********************************

	OSC.XOSCCTRL=0xCB;			//Selects the ext XTAL range 9..16MHz "C" and 16K clocks start up time "B"
	OSC.CTRL=0x19;				//turns on PLL module (b4) , XOSC module (b3), while keeping RC2M module (b0) enabled
	//CLK.PSCTRL = 0x04;
	__asm__ ("push r16");		//	CCP = 0xD8;             // IO Register Protection
	__asm__ ("ldi r16,0xD8");
	__asm__ ("sts 0x34,r16");
	__asm__ ("ldi r16 , 0x01");	//	CLK.PSCTRL = 0x01;		// Prescaled A divide by 1 (b6..b2=00000). (RANGE with predifined values b2..b6:1..512), Prescalers B-C divide by 2 1_2 (b0=1) see page 85
	__asm__ ("sts 0x41, r16");
	__asm__ ("pop r16");
	char x = OSC.STATUS;
	while(!(x & 0x18))			//wait until PLL and XOSC are stable
	{
		x = OSC.STATUS;
	}
	//CLK.CTRL   = 0x04;		// sets external clock
	__asm__ ("push r16");		//	CCP = 0xD8;             // IO Register Protection
	__asm__ ("ldi r16,0xD8");
	__asm__ ("sts 0x34,r16");
	__asm__ ("ldi r16 , 0x04");	//CLK.CTRL   = 0x04;      // sets external clock. (0:RC2Mhz, 1:RC32MHz, 2:RC32KHz, 3:XOSC, 4:PLL)
	__asm__ ("sts 0x40, r16");
	__asm__ ("pop r16");
	
	
	//timer0 interrupt init
	
}