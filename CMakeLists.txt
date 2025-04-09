cmake_minimum_required(VERSION 3.13)

include(pico-sdk/pico_sdk_init.cmake)

project(macropad-firmware)
# Initialize the Pico SDK
pico_sdk_init()

add_executable(macropad-firmware
        uart/uart.c
        gpio/gpio.c
        hid/usb_descriptors.c
        hid/hid.c
        main.c
        )

# Make sure TinyUSB can find tusb_config.h
target_include_directories(macropad-firmware PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/hid)

# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_device
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_link_libraries(macropad-firmware PUBLIC pico_stdlib pico_unique_id tinyusb_device tinyusb_board)

# create map/bin/hex file etc.
pico_add_extra_outputs(macropad-firmware)

