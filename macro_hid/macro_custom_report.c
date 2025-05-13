#include "macro_custom_report.h"
#include "../macro_uart/macro_uart.h"

void excecute_host_command(hid_host_cmd_t *host_command)
{
    static hid_macro_sequence_t macro_sequence_buffer;

    if (host_command->command == CMD_CLEAR)
    {
        macro_sequence_buffer.length = 0;
        uart_send_string("Clear\r\n");
    }
    else if (host_command->command == CMD_ADD)
    {
        if (macro_sequence_buffer.length < MAX_SEQUENCE_LENGTH)
        {
            memcpy(&macro_sequence_buffer.report_sequence[macro_sequence_buffer.length],
                   host_command->data,
                   sizeof(hid_macro_report_t));
            macro_sequence_buffer.length++;
            uart_send_string("Add\r\n");
        }
    }
    else if (host_command->command == CMD_COMMIT)
    {
        uint8_t macro_id = host_command->data[0];
        write_macro_sequence(macro_id, &macro_sequence_buffer);
        uart_send_string("Commit\r\n");
    }
    return;
}