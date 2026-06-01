#!/bin/bash

# Define paths
BUILD_DIR="/c/Users/mrajz/Documents/personal-projects/macropad-project/macropad-firmware/build"
FIRMWARE_FILE="$BUILD_DIR/macropad-firmware.elf"
OPENOCD_CFG=/mingw64/share/openocd/scripts/interface/cmsis-dap.cfg
TARGET_CFG=/mingw64/share/openocd/scripts/target/rp2040_modified.cfg



# Step 1: Build firmware
echo "Building firmware..."
cd "$BUILD_DIR" || { echo "Build directory not found!"; exit 1; }
cmake .. \
  -DPICO_SDK_PATH=../pico-sdk \
  -DPICO_BOARD=pico \
  -G "Unix Makefiles"
make -j4 || { echo "Build failed!"; exit 1; }

# Step 2: Flash
if [[ ! -f "$FIRMWARE_FILE" ]]; then
  echo "ERROR: Binary file not found at $FIRMWARE_FILE"
  exit 1
fi

echo "Flashing $FIRMWARE_FILE to RP2040..."

cd "$BUILD_DIR"
openocd -f "$OPENOCD_CFG" -f "$TARGET_CFG" \
  -c "reset_config srst_only srst_nogate" \
  -c "transport select swd" \
  -c "adapter speed 5000" \
  -c "program macropad-firmware.elf verify reset exit"
  
if [[ $? -eq 0 ]]; then
  echo "Flash successful."
else
  echo "Flash failed."
  exit 1
fi
