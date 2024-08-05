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



# Purge data overlay
print_step "Purging data overlay"

purge_data_overlay



# Finishing
print_finish_step_and_exit
