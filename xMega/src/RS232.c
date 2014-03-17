/*
 * RS232.c
 *
 * Created: 3/11/2014 11:44:31 PM
 *  Author: Klpanagi
 */ 
#include <asf.h>
#include "include/RS232.h"

const uint8_t start_seq[] = "PANDORA";
const uint8_t start_length = 7;
uint8_t	*rx_buf;
uint8_t *tx_buf;

static usart_rs232_options_t USART_SERIAL_OPTIONS = {
	.baudrate = USART_SERIAL_BAUDRATE,
	.charlength = USART_SERIAL_CHAR_LENGTH,
	.paritytype = USART_SERIAL_PARITY,
	.stopbits = USART_SERIAL_STOP_BIT
};

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
	usart_putchar(USART_SERIAL, UACK );
}

void send_conf_packet(sensors_info data){
	tx_buf[0] = data.num_Sensors;
	tx_buf[1] = data.num_Sonars;
	tx_buf[2] = data.num_IR;
	tx_buf[3] = data.num_TPA;
	
	usart_serial_write_packet(USART_SERIAL, tx_buf, CONF_PACKET_SIZE);
}

void testing_com(){
	uint8_t received_byte;
	while (true) {
		received_byte = usart_getchar(USART_SERIAL);
		usart_putchar(USART_SERIAL, 0x06);
	}
}


void rs232_initialization(){
	sysclk_enable_module(SYSCLK_PORT_D, PR_USART0_bm);
	usart_init_rs232(USART_SERIAL, &USART_SERIAL_OPTIONS);
}

/*
void test_rs232_com(){
	uint8_t tx_buf[] = "\n\rHello AVR world ! : ";
	uint8_t tx_length = 22;
	uint8_t received_byte;
	uint8_t i;
	
	for (i = 0; i < tx_length; i++) {
		usart_putchar(USART_SERIAL, tx_buf[i]);
	}
	
	while (true) {
		received_byte = usart_getchar(USART_SERIAL);
		if (received_byte == '\r') {
			for (i = 0; i < tx_length; i++) {
				usart_putchar(USART_SERIAL, tx_buf[i]);
			}
		} else
		usart_putchar(USART_SERIAL, received_byte);
	}
	
}
*/