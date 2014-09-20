/*!
 *	\file battery.c
 *	\brief System Sensors Services Header file.
 *
 *	Created: 5/9/2014 11:46:04 PM
 *	Author: Panayiotou Konstantinos
 *	Email: klpanagi@gmail.com
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


#include "battery.h"

void init_ADC(void)
{
	PORTA.DIR = 0;
	/*  <Move stored calibration values stored in PRGMEM to ADC A.> */
	ADC_CalibrationValues_Load(&ADCA);
	/* Set up ADC A to have signed conversion mode and 12 bit resolution. */
	ADC_ConvMode_and_Resolution_Config(&ADCA, ADC_ConvMode_Unsigned, ADC_RESOLUTION_12BIT_gc);
	/* Set sample rate. Peripheral clk/512 (32MHz/512=62.5kHz) */
	ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV512_gc);
	/* Set reference voltage on ADC A to be AREF.*/
	ADC_Reference_Config(&ADCA, ADC_REFSEL_AREFA_gc);
	/* Setup channel 0, 1 with different inputs. */
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH0,
									 ADC_CH_INPUTMODE_SINGLEENDED_gc,
									 ADC_DRIVER_CH_GAIN_NONE);

	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH1,
									 ADC_CH_INPUTMODE_SINGLEENDED_gc,
									 ADC_DRIVER_CH_GAIN_NONE);

	/* Set input to the channels in ADC A to be PIN1, 3. */
	ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_MUXPOS_PIN1_gc, 0);
	ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_MUXPOS_PIN3_gc, 0);
	/* Setup sweep of all 2 virtual channels.*/
	ADC_SweepChannels_Config(&ADCA, ADC_SWEEP_01_gc);
	/* Enable ADC A .*/
	ADC_Enable(&ADCA);

	/* Wait until common mode voltage is stable. Default clk is 2MHz and
	 * therefore below the maximum frequency to use this function. */
	ADC_Wait_32MHz(&ADCA);

	/* Enable free running mode. */
	ADC_FreeRunning_Enable(&ADCA);
}
