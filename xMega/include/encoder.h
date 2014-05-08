/*
 * encoder.h
 *
 * Created: 4/6/2014 2:09:02 AM
 *  Author: Kostantinos
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_
#define F_CPU 32000000UL

#include <util/delay.h>
#include <avr/io.h>

#define NUMBER_OF_BITS 10
#define ENCODER_INITIAL_VALUE 0x0000
#define SCL_PIN 1
#define SCL_PORT PORTA
#define DOUT_PIN 2
#define DOUT_PORT PORTA

extern void init_encoder(void);
extern uint32_t read_encoder(void);
extern uint32_t pinToggleReadSSI(void);
extern uint32_t _encoder_convert_degrees(uint32_t encoder_value);
extern uint32_t gray_to_binary(uint32_t num);

#endif /* ENCODER_H_ */