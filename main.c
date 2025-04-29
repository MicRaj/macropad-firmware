#include "macro_uart/macro_uart.h"
#include "macro_gpio/macro_gpio.h"
#include "macro_hid/macro_hid.h"
#include <tusb.h>
#include <hid.h>

#define NUM_BUTTONS 9

//--------------------------------------------------------------------+
// Macropad Main Loop
//--------------------------------------------------------------------+

bool switch_states[NUM_BUTTONS] = {false};

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

            // Check if the key is not already pressed
            if (!switch_states[key_idx])
            {
                switch_states[key_idx] = true;

                switch (key_idx)
                {
                case 1:
                    send_key_down(HID_KEY_A);
                    break;
                case 2:
                    send_key_down(HID_KEY_B);
                    break;
                case 3:
                    send_key_down(HID_KEY_C);
                    break;
                case 4:
                    send_key_down(HID_KEY_D);
                    break;
                case 5:
                    send_key_down(HID_KEY_E);
                    break;
                case 6:
                    send_key_down(HID_KEY_F);
                    break;
                case 7:
                    send_key_down(HID_KEY_G);
                    break;
                case 8:
                    send_key_down(HID_KEY_H);
                    break;
                case 9:
                    send_key_down(HID_KEY_I);
                    break;
                default:
                    break;
                }
                send_release_all();
            }

            // Release all other keys if one is pressed
            for (int i = 0; i <= NUM_BUTTONS; i++)
            {
                if (i + 1 != key_idx && switch_states[i])
                {
                    switch_states[i] = false;
                }
            }
        }
        else
        {
            // Set all to false if no key is pressed
            memset(switch_states, 0, sizeof(switch_states));
        }
    }
    return 0;
}