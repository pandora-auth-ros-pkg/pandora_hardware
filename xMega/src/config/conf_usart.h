/*
 * conf_usart.h
 *
 * Created: 3/11/2014 7:44:53 PM
 *  Author: Klpanagi
 */ 


#ifndef CONF_USART_H_
#define CONF_USART_H_
	#define USART_SERIAL                     &USARTD0
	#define USART_SERIAL_BAUDRATE            9600
	#define USART_SERIAL_CHAR_LENGTH         USART_CHSIZE_8BIT_gc
	#define USART_SERIAL_PARITY              USART_PMODE_DISABLED_gc
	#define USART_SERIAL_STOP_BIT            false
#endif /* CONF_USART_H_ */