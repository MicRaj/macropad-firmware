/**
 * @file macro_queue.c
 * @author Michal Rajzer
 * @brief Report queue management.
 * @see macro_queue.h
 */

#include "macro_queue.h"

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
    head = (head + 1) % HID_QUEUE_SIZE; // Circular buffer
    count++;
    return true;
}

bool dequeue_hid_report(hid_macro_report_t *out_report)
{
    if (is_queue_empty())
        return false;

    *out_report = hid_queue[tail];
    tail = (tail + 1) % HID_QUEUE_SIZE;
    count--;
    return true;
}