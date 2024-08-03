#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="launch.sh"
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



# List available targets
print_step "Listing available targets"

TARGETS=("Scooter Control")
while IFS= read -r FILE; do
    TEST_TARGET_FILE_BASENAME="$(basename "${FILE}")"
    TARGETS+=("${TEST_TARGET_FILE_BASENAME:0:-3}")
done < <(find "./scooter-control/test/" -maxdepth 1 -type f -name "*.py" 2>/dev/null)



# Launch the program
print_step "Launching the program" "1:2"

PS3="Select a target to launch (or select 'Abort' to exit): "

while true; do
    select TARGET in "${TARGETS[@]}" Abort; do
        if [[ $REPLY -eq $((${#TARGETS[@]} + 1)) ]]; then
            print_step "Aborting" "1:2"
            exit 0
        elif [[ $REPLY -lt 1 || $REPLY -gt $((${#TARGETS[@]} + 1)) ]]; then
            print_bold "Unknown choice \"${REPLY}\"\n\n"
        else
            break 2
        fi
    done
done

if [[ "${REPLY}" -eq 1 ]]; then
    TARGET_PATH="scooter-control/main.py"
else
    TARGET_PATH="scooter-control/test/${TARGET}.py"
fi

printf "\n"

if ! ./scooter-control/.venv/bin/python3 "${TARGET_PATH}"; then
    print_bold "\nFailed to launch the program.\n"

    print_step "Exiting" "1:2"
    exit 1
fi



# Finishing
print_step "Finishing" "1:2"

exit 0
