/**
 * @file macro_host_commands.h
 * @author Michal Rajzer
 * @brief Defines HID host commands and structures for the macropad firmware.
 * @version 0.1
 * @date 2025-10-08
 *
 * @details
 * This header defines the data structures and command constants for the host
 * command interface, which allows the host to reprogram macro sequences on
 * the macropad.
 */

#ifndef MACRO_HOST_COMMANDS_H
#define MACRO_HOST_COMMANDS_H

#include <string.h>
#include "../macro_core/macro_core_types.h"
#include "../macro_core/macro_flash.h"

/**
 * @brief Execute a host command on the macropad.
 *
 * @param host_command Pointer to the command structure to execute.
 *
 * @details
 * This function handles all supported HID host commands, such as clearing
 * the macro buffer, adding reports, committing to a slot, or writing
 * to flash memory.
 */
void execute_host_command(hid_host_cmd_t *host_command);

#endif // MACRO_HOST_COMMANDS_H
