#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="launch_main.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}common.sh"
assert_root



# Set working location to project root directory
print_step "Setting working location to project root directory"

if ! cd_to_project_root "${SCRIPT_DIR_PATH}"; then
    print_bold "Failed to set working location to project root directory.\n"
    
    print_exit_step_and_exit
fi



# Launch the program
print_step "Launching the program"

if ! ./scooter-control/.venv/bin/python3 -m "scooter-control.main"; then
    print_bold "\nFailed to launch the program.\n"

    print_exit_step_and_exit
fi



# Finishing
print_finish_step_and_exit
