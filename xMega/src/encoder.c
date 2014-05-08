/*
 * encoder.c
 *
 * Created: 4/6/2014 2:07:39 AM
 *  Author: Kostantinos
 */ 
#include "encoder.h"

void init_encoder(void)
{
	//DDROUT = 0xff;//set as test outputs
	//DDRSEND = (1<<SEND_DD_PIN);	//Port B 0 as output  -> SCL
	SCL_PORT.DIR = (1<<SCL_PIN); // Set PortB_Pin0 as output -> SCL
	//PORTSEND = (1<<SEND_PIN);	//Set Port B as 1
	SCL_PORT.OUT = (1<<SCL_PIN); //Set PortC_PIN0
	//DDRIN = (0 << PININ_DD) | (0 << PIN_ENABLE_DD); //Port D 4,2 as inputs
	DOUT_PORT.DIR = (0<<DOUT_PIN);
	//PININ = (1 << PIN_ENABLE_NUMBER);	//DOUT, maybe should be PORTIN for pullup
}

uint32_t read_encoder(void){
	uint32_t read_value = 0;
	
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
	int debug=0;
	return read_value;
}

uint32_t pinToggleReadSSI(void)//sending pulse, 48.3 average duty cycle
{
uint8_t bit_count;
uint32_t result = ENCODER_INITIAL_VALUE;

	for(bit_count=0;bit_count<NUMBER_OF_BITS;bit_count++)
	{
		//falling edge on clock port
		SCL_PORT.OUT &= ~(1<<SCL_PIN);

		//left-shift the current result
		result = (result<<1);

		//delay for pulse symmetry
		_delay_us(1);

		//rising edge on clock port, data changes
		SCL_PORT.OUT |= (1<< SCL_PIN);		// Rising edge

		//evaluate the port data (port set or clear)
		if(DOUT_PORT.IN & (1<<DOUT_PIN))	//Maybe bit in the first falling edge is wrong according to Figure 1a in datasheet
		{
			//bit is set, set LSB of resultPIND 3 16
			result = result | 0x01;
		}
	}
	return result;
}

uint32_t _encoder_convert_degrees(uint32_t encoder_value){
	uint32_t encoder_degrees;
	encoder_degrees = encoder_value * 360 /131072;
	return (encoder_degrees);
}

uint32_t gray_to_binary(uint32_t num)
{
	uint32_t mask;
	for (mask = num >> 1; mask != 0; mask = mask >> 1)
	{
		num = num ^ mask;
	}
	return num;
}