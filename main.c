/**
 * @file main.c
 * @author Michal Rajzer
 * @brief Main file for the macropad firmware.
 * @version 0.1
 * @date 2025-10-08
 *
 * @details
 * This file contains the main initialisation and execution loop for the macropad firmware.
 * It performs the following tasks:
 *   - Initialises hardware and software modules, including GPIO, UART, HID, Flash, and Core functionality.
 *   - Scans the macropad keys and executes macro sequences.
 *   - Handles USB communication via TinyUSB, including HID report generation and transmission.
 *
 * @note
 * The main loop must not be blocked by long delays or blocking functions.
 * The function `tud_task()` must be called as frequently as possible within the main loop
 * to ensure responsive USB HID behavior and maintain reliable communication with the host.
 */

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
    macropad_uart_init();
    macropad_gpio_init();
    macropad_hid_init();
    macropad_core_init();
    macropad_flash_init();
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
                send_release_all(); // TODO do we want to store this in the macro?
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