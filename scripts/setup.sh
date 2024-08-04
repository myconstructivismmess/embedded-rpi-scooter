#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="setup.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}common.sh"
assert_root



# Install packages
print_step "Installing packages" "1:2"

packages_file_path="${SCRIPT_DIR_PATH}data/apt-package-list.txt"
if [ -s "${packages_file_path}" ]; then
    packages_file_content=$(<"${packages_file_path}")
    packages="${packages_file_content//$'\n'/ }"
    IFS=' '
    for package in $packages; do
        if apt install -y "${package}"; then
            print_bold "\n\"${package}\" installed successfully or already installed.\n\n"
        else
            print_bold "\nFailed to install \"${package}\".\n"

            print_exit_step_and_exit
        fi
    done
else
    print_bold "\nNo package to install.\n"
    
    print_exit_step_and_exit
fi



# Make every other scripts in this directory executable
print_step "Making other scripts executable" "0:2"

find "${SCRIPT_DIR_PATH}" -type f -name "*.sh" | while IFS= read -r file; do
	if chmod +x "${file}"; then
        print_bold "Added executable permission to file \"$(basename "${file}")\".\n"
    else
        print_bold "Failed to add executable permission to file \"$(basename "${file}")\".\n"
        
        print_exit_step_and_exit
    fi
done



# Set working location to project root directory
print_step "Setting working location to project root directory"

if ! cd_to_project_root "${SCRIPT_DIR_PATH}"; then
    print_bold "Failed to set working location to project root directory.\n"
    
    print_exit_step_and_exit
fi



# Create python 3 virtual environment
print_step "Creating python 3 virtual environment"

if ! python3 -m venv "./scooter-control/.venv/"; then
    print_bold "Failed to create Python 3 virtual environment.\n"

    print_exit_step_and_exit
fi



# Install python 3 packages
print_step "Installing python 3 packages" "1:2"

PS3="Select a python 3 package list to install (or select 'Abort' to exit): "
python3_package_lists=("Development environment package list" "Board environment package list")

while true; do
    select item in "${python3_package_lists[@]}" Abort; do
        case $REPLY in
            1) break 2;;
            2) break 2;;
            $((${#items[@]}+1))) print_abort_step_and_exit;;
            *) print_bold "Unknown choice \"${REPLY}\"\n\n"; break;
        esac
    done
done

printf "\n"

if [ "$REPLY" -eq "1" ]; then
    packages_file_path="$(realpath "${SCRIPT_DIR_PATH}")/data/python_package_lists/development_environment_requirements.txt"
    error_message="Failed to install development environment packages."
elif [ "$REPLY" -eq "2" ]; then
    packages_file_path="$(realpath "${SCRIPT_DIR_PATH}")/data/python_package_lists/board_environment_requirements.txt"
    error_message="Failed to install board environment packages."
fi

if ! ./scooter-control/.venv/bin/pip3 install -r "${packages_file_path}"; then
    print_bold "\n${error_message}\n"

    print_exit_step_and_exit
fi



# Finishing
print_finish_step_and_exit
