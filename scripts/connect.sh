#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="connect.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}common.sh"
assert_root



# Collecting tty devices
print_step "Collecting ttyUSB devices"

devices=($(ls "/dev/" | grep -o 'ttyUSB[0-9]*' || true))



# Finishing if no devices found
if [ -z "${devices}" ]; then
    print_bold "\nNo device found.\n"
    
    print_finish_step_and_exit
fi



# Select the device
print_step "Connecting to a device" "1:2"

temp_file=$(mktemp)
exec 3> "${temp_file}"

select_option_with_abort "Select a device to connect to" "${devices[@]}"
selected_device_index=$(<"$temp_file")

exec 3>&-

device="${devices[$((selected_device_index - 1))]}"

if ! screen "/dev/${device}" 115200 -L; then
    print_bold "\nFailed to connect to device.\n"

    print_exit_step_and_exit
fi



# Finishing
print_finish_step_and_exit
