#include "macro_core.h"
//--------------------------------------------------------------------+
// Core macro functionality
//--------------------------------------------------------------------+

hid_macro_store_t macro_store;

void macropad_core_init()
{
    // Initial Macro Programming to flash
    // for (int i = 0; i < 9; i++)
    // {
    //     macro_store.macro_sequences[i].length = 0;
    // }

    // hid_macro_sequence_t key1_sequence = {
    //     .report_sequence = {
    //         HID_REPORT(KEYBOARD_MODIFIER_LEFTSHIFT, HID_KEY_H, 0, 0, 0, 0, 0), // Shift + H
    //         HID_REPORT(0, HID_KEY_E, 0, 0, 0, 0, 0),                           // E
    //         HID_REPORT(0, HID_KEY_L, 0, 0, 0, 0, 0),                           // L
    //         HID_REPORT(0, 0, 0, 0, 0, 0, 0),                                   // Release between same key press
    //         HID_REPORT(0, HID_KEY_L, 0, 0, 0, 0, 0),                           // L
    //         HID_REPORT(0, HID_KEY_O, 0, 0, 0, 0, 0),                           // O
    //         HID_REPORT(0, 0, 0, 0, 0, 0, 0),                                   // Release at the end of the macro
    //     },
    //     .length = 7};

    // hid_macro_sequence_t key2_sequence = {
    //     .report_sequence = {
    //         HID_REPORT(0, HID_KEY_2, 0, 0, 0, 0, 0), // 2
    //         HID_REPORT(0, 0, 0, 0, 0, 0, 0),         // Release at the end of the macro
    //     },
    //     .length = 2};

    // hid_macro_sequence_t key3_sequence = {
    //     .report_sequence = {
    //         HID_REPORT(0, HID_KEY_3, 0, 0, 0, 0, 0), // 3
    //         HID_REPORT(0, 0, 0, 0, 0, 0, 0),         // Release at the end of the macro
    //     },
    //     .length = 2};

    // hid_macro_sequence_t key4_sequence = {
    //     .report_sequence = {
    //         HID_REPORT(0, HID_KEY_4, 0, 0, 0, 0, 0), // 4
    //         HID_REPORT(0, 0, 0, 0, 0, 0, 0),         // Release at the end of the macro
    //     },
    //     .length = 2};

    // hid_macro_sequence_t key5_sequence = {
    //     .report_sequence = {
    //         HID_REPORT(0, HID_KEY_5, 0, 0, 0, 0, 0), // 5
    //         HID_REPORT(0, 0, 0, 0, 0, 0, 0),         // Release at the end of the macro
    //     },
    //     .length = 2};

    // hid_macro_sequence_t key6_sequence = {
    //     .report_sequence = {
    //         HID_REPORT(0, HID_KEY_6, 0, 0, 0, 0, 0), // 6
    //         HID_REPORT(0, 0, 0, 0, 0, 0, 0),         // Release at the end of the macro
    //     },
    //     .length = 2};

    // hid_macro_sequence_t key7_sequence = {
    //     .report_sequence = {
    //         HID_REPORT(0, HID_KEY_7, 0, 0, 0, 0, 0), // 7
    //         HID_REPORT(0, 0, 0, 0, 0, 0, 0),         // Release at the end of the macro
    //     },
    //     .length = 2};

    // hid_macro_sequence_t key8_sequence = {
    //     .report_sequence = {
    //         HID_REPORT(0, HID_KEY_8, 0, 0, 0, 0, 0), // 8
    //         HID_REPORT(0, 0, 0, 0, 0, 0, 0),         // Release at the end of the macro
    //     },
    //     .length = 2};

    // hid_macro_sequence_t key9_sequence = {
    //     .report_sequence = {
    //         HID_REPORT(0, HID_KEY_9, 0, 0, 0, 0, 0), // 9
    //         HID_REPORT(0, 0, 0, 0, 0, 0, 0),         // Release at the end of the macro
    //     },
    //     .length = 2};

    // write_macro_sequence(0, &key1_sequence);
    // write_macro_sequence(1, &key2_sequence);
    // write_macro_sequence(2, &key3_sequence);
    // write_macro_sequence(3, &key4_sequence);
    // write_macro_sequence(4, &key5_sequence);
    // write_macro_sequence(5, &key6_sequence);
    // write_macro_sequence(6, &key7_sequence);
    // write_macro_sequence(7, &key8_sequence);
    // write_macro_sequence(8, &key9_sequence);

    // int rc = write_store_to_flash(&macro_store);
    // if (rc == PICO_OK)
    // {
    //     uart_send_string("Flash Successful\r\n");
    // }
    // else
    // {
    //     uart_send_string("Flash Error\r\n");
    // }
}
void play_macro_sequence(uint8_t macro_id)
{
    if (macro_id >= 9)
        return;

    hid_macro_sequence_t *sequence = &macro_store.macro_sequences[macro_id];
    for (uint8_t i = 0; i < sequence->length; i++)
    {
        enqueue_hid_report(&sequence->report_sequence[i]);
    }
}

void write_macro_sequence(uint8_t macro_id, hid_macro_sequence_t *macro_sequence)
{
    // Makes a copy of macro sequence
    if (macro_id >= 9)
        return;

    if (macro_sequence->length > MAX_SEQUENCE_LENGTH)
        macro_sequence->length = MAX_SEQUENCE_LENGTH;

    hid_macro_sequence_t *sequence = &macro_store.macro_sequences[macro_id];

    sequence->length = macro_sequence->length;
    memcpy(sequence->report_sequence, macro_sequence->report_sequence, sizeof(macro_sequence->report_sequence));
}