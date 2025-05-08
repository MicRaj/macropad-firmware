#!/bin/bash

# Define paths
BUILD_DIR="/home/mrlap/Documents/Projects/macropad-project/macropad-firmware/build"
FIRMWARE_FILE="$BUILD_DIR/macropad-firmware.elf"
OPENOCD_CFG="/usr/share/openocd/scripts/interface/cmsis-dap.cfg"
TARGET_CFG="/usr/share/openocd/scripts/target/rp2040.cfg"

# Step 1: Build firmware
echo "Building firmware..."
cd "$BUILD_DIR" || { echo "Build directory not found!"; exit 1; }
cmake .. \
  -DCMAKE_C_COMPILER=/usr/bin/arm-none-eabi-gcc \
  -DCMAKE_CXX_COMPILER=/usr/bin/arm-none-eabi-g++ \
  -DCMAKE_ASM_COMPILER=/usr/bin/arm-none-eabi-gcc && \
make -j4 || { echo "Build failed!"; exit 1; }

# Step 2: Flash
if [[ ! -f "$FIRMWARE_FILE" ]]; then
  echo "ERROR: Binary file not found at $FIRMWARE_FILE"
  exit 1
fi

echo "Flashing $FIRMWARE_FILE to RP2040..."

openocd -f "$OPENOCD_CFG" -f "$TARGET_CFG" \
  -c "adapter speed 5000" \
  -c "program $FIRMWARE_FILE verify reset exit"
  
if [[ $? -eq 0 ]]; then
  echo "Flash successful."
else
  echo "Flash failed."
  exit 1
fi
