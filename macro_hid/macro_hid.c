/**
 * @file macro_hid.c
 * @author Michal Rajzer
 * @brief HID report handling and queue management.
 *
 * @details
 * Uses TinyUSB as the USB stack and communicates with the host
 * using the HID protocol.
 * @see macro_hid.h
 */

#include "macro_hid.h"
#include "../macro_uart/macro_uart.h"

volatile bool tx_ready = true;

void macropad_hid_init(void)
{
    board_init();

    // init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);

    if (board_init_after_tusb)
    {
        board_init_after_tusb();
    }

    // Frequent tud_task calls for device enumeration - placeholder fix.
    for (int i = 0; i < 50; i++)
    {
        tud_task();
        sleep_ms(10);
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

// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(void)
{
    if (!tud_hid_ready())
        return;

    hid_macro_report_t report;
    if (tx_ready && dequeue_hid_report(&report))
    {
        tx_ready = false;
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, report.modifier, report.keycode);
    }
}
