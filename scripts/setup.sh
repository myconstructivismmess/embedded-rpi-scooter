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

while IFS= read -r -d $'\0' file; do
    path_in_script_dir="${file#${SCRIPT_DIR_PATH}}"
	if chmod +x "${file}"; then
        print_bold "Added executable permission to file '${path_in_script_dir}'.\n"
    else
        print_bold "Failed to add executable permission to file '${path_in_script_dir}'.\n"
        
        print_exit_step_and_exit
    fi
done < <(find "${SCRIPT_DIR_PATH}" -type f -name "*.sh" -print0)



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

temp_file=$(mktemp)
exec 3> "${temp_file}"

python3_package_lists=("Development environment package list" "Board environment package list")
select_option_with_abort "Select a python 3 package list to install" "${python3_package_lists[@]}"
selected_package_list_index=$(<"$temp_file")

exec 3>&-

printf "\n"

if [ "${selected_package_list_index}" -eq "1" ]; then
    packages_file_path="$(realpath "${SCRIPT_DIR_PATH}")/data/python_package_lists/development_environment_requirements.txt"
    error_message="Failed to install development environment packages."
elif [ "${selected_package_list_index}" -eq "2" ]; then
    packages_file_path="$(realpath "${SCRIPT_DIR_PATH}")/data/python_package_lists/board_environment_requirements.txt"
    error_message="Failed to install board environment packages."
fi

if ! ./scooter-control/.venv/bin/pip3 install -r "${packages_file_path}"; then
    print_bold "\n${error_message}\n"

    print_exit_step_and_exit
fi



# Finishing
print_finish_step_and_exit
