/*
 * PandoraFSM.c
 *
 * Created: 4/12/2012 5:40:04 μμ
 *  Author: dikan
 */ 

#include "xMega.h"
#include "FSMs.h"
#include "sensorStruct.h"
#include "sensorConf.h"

extern sensor_struct SensorModule[SENSNUM];
extern volatile uint8_t CurrentGroup;
volatile uint16_t	ticks; //-> μηπως ο σωστος ορισμός ειναι static μεσα στο interupt?

static void tcc0_ovf_interrupt_callback(void);
void _startup_system_init(void);
void _usartService(void);

int main(void) 
{
	uint8_t i;
	NextSensor=0;
	PCTXPointer=0;
	
	_startup_system_init();
	/* Sensor Structure Init*/
	PCTXBufferLen=CreateSensorStructs();
	/* Select Sensors that belong in Group1 and clear their 'USED' Flag*/
	CurrentGroup=1;
	CreateArraySameGroupIDs();
	
	PCTXBuffer = malloc(PCTXBufferLen * sizeof(uint8_t));
	for (uint16_t j=0; j<PCTXBufferLen;j++)	{PCTXBuffer[j] ='0';}
		
	for(i=0;i<SENSNUM;i++)
	{
		PCTXBuffer[SensorModule[i].StartIndexInPCTXBuffer] = nybble_chars[(i & 0xf0)>>4];
		PCTXBuffer[SensorModule[i].StartIndexInPCTXBuffer+1] = nybble_chars[(i & 0x0f)];
	}
	PCTXBuffer[0] = NPFF;
	PCTXBuffer[1] = SOH;
	
	sei();
	i= 0;
	
	while(1)
	{
		if(TCTimingFlags & Tick200us)
		{
				TCTimingFlags&=~Tick200us; //every 200us
					//	i= SensorFSMCore(i);					//FSM CORE OF SENSOR SERVICE 15usec MAX 
		}
		if(TCTimingFlags & Tick400us)
		{
			TCTimingFlags&=~Tick400us; //every 400us
			//PUT HERE CODE TO BE SERVICED EVERY 400us
		}
		if(TCTimingFlags & Tick1ms)
		{
			TCTimingFlags&=~Tick1ms; //every 1ms
			//PUT HERE CODE TO BE SERVICED EVERY 1ms
			//SensorWaitTimeHandler();							//Wait Time HANDLER OF SENSOR FSM (Timeout-Idle Timer-Revive Timer etc)
			//TWIWatchdog();
		}
		if(TCTimingFlags & Tick5ms)
		{
			TCTimingFlags&=~Tick5ms; //every 5ms
			//PUT HERE CODE TO BE SERVICED EVERY 5ms
			if(!(PCTXFlags&PCTX_Busy)&&!(PCTXFlags&PCTX_DebugMode)&&!(PCTXFlags&PCTX_DebugMode2))	{CheckSensorBuffersForNewData();}
		}
		if(TCTimingFlags & Tick10ms)
		{
			TCTimingFlags&=~Tick10ms;									
			//PUT HERE CODE TO BE SERVICED EVERY 10ms
		}
		if(TCTimingFlags & Tick50ms)
		{
				TCTimingFlags&=~Tick50ms; //every 50ms
				//PUT HERE CODE TO BE SERVICED EVERY 50ms
		}
		if(TCTimingFlags & Tick100ms)
		{
			TCTimingFlags&=~Tick100ms; //every 100ms
			//PUT HERE CODE TO BE SERVICED EVERY 100ms
			//timestamp
			batteryVoltage_MOTOR = ADCB.CH0RES;						// read 12 bit value and save as data
			batteryVoltage_PSU = ADCB.CH1RES;						// read 12 bit value and save as data
			// end of edit
		}
		if(TCTimingFlags & Tick200ms)
		{
			TCTimingFlags&=~Tick200ms; //every 200ms
			//PUT HERE CODE TO BE SERVICED EVERY 200ms
			if(!(PCTXFlags&PCTX_Busy)&&!(PCTXFlags&PCTX_DebugMode)&&!(PCTXFlags&PCTX_DebugMode2))
			{
				PCTXPointer=0;
				PCTXFlags|=PCTX_Busy;
				usart_putc(PCTXBuffer[0]);
				// edit
				// send ADC data here?????
				// end of edit
			}
		}
	}
	return 0;
}


//-----------------------------------------------------------------------------------------------------------
//-------------------------INTERRUPT HANDLERS----------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
ISR(TWIC_TWIM_vect)
{
	TWI_MasterInterruptHandler(&twiMaster);
}

ISR(TCC0_OVF_vect)
{
	tcc0_ovf_interrupt_callback();
}

//-----------------------------------------------------------------------------------------------------------
static void tcc0_ovf_interrupt_callback(void)
{
	TCTimingFlags |=Tick200us;

	NextSensor = SensorFSMCore(NextSensor);

	if(!(ticks % 2)) {TCTimingFlags |=Tick400us;}
	if(!(ticks % 4)) {}

	if(!(ticks % 5))
	{
		TCTimingFlags |=Tick1ms;
		SensorWaitTimeHandler();														//Wait Time HANDLER OF SENSOR FSM (Timeout-Idle Timer-Revive Timer etc)
		GroupHandler();
		TWIWatchdog();
		if(PCTXWaitAckNakTimer) {PCTXWaitAckNakTimer--;}
	}
	if(!(ticks % 25))
	{
		TCTimingFlags |=Tick5ms;
		WDT_Reset();
	}
	if(!(ticks % 50)) {TCTimingFlags |=Tick10ms;}
	if(!(ticks % 250)) {TCTimingFlags |=Tick50ms;}
	if(!(ticks % 500)) {TCTimingFlags |=Tick100ms;}
	if(!(ticks % 1000)) {TCTimingFlags |=Tick200ms;}

	_usartService();		//USART SERVICE. LOAD NEXT BYTE TO BE TRANSFERED TO PC. Clear PCTX_Busy when all data have been transfered
	++ticks;
	ticks %= 50000;
}

void _startup_system_init(void)
{
	//clock_init();												//true
	init_uC_clock();											//Initialize Fsys clock to use external XTAL and PLL for Fsys=32MHz
	
	WDT_EnableAndSetTimeout(WDT_PER_8CLK_gc);					//Initialize WDT module with 8ms Timeout
	
	/*============================Initialize TCC0 interrupt every 200us===============================*/
	TC_SetPeriod( &TCC0, 799 );									//Set period/TOP value.
	TC0_ConfigWGM( &TCC0, TC_WGMODE_SS_gc );					//Configure the TC for single slope mode.
	TC0_SetOverflowIntLevel( &TCC0, TC_OVFINTLVL_LO_gc );		//Enable overflow interrupt at low level
	TC0_ConfigClockSource( &TCC0, TC_CLKSEL_DIV8_gc );			//Select clock source.
	/*================================================================================================*/
	init_uart();												//Initialize USART
	
	/*================================ Initialize TWI master =========================================*/
	TWI_MasterInit(&twiMaster,
				&TWIC,TWI_MASTER_INTLVL_LO_gc,
				TWI_BAUDSETTING);
	/*================================================================================================*/
	
	PMIC.CTRL |= PMIC_LOLVLEN_bm;								//Enable LO interrupt level
	
	/*================================ Enable ADCB CH0 & CH1 =========================================*/
	PORTB.OUT=0x00;
	PORTB.DIR = 0x00;									// configure PORTB as input
	ADCB.CTRLA |= 0x1;									// enable adc
	ADCB.CTRLB = ADC_RESOLUTION_12BIT_gc; 				// 12 bit conversion
	ADCB.CTRLB |= ADC_FREERUN_bm;						// Freerunning mode enabled
	ADCB.REFCTRL = ADC_REFSEL_AREFB_gc;					// External reference from AREF pin on PORT B
	ADCB.PRESCALER = ADC_PRESCALER_DIV512_gc; 			// peripheral clk/512 (32MHz/512=62.5kHz)
	ADCB.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc; 	// single ended
	ADCB.CH1.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc; 	// single ended
	ADCB.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc; 			// ADC_CHANNEL0 Converts Analog Input PORTB:1 (Motor Battery)
	ADCB.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc; 			// ADC_CHANNEL1 Converts Analog Input PORTB:3 (PSU Battery)
	ADCB.EVCTRL = ADC_SWEEP_01_gc;						// SWEEP mode 01 selected
	/*================================================================================================*/
}

void _usartService(void)
{
	if( (PCTXFlags & PCTX_Busy) && !(PCTXFlags & PCTX_WaitAckNaK) )
	{
		PCTXPointer++;
		if(PCTXPointer<PCTXBufferLen && (USARTD0.STATUS & USART_DREIF_bm))
		{
			//USARTD0.DATA = PCTXBuffer[PCTXPointer];
			usart_putc(PCTXBuffer[PCTXPointer]);
		}
		PCTXPointer %= PCTXBufferLen;								//keep PCTXPointer in range [0,PCTXBufferLen]
		if(!PCTXPointer)
		{
			PCTXFlags &= ~(PCTX_Busy);
			if( !(PCTXFlags&PCTX_DebugMode) )
			{
				PCTXFlags |= (PCTX_WaitAckNaK|PCTX_Busy);			// Set WaitAckNak and Busy Flags for UART
				PCTXWaitAckNakTimer = WaitAckNakTimeOut;
			}
			USARTD0.STATUS |= USART_DREIF_bm;
			USARTD0.STATUS |= USART_RXCIF_bm;						//CLEAR RECEIVED DATA FLAG IN USART
		}
	}
	if( (PCTXFlags&PCTX_Busy)&&(PCTXFlags&PCTX_WaitAckNaK) )
	{
		if(PCTXWaitAckNakTimer)
		{
			if(USARTD0.STATUS & USART_RXCIF_bm)
			{
				uint8_t	TempReceive=USARTD0.DATA;
				USARTD0.STATUS |=USART_RXCIF_bm;
				if (TempReceive == UACK)
				{
					PCTXFlags &= ~(PCTX_Busy|PCTX_WaitAckNaK);		//Last packet transmission was successful. Close the service
				}
				else
				{
					PCTXFlags &= ~PCTX_WaitAckNaK;					//Repeat last transmission!!
					usart_putc(PCTXBuffer[0]);
				}
			}
		}
		else
		{
			PCTXFlags &= ~(PCTX_Busy|PCTX_WaitAckNaK);				//Last packet transmission was without response from PC. Close the service and try again with new data
		}
	}
}

