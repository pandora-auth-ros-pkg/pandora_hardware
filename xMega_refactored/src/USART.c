/*
 * USART.c
 *
 * Created: 3/22/2014 5:17:03 PM
 *  Author: klpanagi
 */ 
#include "include/USART.h"
#include <asf.h>

static usart_rs232_options_t USART_SERIAL_OPTIONS = {
	.baudrate = 115200,
	.charlength = USART_CHSIZE_8BIT_gc,
	.paritytype = USART_PMODE_DISABLED_gc,
	.stopbits = true //Stopbits =2
};


void init_uart(void)
{
	PORTD.OUTSET = PIN3_bm;     // TX high
	PORTD.DIRSET = PIN3_bm;     // and output
	PORTD.DIRCLR = PIN2_bm;     // RX is input
	USARTD0.CTRLA = 0x00;		// 0x00:No IRQ, 0x01:Enable Data Register Empty Interrupt Level / 0x2A: Enable All USART interrupts at MediumLevel 
								//0x28: Enable Rx, Tx interrupts at MeduimLevel / 0x20: Enable Rx Interrupt at MediumLevel
	USARTD0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;// | USART_CLK2X_bm;
	USARTD0.CTRLC = 3|8;		//8: Stop_bits=2 / 3:Character Size = 8bits
	USARTD0.BAUDCTRLA = 131;	// 12 9600//137 28800// FIXED 135 for 57600  //FIX 131 FOR 115200 WITH CLK2X=0 (((CPU_SPEED) / (16)) / USARTF0BAUDRATE) - 1; //value=207 for CLKper CPU_SPEED=32MHz and desired speed USARTF0BAUDRATE
	USARTD0.BAUDCTRLB = 0xD0;	//4 9600//f0=-1 28800//D0=(-03) 115200//E0=(-2) 57600
}

void init_Rs232(USART_data_t *usart_data){
	//sysclk_enable_module(SYSCLK_PORT_D, PR_USART0_bm);
	usart_data->usart = & PC_USART;
	usart_data->buffer.RX_Tail = 0;
	usart_data->buffer.RX_Head = 0;
	usart_data->buffer.TX_Tail = 0;
	usart_data->buffer.TX_Head = 0;
	
	sysclk_enable_peripheral_clock(&USARTD0);
	usart_init_rs232(&USARTD0, &USART_SERIAL_OPTIONS);
	USARTD0.CTRLA |= (0 << 5 ); //Set bit 5 (MSB of RXCINTLVL) in CTRL1 - Rx Interrupt Level 
    USARTD0.CTRLA |= (1 << 4 ); //Set bit 4 (LSB of RXCINTLVL) in CTRL1 - Rx Interrupt Level 
	
}

void usart_putc(char c)
{
	while (!(USARTD0.STATUS & USART_DREIF_bm));
	USARTD0.DATA = c;
}

char usart_getc(void)
{
	while (!(USARTD0.STATUS & USART_RXCIF_bm));
	return USARTD0.DATA;
}

void usart_sendstring(char *s)
{
	while (*s) {
		usart_putc(*s++);
	}
}

void test_rs232(){
	uint8_t received;
	while(1){
		received = usart_getc();
		usart_sendstring("PANDORA");
	}
}

void echo_mode(){
	uint8_t received_byte;
	
	while(1){
		while (!(USARTD0.STATUS & USART_RXCIF_bm));
		received_byte = USARTD0.DATA;
		usart_putc(received_byte);	
	}
}

void echo_mode_string(){
	uint8_t received_byte[5];
	uint8_t count = 0;
	uint8_t i;
	
	while(1){
		while (!(USARTD0.STATUS & USART_RXCIF_bm));
		while ((USARTD0.STATUS & USART_RXCIF_bm)){
			received_byte[count] = USARTD0.DATA;
			count++;
		}
		for (i=0;i<count;i++){
			usart_putc(received_byte[i]);
		}
		count=0;
	}
}