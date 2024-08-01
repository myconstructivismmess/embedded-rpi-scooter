#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="purge.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}common.sh"
check_root



# Set working location to project root directory
print_step "Setting working location to project root directory"

cd_to_project_root "$SCRIPT_DIR_PATH"



# Remove python 3 virtual environment
print_step "Removing python 3 virtual environment"

sudo rm -rf ./scooter-control/.venv/



# Remove python 3 cache directories
print_step "Removing python 3 cache"

find ./scooter-control/ | grep -E "(/__pycache__$|\.pyc$|\.pyo$)" | xargs rm -rf



# Finishing
print_step "Finishing" "1:2"
