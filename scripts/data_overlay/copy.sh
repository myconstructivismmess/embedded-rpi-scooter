#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="copy.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}common.sh"
check_root



# Set working location to project root directory
print_step "Setting working location to project root directory"

if ! cd_to_project_root "${SCRIPT_DIR_PATH}"; then
    print_bold "Failed to set working location to project root directory.\n"
    
    print_step "Exiting" "1:2"
    exit 1
fi



# 