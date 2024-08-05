#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="purge.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}common.sh"
assert_root



# Set working location to project root directory
print_step "Setting working location to project root directory"

if ! cd_to_project_root "${SCRIPT_DIR_PATH}"; then
    print_bold "Failed to set working location to project root directory.\n"
    
    print_exit_step_and_exit
fi



# Remove python 3 virtual environment
print_step "Removing python 3 virtual environment"

if ! rm -rf "./scooter-control/.venv/"; then
    print_bold "Failed to remove python 3 virtual environment.\n"

    print_exit_step_and_exit
fi



# Remove python 3 cache directories
print_step "Removing python 3 project cache"

if ! find "./scooter-control/" | grep -E "(/__pycache__$|\.pyc$|\.pyo$)" | xargs rm -rf; then
    print_bold "Failed to remove python 3 project cache.\n"

    print_exit_step_and_exit
fi



# Finishing
print_finish_step_and_exit
