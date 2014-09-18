/** @file
 * @author Orestis Zachariadis
 * @brief Error handling routines
 */
#include "gpio_api.h"
#include "wait_api.h"
#include "conf.h"

/**@brief This function is called in various unrecoverable error conditions
 * @note It is declared in common/board.c as WEAK so we can easily override it*/
void mbed_die() {

#if DEVELOPMENT

    __disable_irq();	// dont allow interrupts to disturb the flash pattern

    gpio_t led_1;
    gpio_init_out(&led_1, LED1);
    gpio_t led_2;
    gpio_init_out(&led_2, LED2);
    gpio_t led_3;
    gpio_init_out(&led_3, LED3);
    gpio_t led_4;
    gpio_init_out(&led_4, LED4);

    while (1) {
        gpio_write(&led_1, 1);
        gpio_write(&led_2, 0);
        gpio_write(&led_3, 0);
        gpio_write(&led_4, 1);

        wait_ms(150);

        gpio_write(&led_1, 0);
        gpio_write(&led_2, 1);
        gpio_write(&led_3, 1);
        gpio_write(&led_4, 0);

        wait_ms(150);
    }

#else
    NVIC_SystemReset();
#endif

}

/** @name Processor Fault Handlers
 *  Cortex microprocessor exceptions that occur due to some fault.
 *  @see Cortex™-M4 Devices : Generic User Guide
 */
///@{
/** A HardFault is an exception that occurs because of an error during
 * exception processing, or because an exception cannot be managed by any
 * other exception mechanism. */
void HardFault_Handler(void) {
    mbed_die();
}

/** A MemManage fault is an exception that occurs because of a memory
 * protection related fault. */
void MemManage_Handler(void) {
    mbed_die();
}

/** A BusFault is an exception that occurs because of a memory related fault
 * for an instruction or data memory transaction. */
void BusFault_Handler(void) {
    mbed_die();
}

/** A UsageFault is an exception that occurs because of a fault related to
 * instruction execution. */
void UsageFault_Handler(void) {
    mbed_die();
}

///@}
