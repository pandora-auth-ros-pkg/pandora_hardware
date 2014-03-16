/*
 * system.c
 *
 * Created: 3/11/2014 11:58:56 PM
 *  Author: Klpanagi
 */ 
#include <asf.h>
#include "Include/RS232.h"


Bool rs232_com_success;

void system_init (){
/* Initialize Fsys clock to use external XTAL and PLL for Fsys=32MHz*/
	board_init();
	sysclk_init();
	rs232_initialization();
}