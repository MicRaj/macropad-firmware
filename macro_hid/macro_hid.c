#include "macro_hid.h"
#include "../macro_uart/macro_uart.h"

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

// Initial functionality, callback needs to be defined.
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    (void)instance;
    char message[64];
    snprintf(message, sizeof(message), "Report ID: %d, Length: %d\r\n", report_id, bufsize);
    uart_send_string(message);
    report_id = buffer[0]; // TODO Bug tud_hid_set_report not reading first byte as report_id

    // Print the buffer in hex
    for (uint16_t i = 0; i < bufsize; i++)
    {
        snprintf(message, sizeof(message), "0x%02X ", buffer[i]);
        uart_send_string(message);
    }

    uart_send_string("\r\n");

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
        else if (report_id == REPORT_ID_CUSTOM)
        {
            // // bufsize should be (at least) 1
            // if (bufsize < 64)
            //     return;

            uart_send_string("Custom Report Received");

            // uint8_t macro_id = buffer[1];
            // uint8_t action = buffer[2] // 0 for clear, 1 for add
            // uint8_t macro_report_count // Amount of reports added
            // uint8_t *data = buffer + 4 // 60Bytes Remaining.for data
        }
    }
}