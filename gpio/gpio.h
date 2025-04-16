#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define COL_0 7
#define COL_1 8
#define COL_2 9
#define ROW_0 10
#define ROW_1 11
#define ROW_2 12
#define NUM_COLS 3
#define NUM_ROWS 3

void mount_led_flash(void);
void macropad_gpio_init(void);
int matrix_scan(void);

#endif