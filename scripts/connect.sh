#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="connect.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}common.sh"
check_root



# Collecting tty devices
print_step "Collecting ttyUSB devices"

DEVICES=($(sudo ls "/dev/" | grep -o 'ttyUSB[0-9]*' || true))



# Finishing if no devices found
if [ -z "$DEVICES" ]; then
    print_bold "\nNo device found.\n"
    print_step "Finishing" "1:2"

    exit 0
fi



# Select the device
print_step "Connecting to a device" "1:2"

PS3="Select a device to connect to (or select 'Abort' to exit): "
while true; do
    select DEVICE in "${DEVICES[@]}" Abort; do
        if [[ $REPLY -eq $((${#DEVICES[@]} + 1)) ]]; then
            print_step "Aborting" "1:2"
            exit 0
        elif [[ $REPLY -lt 1 || $REPLY -gt $((${#DEVICES[@]} + 1)) ]]; then
            print_bold "Unknown choice \"$REPLY\"\n\n"
        else
            break 2
        fi
    done
done

if ! screen "/dev/${DEVICE}" 115200 -L; then
    print_bold "\nFailed to connect to device.\n"

    print_step "Exiting" "1:2"
    exit 1
fi



# Finishing
print_step "Finishing" "1:2"

exit 0
