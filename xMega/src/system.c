/*!
 *	\file system.c
 *	\brief System Configurations and Initializations Source file
 *
 *	Created: 5/7/2014 9:55:23 PM
 *	Author:	Panayiotou Kostantinos
 *	Email:	glagloui@gmail.com
 */ 

#include "system.h"

TWI_Master_t twiMaster;  

void clock_init(void)
{
	__asm__ ("cli");
	CCP = 0xD8;					// ???????????
	OSC.PLLCTRL=0xC8;			// C for selecting external XTAL as input for the PLL, 4 is the PLL factor
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
}

void init_uC_clock(void){
	/* <Swap Main Clock to Internal RC 2MHz(Default)> */
	CLKSYS_Enable(OSC_RC2MEN_bm); 									/* <Enable Internal RC Clock 2MHz> */
	do {} while(CLKSYS_IsReady(OSC_RC2MRDY_bm) == 0);				/* <Wait 2MHz RC Clock Stable> */
	CLKSYS_Main_ClockSource_Select(CLK_SCLKSEL_RC2M_gc);			/* <Select System Clock = Internal RC 2MHz> */
	CLKSYS_Prescalers_Config(CLK_PSADIV_1_gc,CLK_PSBCDIV_1_1_gc);	/* <Prescale Divide by 1 ,Divide B by 1 and C by 1> */
	CLKSYS_Disable(OSC_XOSCEN_bm);									/* <Disable External XTAL Clock> */
	CLKSYS_Disable(OSC_RC32MEN_bm);									/* <Disable Internal RC 32MHz> */
	CLKSYS_Disable(OSC_RC32KEN_bm);									/* <Disable Internal RC 32KHz> */
	CLKSYS_Disable(OSC_PLLEN_bm);									/* <Disable PLL Clock> */
	/* <Now System Clock is Ready = 2MHz> */

	/* <Start Config New Main Clock Source> */
	CLKSYS_XOSC_Config( OSC_FRQRANGE_2TO9_gc,			  			/* <Used Frequency Range 2-9MHz(8MHz)> */
						true,										/* <Used High-quality watch crystals> */
						OSC_XOSCSEL_XTAL_256CLK_gc					/* <Select 8MHz XTAL + Startup 256 Cycle> */
						);		  		
	CLKSYS_Enable(OSC_XOSCEN_bm);						  			/* <Enable External Clock> */
	do {} while (CLKSYS_IsReady(OSC_XOSCRDY_bm) == 0);  			/* <Wait Clock is Stable> */
	CLKSYS_Prescalers_Config(CLK_PSADIV_1_gc,CLK_PSBCDIV_1_1_gc);	/* <Prescale Divide by 1 ,Divide B by 1 and C by 1> */
	CLKSYS_PLL_Config( OSC_PLLSRC_XOSC_gc, 8 );  					/* <External Oscillator(8MHz) x 8 = 64MHz> */
	CLKSYS_Enable( OSC_PLLEN_bm );									/* <Enable PLL> */
	CLKSYS_Prescalers_Config(CLK_PSADIV_1_gc,CLK_PSBCDIV_1_2_gc);	/* <Prescale Divide by 1 ,Divide B by 1 and C by 2> */
	do {} while ( CLKSYS_IsReady(OSC_PLLRDY_bm ) == 0);				/* <Wait PLL ready> */
	CLKSYS_Main_ClockSource_Select(CLK_SCLKSEL_PLL_gc);				/* <Select System Clock = Phase Locked Loop(32MHz)> */
	/* Now System Clock is Ready = 32MHz */
}

/*!
		Brief Description
	Initialize and startup uController Modules:
	Initialize uClock.
	Initialize Watchdog timer.
	Initialize Timer 0 for Event capturing.
	Initialize UART. (PC communication)
	Initialize I2C Interface.
	Initialize two Analog to Digital Signal Inputs. (ADCB)
	Initialize Encoder.
 */
void _startup_system_init(void)
{
	init_uC_clock();											/* <Initialize Fsys clock to use external XTAL and PLL for Fsys=32MHz> */
	WDT_EnableAndSetTimeout(WDT_PER_8CLK_gc);					/* <Initialize WDT module with 8ms Timeout> */
	
	/*============================Initialize TCC0 interrupt every 200us===============================*/
	TC_SetPeriod( &TCC0, 799 );									/* <Set period/TOP value> */
	TC0_ConfigWGM( &TCC0, TC_WGMODE_SS_gc );					/* <Configure the TC for single slope mode> */
	TC0_SetOverflowIntLevel( &TCC0, TC_OVFINTLVL_LO_gc );		/* <Enable overflow interrupt at low level> */
	TC0_ConfigClockSource( &TCC0, TC_CLKSEL_DIV8_gc );			/* <Select clock source> */
	/*================================================================================================*/
	init_uart();												/* <Initialize USART> */
	
	/*================================ Initialize TWI master =========================================*/
	TWI_MasterInit(&twiMaster,
	&TWIC,TWI_MASTER_INTLVL_LO_gc,
	TWI_BAUDSETTING);
	/*================================================================================================*/
	PMIC.CTRL |= PMIC_LOLVLEN_bm;								/* <Enable LO interrupt level> */
	
	/*================================ Enable ADCB CH0 & CH1 =========================================*/
	PORTB.OUT=0x00;
	PORTB.DIR = 0x00;											/* <Configure PORTB as input> */
	ADCB.CTRLA |= 0x1;											/* <Enable ADCB> */
	ADCB.CTRLB = ADC_RESOLUTION_12BIT_gc; 						/* <Set 12 bit conversion> */
	ADCB.CTRLB |= ADC_FREERUN_bm;								/* <Enable Freerunning mode> */
	ADCB.REFCTRL = ADC_REFSEL_AREFB_gc;							/* <External reference from AREF pin on PORT B> */
	ADCB.PRESCALER = ADC_PRESCALER_DIV512_gc; 					/* <Peripheral clk/512 (32MHz/512=62.5kHz)> */
	ADCB.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc; 			/* <Single Ended> */
	ADCB.CH1.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc; 			/* <Single Ended> */
	ADCB.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc; 					/* <ADC_CHANNEL0 Converts Analog Input PORTB:1 (Motor Battery)> */
	ADCB.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc; 					/* <ADC_CHANNEL1 Converts Analog Input PORTB:3 (PSU Battery)> */
	ADCB.EVCTRL = ADC_SWEEP_01_gc;								/* <SWEEP mode 01 selected> */
	/*================================================================================================*/
	
	init_encoder(&_encoder);
}