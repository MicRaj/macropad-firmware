#ifndef CORE_H
#define CORE_H
#include "../macro_hid/macro_hid.h"
#define HID_REPORT(mod, k0, k1, k2, k3, k4, k5) \
    {                                           \
        .modifier = (mod), .keycode = {(k0),    \
                                       (k1),    \
                                       (k2),    \
                                       (k3),    \
                                       (k4),    \
                                       (k5) }   \
    }

#define MAX_SEQUENCE_LENGTH 20
typedef struct
{
    hid_macro_report_t report_sequence[MAX_SEQUENCE_LENGTH];
    uint8_t length;
} hid_macro_sequence_t;

void macropad_core_init();

void play_macro_sequence(uint8_t macro_id);

void write_macro_sequence(uint8_t macro_id, hid_macro_sequence_t *macro_sequence);

#endif