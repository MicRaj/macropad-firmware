/**
 * @file macro_hid.h
 * @author Michal Rajzer
 * @brief HID report handling and queue management.
 * 
 * @details
 * Uses TinyUSB as the USB stack and communicates with the host
 * using the HID protocol.
 */

#ifndef HID_H
#define HID_H

#include <stdlib.h>
#include <string.h>

#include <bsp/board_api.h>
#include <tusb.h>
#include <hid.h>

#include "macro_hid_types.h"
#include "macro_queue.h"
#include "usb_descriptors.h"
#include "tusb_config.h"

/**
 * @brief Initializes the macropad HID system.
 */
void macropad_hid_init(void);

/**
 * @brief Sends a key press (key down) event. Used for testing purposes.
 * @param key_id HID usage ID of the key to press.
 */
void send_key_down(uint8_t key_id);

/**
 * @brief Sends a key release (all keys up) event.
 */
void send_release_all(void);

/**
 * @brief Sends queued HID reports.
 *
 * @details
 * Dequeues the first report and sends it, subsequent reports are sent using
 * the `tud_hid_report_complete_cb` callback. Should be called frequently in
 * the main loop to ensure responsiveness.
 */
void hid_task(void);

#endif /* HID_H */