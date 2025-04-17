#include "uart/uart.h"
#include "gpio/gpio.h"
#include "macro_hid/macro_hid.h"
#include "tusb.h"

bool pressed = false;

int main()
{
    stdio_init_all();
    macropad_uart_init();
    macropad_gpio_init();
    macropad_hid_init();
    mount_led_flash();

    while (1)
    {
        tud_task(); // tinyusb device task
        hid_task();
        int key_idx = matrix_scan();

        if (key_idx >= 0)
        {
            char message[50];
            snprintf(message, sizeof(message), "Key pressed: %d\n", key_idx);
            uart_send_string(message);
            if (key_idx == 1)
            {
                if (!pressed)
                {
                    send_key_down(HID_KEY_A);
                    send_key_down(HID_KEY_B);
                    send_key_down(HID_KEY_C);
                    send_release_all();
                    pressed = true;
                }
            }
            else
            {
                if (pressed)
                {
                    pressed = false;
                }
            }
        }
    }
    return 0;
}