#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="setup.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}_common.sh"
assert_root



# Install packages
print_step "Installing apt packages" "1:2"

packages_file_path="${SCRIPT_DIR_PATH}data/apt-package-list.txt"
if [ -f "${packages_file_path}" ]; then
    # Process the apt packages list file
    while IFS= read -r package; do
        if apt install -y "${package}"; then
            print_bold "\nPackage '${package}' installed successfully or already installed.\n\n"
        else
            print_bold "\nFailed to install package '${package}'.\n"

            print_exit_step_and_exit
        fi
    done < <(read_file_without_comments "${packages_file_path}")
else
    print_bold "\nThe apt packages list file does not exist.\n"
    
    print_exit_step_and_exit
fi



# Make every other scripts in this directory executable
print_step "Making other scripts executable" "0:2"

while IFS= read -r -d $'\0' file; do
    path_in_script_dir="${file#${SCRIPT_DIR_PATH}}"
	if chmod +x "${file}"; then
        # Print the name of the file that was made executable if it doesn't start with an underscore
        script_file_name="$(basename "${path_in_script_dir}")"
        if [[ "${script_file_name}" != _* ]]; then
            print_bold "File '${path_in_script_dir}' is now executable.\n"
        fi
    else
        print_bold "Failed to add executable permission to file '${path_in_script_dir}'.\n"
        
        print_exit_step_and_exit
    fi
done < <(find "${SCRIPT_DIR_PATH}" -type f -name "*.sh" -print0)



# Finishing
print_finish_step_and_exit
