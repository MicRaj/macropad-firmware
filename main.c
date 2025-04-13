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

    while (1)
    {
        sleep_ms(10);
        tud_task(); // tinyusb device task
        // hid_task();
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
                    send_button_press(HID_KEY_B);
                    pressed = true;
                }
            }
            else
            {
                if (pressed)
                {
                    // Release the key when it's no longer pressed
                    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
                    pressed = false;
                }
            }
        }
    }
    return 0;
}
