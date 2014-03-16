/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include "Include/RS232.h"
#include "Include/system.h"
#include "Include/PCTX.h"
//#include <conf_usart.h>

/************************************************************************/
/*								GLOBALS                                                                     */
/************************************************************************/


int main (void)
{	
	system_init ();
	pc_start_com();
	//testing_com();
	//test_rs232_com();
}
