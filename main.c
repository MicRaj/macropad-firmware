#include "macro_uart/macro_uart.h"
#include "macro_gpio/macro_gpio.h"
#include "macro_hid/macro_hid.h"
#include "macro_core/macro_core.h"
#include <tusb.h>
#include <hid.h>

//--------------------------------------------------------------------+
// Macropad Main Loop
//--------------------------------------------------------------------+

bool pressed = false;

int main()
{
    stdio_init_all();
    macropad_uart_init();
    macropad_gpio_init();
    macropad_hid_init();
    mount_led_flash();

    hid_macro_sequence_t key1_sequence = {
        .report_sequence = {
            HID_REPORT(0x00, HID_KEY_A, 0, 0, 0, 0, 0), // A
            HID_REPORT(0x00, HID_KEY_B, 0, 0, 0, 0, 0), // B
            HID_REPORT(0x02, HID_KEY_C, 0, 0, 0, 0, 0), // Shift + C
        },
        .length = 3};

    write_macro_sequence(0, &key1_sequence);

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
                    play_macro_sequence(key_idx - 1);
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