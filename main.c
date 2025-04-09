#include "uart/uart.h"
#include "gpio/gpio.h"

int main()
{
    stdio_init_all();
    macropad_uart_init();
    macropad_gpio_init();

    while (1)
    {
        int key_idx = matrix_scan();
        if (key_idx >= 0)
        {
            char message[50];
            snprintf(message, sizeof(message), "Key pressed: %d\n", key_idx);
            uart_send_string(message);
        }
    }

    return 0;
}