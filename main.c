#include "uart/uart.h"
#include "gpio/gpio.h"
#include "hid/hid.h"

int main()
{
    stdio_init_all();
    macropad_uart_init();
    macropad_gpio_init();
    macropad_hid_init();

    while (1)
    {
        hid_task();
        int key_idx = matrix_scan();
        if (key_idx >= 0)
        {
            char message[50];
            snprintf(message, sizeof(message), "Key pressed: %d\n", key_idx);
            uart_send_string(message);
            send_hid_report(REPORT_ID_KEYBOARD, key_idx);
        }
    }

    return 0;
}