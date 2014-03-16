/*
 * PCTX.c
 *
 * Created: 3/12/2014 11:25:50 PM
 *  Author: Klpanagi
 */ 

/* 
 * Method for Startup communication between xMega and PC. Waiting for PC to send the REQUEST_COM.
 * When REQUEST_COM is received, xMega sends ACK back to PC. 
 * Communication between the 2 nodes is then established.
 */ 
#include "include/RS232.h"
#include "include/sensors.h"
#include "include/PCTX.h"
#include <asf.h>

const uint8_t start_seq[] = "PANDORA";
const uint8_t start_length = 7;
uint8_t	*rx_buf;
uint8_t *tx_buf; 

void pc_start_com(){
	uint8_t received_byte;
	Bool success = false;
	uint8_t matched = 0;
	
	while(!success){
		usart_serial_read_packet(USART_SERIAL, rx_buf, start_length);
		for(int i=0;i<start_length;i++){
			if (rx_buf[i] == start_seq[i]){matched+=1;}
		}
		success = (matched == start_length);
	}
	//usart_serial_write_packet(USART_SERIAL, rx_buf, start_length); //debugging
	usart_putchar(USART_SERIAL, ACK );
}

void send_conf_packet(sensors_info data){
	tx_buf[1] = data.num_Sensors;
	tx_buf[2] = data.num_Sonars;
	tx_buf[3] = data.num_IR;
	tx_buf[4] = data.num_TPA;
	
	usart_serial_write_packet(USART_SERIAL, tx_buf, CONF_PACKET_SIZE);
}

void testing_com(){
	uint8_t received_byte;
	while (true) {
		received_byte = usart_getchar(USART_SERIAL);
		usart_putchar(USART_SERIAL, 0x06);
	}
}