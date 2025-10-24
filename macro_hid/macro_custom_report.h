/**
 * @file macro_custom_report.h
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

#ifndef CUSTOM_H
#define CUSTOM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <bsp/board_api.h>
#include <tusb.h>
#include <hid.h>

#include "usb_descriptors.h"
#include "tusb_config.h"
#include "../macro_core/macro_core.h"

/**
 * @brief Structure representing a HID host command.
 *
 * Each command contains a command type and a 7-byte payload
 * used for command-specific data.
 */
typedef struct __attribute__((packed)) // Needs packing for memcpy.
{
    uint8_t command; /**< Command type (CMD_CLEAR, CMD_ADD, etc.) */
    uint8_t data[7]; /**< Command-specific payload */
} hid_host_cmd_t;

/**
 * @brief HID host command types
 */
enum
{
    CMD_CLEAR,  /**< Clear the current working buffer */
    CMD_ADD,    /**< Add a single hid_macro_report_t to the working buffer */
    CMD_COMMIT, /**< Save the working buffer to the specified macro slot */
    CMD_FLASH   /**< Save the current macro store to flash */
};

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

#endif // CUSTOM_H
