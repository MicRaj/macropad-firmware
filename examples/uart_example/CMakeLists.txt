cmake_minimum_required(VERSION 3.13)

include(../pico-sdk/pico_sdk_init.cmake)

project(my_project)
# Initialize the Pico SDK
pico_sdk_init()

add_executable(hello_uart
        hello_uart.c
        )

# pull in common dependencies
target_link_libraries(hello_uart pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(hello_uart)

# add url via pico_set_program_url
# example_auto_set_url(hello_uart)
