#ifndef MACRO_HID_TYPES_H
#define MACRO_HID_TYPES_H

#include <stdint.h>

/** Helper macro to define a HID report */
#define HID_REPORT(mod, k0, k1, k2, k3, k4, k5) \
    {                                           \
        .modifier = (mod), .keycode = {(k0),    \
                                       (k1),    \
                                       (k2),    \
                                       (k3),    \
                                       (k4),    \
                                       (k5) }, \
        .delay_ms = 0 \
    }

#define HID_REPORT_DELAY(mod, k0, k1, k2, k3, k4, k5, delay) \
    {                                           \
        .modifier = (mod), .keycode = {(k0),    \
                                       (k1),    \
                                       (k2),    \
                                       (k3),    \
                                       (k4),    \
                                       (k5) }, \
        .delay_ms = (delay) \
    }


/**
 * @brief HID keyboard report structure (Report ID and reserved byte removed).
 */
typedef struct __attribute__((packed)) // Packing for memcpy.
{
    uint8_t modifier;     /**< Modifier key bitmask (Ctrl, Shift, Alt, GUI). */
    uint8_t keycode[6];   /**< Array of up to six simultaneous key codes. */
    uint16_t delay_ms;  /**< Delay after this report (not sent to host). */
} hid_macro_report_t;

/**
 * @brief HID host command structure.
 *
 * Each command contains a command type and a 9-byte payload
 * used for command-specific data.
 */
typedef struct __attribute__((packed)) { // Needs packing for memcpy.
    uint8_t command; /**< Command type (CMD_CLEAR, CMD_ADD, etc.) */
    uint8_t data[9]; /**< Command-specific payload */
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
#endif // MACRO_HID_TYPES_H