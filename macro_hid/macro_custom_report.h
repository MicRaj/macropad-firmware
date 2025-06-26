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

typedef struct __attribute__((packed))
{
    uint8_t command; // CMD_SET_INDEX, CMD_ADD_REPORT, etc.
    uint8_t data[7]; // Command-specific payload
} hid_host_cmd_t;

enum
{
    CMD_CLEAR,  // Clear the current working buffer (before adding new reports)
    CMD_ADD,    // Add a single hid_macro_report_t to the working buffer
    CMD_COMMIT, // Save the working buffer to the specified macro slot
    CMD_FLASH   // Save the current macro_store to flash
};

void excecute_host_command(hid_host_cmd_t *host_command);
#endif