#include "macro_core.h"

hid_macro_sequence_t macro_sequences[9];

void init_macro_sequences()
{
    for (int i = 0; i < 9; i++)
    {
        macro_sequences[i].length = 0;
    }
}

void play_macro_sequence(uint8_t macro_id)
{
    if (macro_id >= 9)
        return;

    hid_macro_sequence_t *sequence = &macro_sequences[macro_id];
    for (uint8_t i = 0; i < sequence->length; i++)
    {
        dequeue_hid_report(&sequence->report_sequence[i]);
    }
}

void write_macro_sequence(uint8_t macro_id, hid_macro_sequence_t *macro_sequence)
{
    if (macro_id >= 9)
        return;

    if (macro_sequence->length > MAX_SEQUENCE_LENGTH)
        macro_sequence->length = MAX_SEQUENCE_LENGTH;

    hid_macro_sequence_t *sequence = &macro_sequences[macro_id];

    sequence->length = macro_sequence->length;
    memcpy(sequence->report_sequence, macro_sequence->report_sequence, sizeof(macro_sequence->report_sequence));
}