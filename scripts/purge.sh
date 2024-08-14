#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="purge.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}_common.sh"
assert_root



# Set working location to project root directory
print_step "Setting working location to project root directory"

if ! cd_to_project_root "${SCRIPT_DIR_PATH}"; then
    print_bold "Failed to set working location to project root directory.\n"
    
    print_exit_step_and_exit
fi



# Remove scons cache
print_step "Removing scons cache"

if ! find "./scooter-control/" | grep -E "(/__pycache__$|\.pyc$|\.pyo$)" | xargs rm -rf; then
    print_bold "Failed to remove scons cache.\n"

    print_exit_step_and_exit
fi



# Remove build directory
print_step "Removing build directory"

if ! rm -rf "./scooter-control/build/"; then
    print_bold "Failed to remove build directory.\n"

    print_exit_step_and_exit
fi



# Finishing
print_finish_step_and_exit
