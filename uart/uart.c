
#include "uart.h"

void macropad_uart_init()
{
    // Initialize the UART with the specified baud rate
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));
}

void uart_send_string(const char *str)
{
    // Send a string with CR/LF conversions
    uart_puts(UART_ID, str);
}
