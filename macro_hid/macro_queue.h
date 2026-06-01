/**
 * @file macro_queue.h
 * @author Michal Rajzer
 * @brief Report queue management.
 */

#ifndef MACRO_QUEUE_H
#define MACRO_QUEUE_H

#include <stdbool.h>
#include "macro_hid_types.h"

#define HID_QUEUE_SIZE 1024 /**< Maximum number of HID reports that can be queued. */

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

#endif // MACRO_QUEUE_H