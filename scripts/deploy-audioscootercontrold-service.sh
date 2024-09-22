#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="deploy-audioscootercontrold-service.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}_common.sh"
assert_root



# Set working location to project root directory
print_step "Setting working location to project directory"

if ! cd_to_project_root "${SCRIPT_DIR_PATH}"; then
    print_bold "Failed to set working location to project root directory.\n"
    
    print_exit_step_and_exit
fi



# Creating the user if it does not exist
print_step "Creating the user if it does not exist"

if ! id -u scootercontrol &>/dev/null; then
    if ! useradd --system --no-create-home scootercontrol; then
        print_bold "Failed to create scootercontrol user.\n"
        
        print_exit_step_and_exit
    fi
fi



# Copying the executable
print_step "Copying the executable to binary directory"

if [ ! -x "./scooter-control/build/daemons/audioscootercontrold/audioscootercontrold" ]; then
    print_bold "The audioscootercontrold executable doesn't exist. Compile it before re-deploying service.\n"

    print_exit_step_and_exit
fi

if ! cp "./scooter-control/build/daemons/audioscootercontrold/audioscootercontrold" "/usr/bin/audioscootercontrold"; then
    print_bold "Failed to copy audioscootercontrold executable.\n"
    
    print_exit_step_and_exit
fi



# Setting up the service
print_step "Setting up the service"

if ! cp "./services/audioscootercontrold.service" "/etc/systemd/system/audioscootercontrold.service"; then
    print_bold "Failed to copy audioscootercontrold service file.\n"
    
    print_exit_step_and_exit
fi



# Stop the service
print_step "Stopping the service if it's running"

if systemctl is-active --quiet audioscootercontrold; then
    if ! systemctl stop audioscootercontrold; then
        print_bold "Failed to stop audioscootercontrold service.\n"
        
        print_exit_step_and_exit
    fi
else
    print_bold "audioscootercontrold service is not running.\n"
fi



# Reloading the services
print_step "Reloading the services"

if ! systemctl daemon-reload; then
    print_bold "Failed to reload the services.\n"
    
    print_exit_step_and_exit
fi



# Enabling the service
print_step "Enabling the service"

if ! systemctl enable audioscootercontrold; then
    print_bold "Failed to enable audioscootercontrold service.\n"
    
    print_exit_step_and_exit
fi



# Starting the service
print_step "Starting the service"

if ! systemctl start audioscootercontrold; then
    print_bold "Failed to start audioscootercontrold service.\n"
    
    print_exit_step_and_exit
fi



# Finishing
print_finish_step_and_exit
