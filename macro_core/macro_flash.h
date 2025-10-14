/**
 * @file macro_flash.h
 * @brief Flash memory interface for storing macropad macros.
 * @author Michal Rajzer
 *
 * Provides functions to initialize flash, read and write the macro store,
 * and manage flash memory for persistent macro storage.
 *
 * @note Currently uses a fixed flash sector range. Avoid overlapping firmware.
 */

#ifndef FLASH_H
#define FLASH_H

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/flash.h"
#include "hardware/flash.h"
#include "macro_core.h"

#define FLASH_SECTOR_NUMBER 4 /**< Number of flash sectors reserved for macro storage */ //TODO remove magic number
#define FLASH_WRITE_START (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_NUMBER * FLASH_SECTOR_SIZE) /**< Start address for writing */
#define FLASH_READ_START (FLASH_WRITE_START + XIP_BASE) /**< Read address in memory-mapped space */

/**
 * @brief Initialize flash subsystem for macro storage.
 */
void macropad_flash_init(void);

/**
 * @brief Write the given macro store to flash memory.
 * @param macro_store Pointer to the macro store to persist
 * @return PICO_OK on success, or error code on failure
 */
int write_store_to_flash(const hid_macro_store_t *macro_store);

/**
 * @brief Read the macro store from flash memory into RAM.
 * @param[out] macro_store Pointer to the macro store structure to fill
 * 
 * @note Loading this when the flash contents are from an incompatible
 *       version or unknown state may produce invalid data.
 */
void read_store_from_flash(hid_macro_store_t *macro_store);

#endif /* FLASH_H */