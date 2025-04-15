#include "uart/uart.h"
#include "gpio/gpio.h"
#include "hid/hid.h"
#include "tusb.h"

bool pressed = false;

int main()
{
    stdio_init_all();
    macropad_uart_init();
    macropad_gpio_init();
    macropad_hid_init();
    bool key1_now;
    static bool key1_prev = false;
    while (1)
    {
        tud_task(); // tinyusb device task
        hid_task();
        int key_idx = matrix_scan();
        key1_now = (key_idx == 1);

        if (key1_now && !key1_prev)
        {
            char message[50];
            snprintf(message, sizeof(message), "Key pressed: %d\n", key_idx);
            uart_send_string(message);
            // rising edge: key was just pressed
            send_key_down(HID_KEY_B);
            send_release_all();
            send_key_down(HID_KEY_C);
            send_release_all();
        }
        else if (!key1_now && key1_prev)
        {
            // falling edge: key was just released
            send_release_all();
        }

        key1_prev = key1_now; // remember for next cycle

        // if (key_idx >= 0)
        // {
        //     char message[50];
        //     snprintf(message, sizeof(message), "Key pressed: %d\n", key_idx);
        //     uart_send_string(message);
        //     if (key_idx == 1)
        //     {
        //         if (!pressed)
        //         {
        //             send_key_down(HID_KEY_B);
        //             send_key_down(HID_KEY_C);
        //             send_release_all();
        //             pressed = true;
        //         }
        //     }
        //     else
        //     {
        //         if (pressed)
        //         {
        //             // Release the key when it's no longer pressed
        //             send_release_all();
        //             pressed = false;
        //         }
        //     }
    }
    return 0;
}