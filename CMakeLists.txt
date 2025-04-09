cmake_minimum_required(VERSION 3.13)

include(pico-sdk/pico_sdk_init.cmake)

project(macropad-firmware)
# Initialize the Pico SDK
pico_sdk_init()

add_executable(macropad-firmware
        uart/uart.c
        gpio/gpio.c
        main.c
        )

# pull in common dependencies
target_link_libraries(macropad-firmware pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(macropad-firmware)

