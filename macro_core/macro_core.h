/**
 * @file macro_core.h
 * @brief Core macropad logic: macro storage, playback, and management.
 * @author Michal Rajzer
 *
 * @details
 * Defines data structures for HID macro sequences and the macro store,
 * along with functions to initialize the core, play macros, and write
 * sequences to flash memory.
 *
 * @note Macros are loaded from flash into RAM at flash initialisation.
 *       `write_macro_sequence()` updates the in-memory store only;
 *       use `write_current_macro_store_to_flash()` to persist changes.
 */
#ifndef CORE_H
#define CORE_H
#include "../macro_hid/macro_hid.h"
#include "../macro_uart/macro_uart.h"

#define NUM_MACROS 9

/** Helper macro to define a HID report */
#define HID_REPORT(mod, k0, k1, k2, k3, k4, k5) \
    {                                           \
        .modifier = (mod), .keycode = {(k0),    \
                                       (k1),    \
                                       (k2),    \
                                       (k3),    \
                                       (k4),    \
                                       (k5) }   \
    }

#define MAX_SEQUENCE_LENGTH 255 /**< Max number of reports in a macro sequence. */ // Max that fits in uint8_t length.

/** Macro sequence containing multiple HID reports */
typedef struct
{
    hid_macro_report_t report_sequence[MAX_SEQUENCE_LENGTH];
    uint8_t length;
} macro_sequence_t; // TODO Change these to macro_sequence_t?

/** Macro store containing multiple sequences */
typedef struct
{
    macro_sequence_t macro_sequences[NUM_MACROS];
} macro_store_t;

/**
 * @brief Used to write hardcoded macros to flash on first run. Unused otherwise.
 * 
 * Uncomment the relevant section in the implementation file to use.
 */
void macropad_core_init();

/**
 * @brief Play a macro sequence by its ID.
 * @param macro_id ID of the macro sequence to play (0–8)
 */
void play_macro_sequence(uint8_t macro_id);

/**
 * @brief Write or update a macro sequence in the store.
 * @param macro_id ID of the macro sequence to write (0–8)
 * @param macro_sequence Pointer to the sequence data to store
 */
void write_macro_sequence(uint8_t macro_id, macro_sequence_t *macro_sequence);

/**
 * @brief Persist the current macro store to flash memory.
 */
void write_current_macro_store_to_flash();

#endif