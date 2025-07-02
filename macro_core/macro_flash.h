#ifndef FLASH_H
#define FLASH_H
#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/flash.h"
#include "hardware/flash.h"
#include "macro_core.h"

#define FLASH_SECTOR_NUMBER 4
#define FLASH_WRITE_START (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_NUMBER * FLASH_SECTOR_SIZE)
#define FLASH_READ_START (FLASH_WRITE_START + XIP_BASE)

void macropad_flash_init();
int write_store_to_flash(const hid_macro_store_t *macro_store);
void read_store_from_flash(hid_macro_store_t *macro_store);
#endif