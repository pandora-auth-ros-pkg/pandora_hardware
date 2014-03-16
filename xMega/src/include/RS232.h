/*
 * RS232.h
 *
 * Created: 3/11/2014 11:55:12 PM
 *  Author: Klpanagi
 */ 


#ifndef RS232_H_
#define RS232_H_

	#define ACK 0x06
	#define NAK 0x15
	#define REQUEST_COM '\a' //The bell character (BEL)

#endif /* RS232_H_ */

extern void rs232_initialization();
extern void test_rs232_com();

