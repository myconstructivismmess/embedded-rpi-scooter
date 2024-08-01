#!/usr/bin/env bash
set -e

BOLD_START="\033[1m"
BOLD_END="\033[0m"
RELATIVE_PATH_FROM_SCRIPT_DIR="../"

SCRIPT_DIR_PATH="${BASH_SOURCE:0:-8}"
cd "$SCRIPT_DIR_PATH"

# Install packages
printf "\n${BOLD_START} 1. Installing packages...${BOLD_END}\n\n"

PACKAGES_FILE_CONTENT="$(<"./data/apt-package-list.txt")"
PACKAGES="${PACKAGES_FILE_CONTENT//$'\n'/ }"
IFS=' '
for package in $PACKAGES; do
  if sudo apt install -y "$package"; then
    printf "\n${BOLD_START}${package} installed successfully.${BOLD_END}\n\n"
  else
    printf "\n${BOLD_START}Failed to install ${package}.${BOLD_END}\n\n"
  fi
done

# Make every other scripts in this directory executable
printf "${BOLD_START} 2. Making every other scripts in this directory executable...${BOLD_END}\n\n"

find . -maxdepth 1 -type f -name "*.sh" | while IFS= read -r file; do
	chmod +x "$file"
	printf "${BOLD_START}Added executable permission to file \"$file\"${BOLD_END}\n"
done

# Set working location to the project root directory
printf "\n${BOLD_START} 3. Setting script working location...${BOLD_END}\n"

cd "${RELATIVE_PATH_FROM_SCRIPT_DIR}"

# Create python 3 virtual environment
printf "\n${BOLD_START} 4. Creating python 3 virtual environment...${BOLD_END}\n"

python3 -m venv ./scooter-control/.venv/

# Install python 3 packages
printf "\n${BOLD_START} 5. Installing python 3 packages...${BOLD_END}\n\n"

PS3="Select a python 3 package list to install: "
items=("Development environment package list" "Board environment package list")

while true; do
    select item in "${items[@]}" Abort
    do
        case $REPLY in
            1) break 2;;
            2) break 2;;
            $((${#items[@]}+1))) printf "\n${BOLD_START} 5. Aborting...${BOLD_END}\n\n"; exit 0;;
            *) printf "Unknown choice \"$REPLY\"\n\n"; break;
        esac
    done
done

printf "\n"

if [ "$REPLY" -eq "1" ]; then
    sudo ./scooter-control/.venv/bin/pip3 install -r "$(realpath $SCRIPT_DIR_PATH)/data/python_package_lists/development_environment_requirements.txt"
elif [ "$REPLY" -eq "2" ]; then
    sudo ./scooter-control/.venv/bin/pip3 install -r "$(realpath $SCRIPT_DIR_PATH)/data/python_package_lists/board_environment_requirements.txt"
fi

# Finishing
printf "\n${BOLD_START} 6. Finishing...${BOLD_END}\n\n"