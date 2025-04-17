#ifndef HID_H
#define HID_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <bsp/board_api.h>
#include <tusb.h>

#include "usb_descriptors.h"

#define HID_QUEUE_SIZE 1024

// HID Report Struct (Report ID and reserved removed)
typedef struct
{
    uint8_t modifier;
    uint8_t keycode[6];
} hid_macro_report_t;

bool is_queue_full();
bool is_queue_empty();
bool enqueue_hid_report(hid_macro_report_t *report);
bool dequeue_hid_report(hid_macro_report_t *out_report);

void macropad_hid_init(void);
void send_key_down(uint8_t key_id);
void send_release_all(void);
void hid_task(void);

#endif