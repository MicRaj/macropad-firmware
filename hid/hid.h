#ifndef HID_H
#define HID_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board_api.h"
#include "tusb.h"

#include "usb_descriptors.h"
#include <hid.h>

void macropad_hid_init(void);
void send_key_down(uint8_t key_id);
void send_release_all(void);
void send_hid_report(uint8_t report_id, uint32_t btn);
void hid_task(void);

#endif