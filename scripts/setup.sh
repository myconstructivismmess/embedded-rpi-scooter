#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="setup.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}common.sh"
check_root



# Install packages
print_step "Installing packages" "1:2"

PACKAGES_FILE_CONTENT="$(<"${SCRIPT_DIR_PATH}data/apt-package-list.txt")"
PACKAGES="${PACKAGES_FILE_CONTENT//$'\n'/ }"
IFS=' '
for package in $PACKAGES; do
  if apt install -y "$package"; then
    print_bold "\n\"${package}\" installed successfully or already installed.\n\n"
  else
    print_bold "\nFailed to install \"${package}\".\n"

    print_step "Exiting" "1:2"
    exit 1
  fi
done



# Make every other scripts in this directory executable
print_step "Making other scripts executable" "0:2"

find "$SCRIPT_DIR_PATH" -maxdepth 1 -type f -name "*.sh" | while IFS= read -r file; do
	chmod +x "$file"
    print_bold "Added executable permission to file \"$(basename "$file")\".\n"
done



# Set working location to project root directory
print_step "Setting working location to project root directory"

cd_to_project_root "$SCRIPT_DIR_PATH"



# Create python 3 virtual environment
print_step "Creating python 3 virtual environment"

python3 -m venv "./scooter-control/.venv/"



# Install python 3 packages
print_step "Installing python 3 packages" "1:2"

PS3="Select a python 3 package list to install (or select 'Abort' to exit): "
PYTHON3_PACKAGE_LISTS=("Development environment package list" "Board environment package list")

while true; do
    select item in "${PYTHON3_PACKAGE_LISTS[@]}" Abort; do
        case $REPLY in
            1) break 2;;
            2) break 2;;
            $((${#items[@]}+1))) print_step "Aborting" "1:2"; exit 0;;
            *) print_bold "Unknown choice \"$REPLY\"\n\n"; break;
        esac
    done
done

printf "\n"

if [ "$REPLY" -eq "1" ]; then
    sudo ./scooter-control/.venv/bin/pip3 install -r "$(realpath "$SCRIPT_DIR_PATH")/data/python_package_lists/development_environment_requirements.txt"
elif [ "$REPLY" -eq "2" ]; then
    sudo ./scooter-control/.venv/bin/pip3 install -r "$(realpath "$SCRIPT_DIR_PATH")/data/python_package_lists/board_environment_requirements.txt"
fi



# Finishing
print_step "Finishing" "1:2"

exit 0
