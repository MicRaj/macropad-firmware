#ifndef UART_H
#define UART_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

void macropad_uart_init();

void uart_send_string(const char *str);

#endif