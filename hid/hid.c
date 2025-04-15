#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board_api.h"
#include "tusb.h"
#include "../uart/uart.h"

#include "usb_descriptors.h"
#include <hid.h>
#include "hid.h"

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

hid_macro_report_t hid_queue[HID_QUEUE_SIZE];
int head = 0;  // Where to insert next
int tail = 0;  // Where to remove from
int count = 0; // Number of items in queue

bool is_queue_full()
{
    return count == HID_QUEUE_SIZE;
}

bool is_queue_empty()
{
    return count == 0;
}

bool enqueue_hid_report(hid_macro_report_t *report)
{
    if (is_queue_full())
    {
        uart_send_string("Failed enqueue\n\r");
        return false;
    }
    hid_queue[head] = *report;
    head = (head + 1) % HID_QUEUE_SIZE;
    count++;
    return true;
}

bool dequeue_hid_report(hid_macro_report_t *out_report)
{
    if (is_queue_empty())
        return false;

    uart_send_string("Dequeue\n\r");

    *out_report = hid_queue[tail]; // Assuming we want to return the first keycode
    tail = (tail + 1) % HID_QUEUE_SIZE;
    count--;
    return true;
}

void macropad_hid_init(void)
{
    board_init();

    // init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);

    if (board_init_after_tusb)
    {
        board_init_after_tusb();
    }
}

void send_key_down(uint8_t key_id)
{
    if (!tud_hid_ready())
        return;
    hid_macro_report_t report = {0};
    report.keycode[0] = key_id;

    enqueue_hid_report(&report);
}

void send_release_all(void)
{
    if (!tud_hid_ready())
        return;

    hid_macro_report_t report = {0};
    enqueue_hid_report(&report);
}

void send_hid_report(uint8_t report_id, uint32_t btn)
{
    // skip if hid is not ready yet
    if (!tud_hid_ready())
        return;

    switch (report_id)
    {
    case REPORT_ID_KEYBOARD:
    {
        // use to avoid send multiple consecutive zero report for keyboard
        static bool has_keyboard_key = false;

        if (btn)
        {
            uint8_t keycode[6] = {0};
            keycode[0] = HID_KEY_A;

            tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
            has_keyboard_key = true;
        }
        else
        {
            // send empty key report if previously has key pressed
            if (has_keyboard_key)
                tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
            has_keyboard_key = false;
        }
    }
    break;

    default:
        break;
    }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(void)
{
    if (!tud_hid_ready())
        return;

    hid_macro_report_t report;
    if (dequeue_hid_report(&report))
    {
        // Send the first report the rest is handled by tud_hid_report_complete_cb
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, report.modifier, report.keycode);
    }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len)
{
    (void)instance;
    (void)len;

    hid_macro_report_t next_report;
    if (dequeue_hid_report(&next_report))
    {
        // Send the first report the rest is handled by tud_hid_report_complete_cb
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, next_report.modifier, next_report.keycode);
    }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    (void)instance;

    if (report_type == HID_REPORT_TYPE_OUTPUT)
    {
        // Set keyboard LED e.g Capslock, Numlock etc...
        if (report_id == REPORT_ID_KEYBOARD)
        {
            // bufsize should be (at least) 1
            if (bufsize < 1)
                return;

            uint8_t const kbd_leds = buffer[0];

            if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
            {
                // Capslock On: disable blink, turn led on
                board_led_write(true);
            }
            else
            {
                // Caplocks Off: back to normal blink
                board_led_write(false);
            }
        }
    }
}