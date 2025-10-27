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

uint8_t b_tx_ready_flag = 0; /**< USB ready flag: non-zero if a new HID report can be sent (previous transfer completed). */
uint8_t programming_cmd_flag = 0; /**< Programming command flag: a new programming command has been received from the host.*/

hid_report_queue_t hid_report_queue = {0};
host_cmd_queue_t host_cmd_queue = {0};

bool is_hid_report_queue_full()
{
    return count == HID_QUEUE_SIZE;
}

bool is_hid_report_queue_empty()
{
    return count == 0;
}

bool enqueue_hid_report(hid_macro_report_t *report)
{
    if (is_hid_report_queue_full())
    {
        return false;
    }
    hid_queue[head] = *report;
    head = (head + 1) % HID_QUEUE_SIZE; // Circular buffer
    count++;
    return true;
}

bool dequeue_hid_report(hid_macro_report_t *out_report)
{
    if (is_hid_report_queue_empty())
        return false;

    *out_report = hid_queue[tail];
    tail = (tail + 1) % HID_QUEUE_SIZE;
    count--;
    return true;
}

bool is_host_cmd_queue_full()
{
    return count == HID_QUEUE_SIZE;
}

bool is_hostt_queue_empty()
{
    return count == 0;
}

bool enqueue_hid_report(hid_macro_report_t *report)
{
    if (is_hid_report_queue_full())
    {
        return false;
    }
    hid_queue[head] = *report;
    head = (head + 1) % HID_QUEUE_SIZE; // Circular buffer
    count++;
    return true;
}

bool dequeue_hid_report(hid_macro_report_t *out_report)
{
    if (is_hid_report_queue_empty())
        return false;

    *out_report = hid_queue[tail];
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

    // Frequent tud_task calls for device enumeration - placeholder fix.
    for (int i = 0; i < 30; i++)
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

void hid_task(void)
{
    if (!tud_hid_ready())
        return;

    hid_macro_report_t report;
    if (usb_tx_ready_flag && dequeue_hid_report(&report))
    {
        usb_tx_ready_flag = 0;
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, report.modifier, report.keycode);
    }
}

// Invoked when sent REPORT successfully to host
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len)
{
    (void)instance;
    (void)len;

    usb_tx_ready_flag = 1;
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