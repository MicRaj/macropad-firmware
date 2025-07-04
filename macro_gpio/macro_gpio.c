#include "macro_gpio.h"

//--------------------------------------------------------------------+
// Macropad GPIO - including matrix scan operation
//--------------------------------------------------------------------+

int cols[NUM_COLS] = {COL_0, COL_1, COL_2};
int rows[NUM_ROWS] = {ROW_0, ROW_1, ROW_2};

void mount_led_flash(void)
{
    // Flash the mount LED
    for (int i = 0; i < 3; i++)
    {
        board_led_write(true);
        sleep_ms(100);
        board_led_write(false);
        sleep_ms(100);
    }
}

void macropad_gpio_init(void)
{
    // Initialise GPIO
    for (int i = 0; i < NUM_COLS; i++)
    {
        gpio_init(cols[i]);
        gpio_set_dir(cols[i], GPIO_OUT);
    }
    for (int i = 0; i < NUM_ROWS; i++)
    {
        gpio_init(rows[i]);
        gpio_set_dir(rows[i], GPIO_IN);
        gpio_pull_down(rows[i]);
    }
}

int matrix_scan(void)
{
    for (int col_idx = 0; col_idx < NUM_COLS; col_idx++)
    {
        for (int i = 0; i < NUM_COLS; i++)
        {
            gpio_put(cols[i], 0);
        }

        gpio_put(cols[col_idx], 1);
        sleep_ms(5);
        for (int row_idx = 0; row_idx < NUM_ROWS; row_idx++)
        {
            if (gpio_get(rows[row_idx]))
            {
                // Delay to debounce
                sleep_ms(20);
                if (!gpio_get(rows[row_idx]))
                {
                    continue;
                }
                int key_idx = col_idx * NUM_ROWS + row_idx;
                return key_idx;
            }
        }
    }
    return -1;
}
