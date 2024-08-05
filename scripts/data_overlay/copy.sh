#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="copy.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}common.sh"
assert_root



# Set working location to project root directory
print_step "Setting working location to project root directory"

if ! cd_to_project_root "${SCRIPT_DIR_PATH}"; then
    print_bold "Failed to set working location to project root directory.\n"
    
    print_exit_step_and_exit
fi



# Check if the .env file exists
print_step "Checking if the .env file exists"

assert_env_file_exists



# Load environment variables
print_step "Loading environment variables"

load_env_variables



# Purge data overlay
print_step "Purging data overlay"

purge_data_overlay



# Copy data overlay
print_step "Copying data overlay"

copy_data_overlay



# Finishing
print_finish_step_and_exit
