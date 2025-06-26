#include "macro_flash.h"

extern hid_macro_store_t macro_store;

void macropad_flash_init() // Placeholder
{
    read_store_from_flash(&macro_store);
}
// This function will be called when it's safe to call flash_range_erase
static void call_flash_range_erase(void *param)
{
    uint32_t offset = (uint32_t)param;
    flash_range_erase(offset, FLASH_SECTOR_SIZE);
}

// This function will be called when it's safe to call flash_range_program
static void call_flash_range_program(void *param)
{
    uint32_t offset = ((uintptr_t *)param)[0];
    const uint8_t *data = (const uint8_t *)((uintptr_t *)param)[1];
    flash_range_program(offset, data, FLASH_PAGE_SIZE);
}

int write_flash_page(uint8_t *base_write_address, uint8_t page_data[FLASH_PAGE_SIZE])
{
    uintptr_t params[] = {(unsigned int)base_write_address, (uintptr_t)page_data};
    int rc = flash_safe_execute(call_flash_range_program, params, UINT32_MAX);
    if (rc != PICO_OK)
        return rc;

    // Check for valid flash
    uint8_t *base_read_address = (uint8_t *)(base_write_address + XIP_BASE);
    bool mismatch = false;
    for (uint i = 0; i < FLASH_PAGE_SIZE; i++)
    {
        if (page_data[i] != base_read_address[i])
            mismatch = true;
        break;
    }
    if (mismatch)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int write_store_to_flash(const hid_macro_store_t *macro_store)
{
    int rc = flash_safe_execute(call_flash_range_erase, (void *)FLASH_WRITE_START, UINT32_MAX);
    if (rc != PICO_OK)
        return rc;

    // Find number of pages (ceiling divide)
    int size = sizeof(*macro_store);
    int pages = (size + FLASH_PAGE_SIZE - 1) / 256;

    // Pointer to the data as bytes
    const uint8_t *data_ptr = (const uint8_t *)macro_store;

    // Write each page
    static uint8_t buffer[FLASH_PAGE_SIZE];
    for (int i = 0; i < pages; i++)
    {
        memset(buffer, 0, FLASH_PAGE_SIZE);
        // Copy up to 256 bytes into the buffer
        int remaining = size - i * FLASH_PAGE_SIZE;
        int chunk_size = remaining > FLASH_PAGE_SIZE ? FLASH_PAGE_SIZE : remaining;
        memcpy(buffer, data_ptr + i * FLASH_PAGE_SIZE, chunk_size);
        // Write the buffer to flash
        rc = write_flash_page((uint8_t *)(FLASH_WRITE_START + i * FLASH_PAGE_SIZE), buffer);
        if (rc != PICO_OK)
            return rc;
    }
    return PICO_OK;
}

void read_store_from_flash(hid_macro_store_t *macro_store)
{
    const hid_macro_store_t *flash_macro_store = (const hid_macro_store_t *)(FLASH_READ_START);
    memcpy(macro_store, flash_macro_store, sizeof(*macro_store));
}

void write_current_macro_store_to_flash()
{
    int rc = write_store_to_flash(&macro_store);
    if (rc == PICO_OK)
    {
        uart_send_string("Flash Successful\r\n");
    }
    else
    {
        uart_send_string("Flash Error\r\n");
    }
}