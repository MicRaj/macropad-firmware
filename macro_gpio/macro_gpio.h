/**
 * @file macro_gpio.h
 * @brief GPIO interface for macropad keys and status LED.
 * @author Michal Rajzer
 *
 * Defines row and column pins, initialises GPIOs, scans the key matrix,
 * and provides a function to flash the mount LED.
 */
#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <hid.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <bsp/board_api.h>

#define COL_0 7
#define COL_1 8
#define COL_2 9
#define ROW_0 10
#define ROW_1 11
#define ROW_2 12
#define NUM_COLS 3
#define NUM_ROWS 3

/**
 * @brief Flash the mount LED three times.
 *
 * Uses the board LED to indicate that the macropad has been initialised.
 */
void mount_led_flash(void);

/**
 * @brief Initialize GPIO pins for the macropad.
 *
 * Sets column pins as outputs and row pins as inputs with pull-downs.
 */
void macropad_gpio_init(void);

/**
 * @brief Scan the key matrix for pressed keys.
 *
 * Iterates over columns and reads rows to detect a key press.
 * Implements basic debounce delay.
 *
 * @return Index of the pressed key id 0-8, or -1 if no key is pressed.
 */
int matrix_scan(void);

#endif