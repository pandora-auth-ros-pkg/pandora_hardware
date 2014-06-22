/*!
 *	\file encoder.c
 *	\brief Rotary Encoder Services Source file.
 *
 *	Created: 4/6/2014 2:07:39 AM
 *	Author: Panayiotou Kostantinos
 *	Email: glagloui@gmail.com
 */ 
#include "encoder.h"


void init_encoder(encoder_struct *encoder_)
{
	ENCODER_PORT.DIR |= (1<<SCL_PIN);								/* <Set PortB_Pin1 as output -> SCL> */
	ENCODER_PORT.OUT |= (1<<SCL_PIN);								/* <Set PortB_PIN1> */
	ENCODER_PORT.DIR |= (0<<DO_PIN);									/* <Set PortB_Pin2 as input -> Data> */
	ENCODER_PORT.DIR |= (1<<CS_PIN);									/* <Set PortB_Pin1 as output -> CS> */
	ENCODER_PORT.OUT |= (1<<CS_PIN);									/* <Set PortB_PIN3> */
	
	/* <Initialize encoder values in encoder struct> */
	encoder_ -> value = 0;
	encoder_ -> value_degrees = 0;
	
}


uint16_t read_encoder(void){
	uint16_t read_value = 0;
	
	ENCODER_PORT.OUT &= ~(1<<CS_PIN);
	_delay_us(2);
	
	for(int i=0;i<(NUMBER_OF_BITS);i++){
		ENCODER_PORT.OUT &= ~(1<<SCL_PIN);;	/* <First falling edge according to SSI protocol> */
		_delay_us(1);
		ENCODER_PORT.OUT |= (1<<SCL_PIN);
		_delay_us(1);	
		read_value = (read_value<<1) | (ENCODER_PORT.IN & (1<<DO_PIN)) ;
		
	}
	ENCODER_PORT.OUT |= (1<<CS_PIN);
	return read_value;
}


uint16_t encoder_convert_degrees(uint16_t value){
	uint16_t degrees;
	degrees = value * 360 /1024;
	return (degrees);
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


void get_encoder_values(encoder_struct *_encoder)
{
	uint16_t degrees;
	uint16_t value;
	cli();/* <Disable CPU Interrupts to avoid interrupt while triggering encoder> */ 
	value = read_encoder();
	sei();
	degrees = encoder_convert_degrees(value);
	_encoder -> value = value;
	_encoder -> value_degrees = degrees;
	
}


uint16_t get_encoder_degrees(void)
{
	uint16_t degrees;
	uint16_t value;

	cli();/* <Disable CPU Interrupts to avoid interrupt while triggering encoder> */ 
	value = read_encoder();
	sei();
	degrees = encoder_convert_degrees(value);
	
	return (degrees);
}
