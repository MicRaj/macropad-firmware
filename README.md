# macropad-firmware

Firmware for the macropad.

## Prerequisites

- Install CMake (at least version 3.13), python 3, a native compiler, and a GCC cross compiler
  ```
  sudo apt install cmake python3 build-essential gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
  ```

# Building and flashing firmware

- Change directory to the build directory of the code/example you are trying to flash
  ```
  cd blink_example/build
  ```
- Run CMake.
  ```
  cmake ..
  ```
- Run Make (-j4 argument allows for four parallel jobs)
  ```
  make -j4
  ```
- The build should produce two files which can be used to flash the microcontroller.
  - **.elf** - Debugging and flashing with debug probe and OpenOCD
  - **.uf2** - Flashing using bootsel button.
- Flash the Raspberry Pico.
  - **Bootselect Button** - Holding the bootsel button while connecting to a computer will cause the pico to mount as a USB mass storage device, the uf2 file can then be copied into this new drive to flash the device.
  - **Debug Probe** - (coming soons)

# Debugging
- Open a terminal and run:
  ```
  sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000"
  ```
- In another terminal run:
  ```
  gdb-multiarch ./build/macropad-firmware.elf
  ```
- Connect GDB to OpenOCD
  ```
  target extended-remote :3333
  ```