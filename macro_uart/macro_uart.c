/**
 * @file macro_uart.c
 * @author Michal Rajzer
 * @brief Implementation of UART functions for the macropad firmware.
 *
 * @details
 * Used for debugging and sending key press information over UART.
 * @see macro_uart.h
 */

#include "macro_uart.h"
#define UART_DEBUG 1
void macropad_uart_init()
{
    #ifdef UART_DEBUG
    // Initialize the UART with the specified baud rate
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));
    #endif
}

void uart_send_string(const char *str)
{
    #ifdef UART_DEBUG
    // Send a string with CR/LF conversions
    uart_puts(UART_ID, str);
    #endif
}
