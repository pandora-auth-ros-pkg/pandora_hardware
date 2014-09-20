/*!
 *	\file encoder.c
 *	\brief Rotary Encoder Services Source file.
 *
 *	Created: 4/6/2014 2:07:39 AM
 *	Author: Panayiotou Kostantinos
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



#include "encoder.h"


void init_encoder(encoder_struct *encoder_)
{
	ENCODER_PORT.DIR |= (1<<SCL_PIN);								/* <Set PortB_Pin1 as output -> SCL> */
	ENCODER_PORT.OUT |= (1<<SCL_PIN);								/* <Set PortB_PIN1> */
	ENCODER_PORT.DIR |= (0<<DO_PIN);								/* <Set PortB_Pin2 as input -> Data> */
	ENCODER_PORT.DIR |= (1<<CS_PIN);								/* <Set PortB_Pin1 as output -> CS> */
	ENCODER_PORT.OUT |= (1<<CS_PIN);								/* <Set PortB_PIN3> */
	
	/* <Initialize encoder values in encoder struct> */
	encoder_ -> rotation_value = 0;
	encoder_ -> rotation_value_degrees = 0;
	encoder_ -> rotation_value_read = 0; 
	
}


uint16_t read_encoder(void){
	uint16_t read_value = 0;
	
	ENCODER_PORT.OUT &= ~(1<<CS_PIN);
	_delay_us(2);
	
	for(int i=0;i<(NUMBER_OF_BITS-1);i++){
		ENCODER_PORT.OUT &= ~(1<<SCL_PIN);;	/* <First falling edge according to SSI protocol> */
		_delay_us(1);
		ENCODER_PORT.OUT |= (1<<SCL_PIN);
		_delay_us(1);	
		read_value = (read_value<<1) | (ENCODER_PORT.IN & (1<<DO_PIN)) ;
		
	}
	ENCODER_PORT.OUT |= (1<<CS_PIN);
	return read_value;
}


float encoder_convert_degrees(uint16_t _value){
	float degrees;
	degrees =  ((float)_value * 360)/1024 ;
	return degrees;
}


uint16_t gray_to_binary(uint16_t num)
{
	uint16_t mask;
	for (mask = num >> 1; mask != 0; mask = mask >> 1)
	{
		num = num ^ mask;
	}
	return num;
}


void get_encoder_values(encoder_struct *encoder_)
{
	float value_degrees;
	uint16_t value_read;
	uint16_t value_proc;
	cli();																	/* <Clear Global Interrupts for SCL generation> */
	value_read = read_encoder();
	//encoder_value_encoded = read_encoder();
	sei();																	/* <Enable Global Interrupts> */
	value_degrees = encoder_convert_degrees(value_read);
	value_proc = proc_encoder_value(value_degrees);
	
	encoder_ -> rotation_value_read = value_read;
	encoder_ -> rotation_value_degrees = value_degrees;
	encoder_ -> rotation_value = value_proc;
	
}


float get_encoder_degrees(void)
{
	float encoder_value_degrees;
	uint16_t encoder_value_encoded;
	uint16_t encoder_value_decoded;
	
	cli();																	/* <Clear Global Interrupts for SCL generation> */
	encoder_value_encoded = read_encoder();
	sei();																	/* <Enable Global Interrupts> */
	encoder_value_decoded = gray_to_binary(encoder_value_encoded);
	encoder_value_degrees = encoder_convert_degrees(encoder_value_decoded);
	
	return (encoder_value_degrees);
}

uint16_t proc_encoder_value(float rotDegrees)
{
	float tempvalue_degrees = rotDegrees - ENCODER_OFFSET_DEGREES ;
	int tempvalue = rotDegrees * 1024 / 360 - ENCODER_OFFSET;
	
	if ( (tempvalue_degrees >= 0) && (tempvalue_degrees <= 360) )	
		return (tempvalue);
	else
	{
		if 	(tempvalue_degrees < 0)		{tempvalue = 1024 - (-tempvalue);}
		if 	(tempvalue_degrees > 360)	{tempvalue = tempvalue - 1024;}
	}
	
	return	( tempvalue);
}
