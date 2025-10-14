/**
 * @file mainpage.h
 * @brief Main page for Macropad firmware documentation.
 *
 * @mainpage Macropad Firmware Documentation
 *
 * Welcome to the documentation for the **Macropad firmware**.
 *
 * ## Overview
 * This project implements a 3x3 macro pad using the RP2040 microcontroller.
 *
 * Key features:
 * - Core functionality: macros, macro sequences, and macro store
 * - HID report handling via TinyUSB
 * - GPIO scanning for the key matrix
 * - Flash storage for macros
 * - USB descriptors for device enumeration
 *
 * ## Project Structure
 * - `macro_core/` - core macro logic and flash interface.
 * - `macro_hid/`  - HID handling and report queues
 * - `macro_gpio/` - GPIO initialization and matrix scanning
 * - `macro_uart/` - UART communication (debugging)
 *
 * ## References
 * - See `README.md` for setup, building, and usage instructions
 * - Refer to individual headers for detailed function documentation
 */
