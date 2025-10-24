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
#include <stdio.h>
#include <string.h>

#include <bsp/board_api.h>
#include <tusb.h>
#include <hid.h>

#include "usb_descriptors.h"
#include "tusb_config.h"

// TODO Dynamic allocation with linked list?
#define HID_QUEUE_SIZE 1024 /**< Maximum number of HID reports that can be queued. */

/**
 * @brief HID keyboard report structure (Report ID and reserved byte removed).
 */
typedef struct __attribute__((packed)) // Packing for memcpy.
{
    uint8_t modifier;   /**< Modifier key bitmask (Ctrl, Shift, Alt, GUI). */
    uint8_t keycode[6]; /**< Array of up to six simultaneous key codes. */
} hid_macro_report_t;

/**
 * @brief Checks if the HID report queue is full.
 * @return true if the queue is full, false otherwise.
 */
bool is_queue_full(void);

/**
 * @brief Checks if the HID report queue is empty.
 * @return true if the queue is empty, false otherwise.
 */
bool is_queue_empty(void);

/**
 * @brief Adds a HID report to the queue.
 * @param report Pointer to the HID report to enqueue.
 * @return true if the report was successfully enqueued, false if the queue is full.
 */
bool enqueue_hid_report(hid_macro_report_t *report);

/**
 * @brief Removes and retrieves the next HID report from the queue.
 * @param[out] out_report Pointer to store the dequeued HID report.
 * @return true if a report was dequeued, false if the queue was empty.
 */
bool dequeue_hid_report(hid_macro_report_t *out_report);

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