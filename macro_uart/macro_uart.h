/**
 * @file macro_uart.h
 * @author Michal Rajzer
 * @brief UART interface for the macropad firmware.
 */

#ifndef MACROPAD_UART_H
#define MACROPAD_UART_H

#include <pico/stdlib.h>
#include <hardware/uart.h>

#define UART_ID uart0      /**< UART peripheral to use */
#define BAUD_RATE 115200   /**< UART baud rate */
#define UART_TX_PIN 0      /**< GPIO pin for UART TX */
#define UART_RX_PIN 1      /**< GPIO pin for UART RX */

/**
 * @brief Initialize UART with defined baud rate and pins.
 */
void macropad_uart_init(void);

/**
 * @brief Send a null-terminated string over UART.
 * @param str String to send. '\n' will be converted to '\r\n'.
 */
void uart_send_string(const char *str);

#endif // MACROPAD_UART_H