
#include <asf.h>
#include "include/USART.h"
#include "include/system.h"


volatile uint8_t received_byte;
USART_data_t PC_USART_data;


int main(void)
{
	board_init();
	sysclk_init();
	init_Rs232(&PC_USART_data);
	
	//echo_mode();
	echo_mode_string();
}



/*! \brief Receive complete interrupt service routine.
 *
 *  Receive complete interrupt service routine.
 *  Calls the common receive complete handler with pointer to the correct USART
 *  as argument.
 */
/*
ISR(USARTD0_RXC_vect)
{
	USART_RXComplete(&USART_data);
}
*/

/*! \brief Data register empty  interrupt service routine.
 *
 *  Data register empty  interrupt service routine.
 *  Calls the common data register empty complete handler with pointer to the
 *  correct USART as argument.
 */
/*
ISR(USARTC0_DRE_vect)
{
	USART_DataRegEmpty(&USART_data);
}*/
