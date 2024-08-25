#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="build-audioscootercontrold.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}_common.sh"
assert_root



# Set working location to project root directory
print_step "Setting working location to project directory"

if ! cd_to_project_root "${SCRIPT_DIR_PATH}"; then
    print_bold "Failed to set working location to project root directory.\n"
    
    print_exit_step_and_exit
fi

if ! cd "./scooter-control/"; then
    print_bold "Failed to set working location to project directory.\n"
    
    print_exit_step_and_exit
fi



# Building the project
print_step "Building the project" "1:2"

if ! mkdir -p "./build/"; then
    print_bold "Failed to make the build directory.\n"

    print_exit_step_and_exit
fi

if ! cd "./build/"; then
    print_bold "Failed to change directory to './build/'.\n"
fi

if ! cmake ..; then
    print_bold "Failed to configure the project with CMake.\n"
    
    print_exit_step_and_exit
fi

if ! cmake --build "./" --target "audioscootercontrold"; then
    print_bold "Failed to build the project.\n"

    print_exit_step_and_exit
fi



# Finishing
print_finish_step_and_exit
