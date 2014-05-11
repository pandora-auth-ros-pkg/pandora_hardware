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
	SCL_PORT.DIR = (1<<SCL_PIN);								/*! <Set PortB_Pin0 as output -> SCL> */
	SCL_PORT.OUT = (1<<SCL_PIN);								/*! <Set PortC_PIN0> */
	DOUT_PORT.DIR = (0<<DOUT_PIN);
	
	/*! <Initialize encoder values in encoder struct> */
	encoder_ -> encoder_value_decoded = 0;
	encoder_ -> encoder_value_encoded = 0;
	encoder_ -> encoder_value_degrees = 0;
	
}

uint16_t read_encoder(void){
	uint16_t read_value = 0;
	
	SCL_PORT.OUT &= ~(1<<SCL_PIN);	// First falling edge according to SSI protocol
	_delay_us(1);
	
	for(int i=0;i<NUMBER_OF_BITS;i++){
		read_value = (read_value<<1);
		SCL_PORT.OUT |= (1<< SCL_PIN);		// Rising edge
		_delay_us(1);
		SCL_PORT.OUT &= ~(1<<SCL_PIN);
		if(DOUT_PORT.IN & (1<<DOUT_PIN))	//Maybe bit in the first falling edge is wrong according to Figure 1a in datasheet
		{
			//bit is set, set LSB of resultPIND 3 16
			read_value = read_value | 0x01;
		}
		_delay_us(1);
	}
	SCL_PORT.OUT |= (1<< SCL_PIN);		// Rising edge
	
	return read_value;
}

uint16_t encoder_convert_degrees(uint16_t encoder_value){
	uint16_t encoder_degrees;
	encoder_degrees = encoder_value * 360 /1024;
	return (encoder_degrees);
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

/*
	Read encoder value and stores encoded, decoded, and degree values 
*/
void get_encoder_values(encoder_struct *encoder_)
{
	uint16_t encoder_value_degrees;
	uint16_t encoder_value_encoded;
	uint16_t encoder_value_decoded;
	
	cli();																	/*! <Clear Global Interrupts for SCL generation> */
	encoder_value_encoded = read_encoder();
	sei();																	/*! <Enable Global Interrupts> */
	encoder_value_decoded = gray_to_binary(encoder_value_encoded);
	encoder_value_degrees = encoder_convert_degrees(encoder_value_decoded);
	
	encoder_ -> encoder_value_encoded = encoder_value_encoded;
	encoder_ -> encoder_value_decoded = encoder_value_decoded;
	encoder_ -> encoder_value_degrees = encoder_value_degrees;
	
}

uint16_t get_encoder_degrees(void)
{
	uint16_t encoder_value_degrees;
	uint16_t encoder_value_encoded;
	uint16_t encoder_value_decoded;
	
	cli();																	/*! <Clear Global Interrupts for SCL generation> */
	encoder_value_encoded = read_encoder();
	sei();																	/*! <Enable Global Interrupts> */
	encoder_value_decoded = gray_to_binary(encoder_value_encoded);
	encoder_value_degrees = encoder_convert_degrees(encoder_value_decoded);
	
	return (encoder_value_degrees);
}