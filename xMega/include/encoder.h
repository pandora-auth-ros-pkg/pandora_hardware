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

#define NUMBER_OF_BITS 10				/*! \def NUMBER_OF_BITS \brief  Encoder Resolution Definition */
#define ENCODER_INITIAL_VALUE 0x0000
#define SCL_PIN 1
#define SCL_PORT PORTA
#define DOUT_PIN 2
#define DOUT_PORT PORTA

/*!
 * \struct encoder_struct
 * \brief Structure Holding Rotary encoder Info and measurements
 */
typedef struct{
	uint16_t encoder_value_degrees;
	uint16_t encoder_value_encoded;
	uint16_t encoder_value_decoded;
}encoder_struct;

extern void init_encoder(encoder_struct *encoder_);
extern uint16_t read_encoder(void);
extern void get_encoder_values(encoder_struct *encoder_);
extern uint16_t encoder_convert_degrees(uint16_t encoder_value);
extern uint16_t gray_to_binary(uint16_t num);
extern uint16_t get_encoder_degrees(void);

#endif /* ENCODER_H_ */