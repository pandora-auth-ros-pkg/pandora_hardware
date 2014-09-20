/*
 * \file xMega.c
 * \brief xMega main Source file.
 *
 * Created:	5/9/2014 6:17:02 PM
 *  Author:	Panayiotou Kostantinos
 *  Email:	klpanagi@gmail.com
 */

/***********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2014, P.A.N.D.O.R.A. Team.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the P.A.N.D.O.R.A. Team nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * *********************************************************************/


#include "xMega.h"

static void tcc0_ovf_interrupt_callback(void);

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
		
	for(i=0;i<I2CSENSNUM;i++)
	{
		PCTXBuffer[SensorModule[i].StartIndexInPCTXBuffer] = nybble_chars[(i & 0xf0)>>4];
		PCTXBuffer[SensorModule[i].StartIndexInPCTXBuffer+1] = nybble_chars[(i & 0x0f)];
	}
	PCTXBuffer[0] = NPFF;
	PCTXBuffer[1] = SOH;
	
	sei();
	i= 0;
	
	pandora_fsm();
	
	return 0;
}

/************************************************************************/
/* Interrupt Handlers for I2C and TimerCounter0                          */
/************************************************************************/
ISR(TWIC_TWIM_vect)
{
	TWI_MasterInterruptHandler(&twiMaster);
}

ISR(TCC0_OVF_vect)
{
	tcc0_ovf_interrupt_callback();
}

/*!	\fn static void tcc0_ovf_interrupt_callback(void)
 *	\brief Timer (200us) Interrupt callback Service routine.
 */
static void tcc0_ovf_interrupt_callback(void)
{
	TCTimingFlags |=Tick200us;

	NextSensor = I2CSensorFSMCore(NextSensor);

	if(!(ticks % 2)) {TCTimingFlags |=Tick400us;}
	if(!(ticks % 4)) {}

	if(!(ticks % 5))
	{
		TCTimingFlags |=Tick1ms;
		SensorWaitTimeHandler();							/* <Wait Time HANDLER OF SENSOR FSM (Timeout-Idle Timer-Revive Timer etc)> */
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
	
	/*<Service data transmission. Sending 1 byte every 200us> */
	data_transmitService();	
	++ticks;
	ticks %= 50000;
}

