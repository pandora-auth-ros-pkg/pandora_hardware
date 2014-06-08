/*!
 *	\file encoder.h
 *	\brief Rotary Encoder Services Header file.
 *
 * Created: 4/6/2014 2:09:02 AM
 *  Author:	Panayiotou Kostantinos
 * Email: glagloui@gmail.com
 */ 


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

/*!
 * \struct encoder_struct
 * \brief Structure Holding Rotary encoder Info and measurements
 */
typedef struct{
	uint16_t value_degrees;
	uint16_t value;
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
extern uint16_t encoder_convert_degrees(uint16_t encoder_value);

/*!	\fn uint16_t gray_to_binary(uint16_t num)
 *	\brief Converts a number from Gray code to binary code value.
 *	\param num Gray code number.
 */
extern uint16_t gray_to_binary(uint16_t num);

/*!	\fn uint16_t get_encoder_degrees(void)
 *	\brief Triggers Encoder, read value and return measurement in degrees.
 */
extern uint16_t get_encoder_degrees(void);

#endif /* ENCODER_H_ */