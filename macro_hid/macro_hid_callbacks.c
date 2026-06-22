/**
 * @file macro_hid_callbacks.c
 * @author Michal Rajzer
 * @brief Defines TinyUSB HID callback functions.
 */

#include <stdio.h>
#include "macro_host_commands.h"
#include "../macro_uart/macro_uart.h"
#include "macro_hid_types.h"
#include "macro_queue.h"
#include "usb_descriptors.h"

#include <bsp/board_api.h>
#include <tusb.h>

extern int tx_ready;

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len)
{
    (void)instance;
    (void)report;
    (void)len;
    tx_ready = true;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    // TODO not Implemented. Host sends request - used to get current macro settings.
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    char message[128];
    if (bufsize>5){
    snprintf(message, sizeof(message),
             "Interface (instance): %u, Report ID: %u, Length: %u, Report Type: %u\r\n",
             instance, report_id, bufsize, report_type);
    uart_send_string(message);

    for (uint16_t i = 0; i < bufsize; i++)
    {
        snprintf(message, sizeof(message), "0x%02X ", buffer[i]);
        uart_send_string(message);
    }

    uart_send_string("\r\n");
    }
    if (instance == ITF_NUM_HID)
    {
        if (report_type == HID_REPORT_TYPE_OUTPUT)
        {
            // Capslock, Numlock etc...
            if (report_id == REPORT_ID_KEYBOARD)
            {
                if (bufsize < 1)
                    return;

                uint8_t const kbd_leds = buffer[0];

                if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
                {
                    board_led_write(true);
                }
                else
                {
                    board_led_write(false);
                }
            }
        }
    }
    else if (instance == ITF_NUM_PROGRAMMING)
    {
        if (bufsize <= 1) // Bug on application side, sending 1 byte reports.
        {
            return;
        }
        uart_send_string("Custom Report Received\r\n");
        uint8_t const *host_command = buffer + 1;
        if (bufsize < 8)
        {
            uart_send_string("Incorrect size\r\n");
            return;
        }
        execute_host_command((hid_host_cmd_t *)host_command);
    }
}