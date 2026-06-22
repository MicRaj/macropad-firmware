#ifndef MACRO_CORE_TYPES_H
#define MACRO_CORE_TYPES_H

#include "../macro_hid/macro_hid_types.h"

#define NUM_MACROS 9

#define MAX_SEQUENCE_LENGTH 255 /**< Max number of reports in a macro sequence. */ // Max that fits in uint8_t length.

/** Macro sequence containing multiple HID reports */
typedef struct
{
    hid_macro_report_t report_sequence[MAX_SEQUENCE_LENGTH];
    uint8_t length;
} macro_sequence_t;

/** Macro store containing multiple sequences */
typedef struct
{
    macro_sequence_t macro_sequences[NUM_MACROS];
} macro_store_t;
#endif // MACRO_CORE_TYPES_H