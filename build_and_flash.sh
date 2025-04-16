#!/bin/bash

# Define paths
BUILD_DIR="/home/mrlap/Documents/Projects/macropad-project/macropad-firmware/build"
FIRMWARE_FILE="macropad-firmware.uf2"
MOUNT_POINT="/media/mrlap/RPI-RP2"

# Step 1: Build firmware
echo "Building firmware..."
cd "$BUILD_DIR" || { echo "Build directory not found!"; exit 1; }
cmake .. && make -j4 || { echo "Build failed!"; exit 1; }

# Step 2: Check for mounted RPi-RP2
echo "Checking for RPi-RP2 mount at $MOUNT_POINT..."
if [[ -d "$MOUNT_POINT" ]]; then
    echo "RPi-RP2 is mounted. Copying firmware..."
    cp "$FIRMWARE_FILE" "$MOUNT_POINT" && echo "Firmware copied successfully!"
else
    echo "RPi-RP2 not mounted at $MOUNT_POINT."
fi

cd ..