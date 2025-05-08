#include "macro_uart/macro_uart.h"
#include "macro_gpio/macro_gpio.h"
#include "macro_hid/macro_hid.h"
#include "macro_core/macro_core.h"
#include "macro_core/macro_flash.h"
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
    macropad_core_init();
    mount_led_flash();

    while (1)
    {
        tud_task(); // tinyusb device task
        hid_task();

        int key_idx = matrix_scan();

        if (key_idx >= 0)
        {
            char message[50];
            snprintf(message, sizeof(message), "Key pressed: %d\r\n", key_idx);
            uart_send_string(message);

            // Check if the key is not already pressed
            if (!switch_states[key_idx])
            {
                switch_states[key_idx] = true;
                play_macro_sequence(key_idx);
                send_release_all();
            }

            // Release all other keys if one is pressed
            for (int i = 0; i < NUM_BUTTONS; i++)
            {
                if (i != key_idx && switch_states[i] == true)
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