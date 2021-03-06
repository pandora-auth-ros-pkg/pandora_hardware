/*!
 *	\file encoder.h
 *	\brief Rotary Encoder Services Header file.
 *
 * 	Created: 4/6/2014 2:09:02 AM
 *	Author:	Panayiotou Kostantinos
 * 	Email: klpanagi@gmail.com
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



#ifndef ENCODER_H_
#define ENCODER_H_

#define F_CPU 32000000UL	

#include <util/delay.h>
#include <avr/io.h>
#include "avr_compiler.h"

/*!	\def NUMBER_OF_BITS
 *	\brief  Encoder Resolution Definition 
 */
#define NUMBER_OF_BITS 10

/*!	\def ENCODER_PORT
 *	\brief  Rotary Encoder Communication PORT
 */
#define ENCODER_PORT PORTE	

/*!	\def SCL
 *	\brief Clock Line Pin for encoder communications. 
 */
#define SCL_PIN 1
#define DO_PIN 2
#define CS_PIN 3

#define ENCODER_OFFSET	448	/*<157.5 degrees>*/
#define ENCODER_OFFSET_DEGREES 157.5

/*!
 * \struct encoder_struct
 * \brief Structure Holding Rotary encoder Info and measurements
 */
typedef struct{
	float rotation_value_degrees;
	uint16_t rotation_value_read;
	uint16_t rotation_value;
	
}encoder_struct;

/*!	\fn void init_encoder(encoder_struct *encoder_)
 *	\brief Initialize Port / Pins used for encoder communications.
 *	\param encoder_struct *encoder_ Encoder struct Object holding encoder values
 */
extern void init_encoder(encoder_struct *encoder_);

/*!	\fn uint16_t read_encoder(void)
 *	\brief Triggers communication with encoder (SSI Protocol) and returns encoder rotary measurement value.
 */
extern uint16_t read_encoder(void);

/*!	\fn void get_encoder_values(encoder_struct *encoder_)
 *	\brief Read encoder value and stores encoded, decoded, and degree values 
 *	\param *encoder_ Encoder Struct holding rotary measurements.
 */
extern void get_encoder_values(encoder_struct *encoder_);

/*!	\fn uint16_t encoder_convert_degrees(uint16_t encoder_value)
 *	\brief Converts encoder value to degrees. Returns the value in degrees.
 *	\param encoder_value Encoder value.
 */
extern float encoder_convert_degrees(uint16_t value);

/*!	\fn uint16_t gray_to_binary(uint16_t num)
 *	\brief Converts a number from Gray code to binary code value.
 *	\param num Gray code number.
 */
extern uint16_t gray_to_binary(uint16_t num);

/*!	\fn uint16_t get_encoder_degrees(void)
 *	\brief Triggers Encoder, read value and return measurement in degrees.
 */
extern float get_encoder_degrees(void);

extern uint16_t proc_encoder_value(float rotDegrees);
#endif /* ENCODER_H_ */
