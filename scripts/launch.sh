#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="launch.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}common.sh"
assert_root



# Set working location to project root directory
print_step "Setting working location to project root directory"

if ! cd_to_project_root "${SCRIPT_DIR_PATH}"; then
    print_bold "Failed to set working location to project root directory.\n"
    
    print_exit_step_and_exit
fi



# List available targets
print_step "Listing available targets"

targets=("Scooter Control")
while IFS= read -r file; do
    test_target_file_basename="$(basename "${file}")"
    targets+=("${test_target_file_basename:0:-3}")
done < <(find "./scooter-control/tests/" -maxdepth 1 -type f -name "*.py" 2>/dev/null)



# Launch the program
print_step "Launching the program" "1:2"

temp_file=$(mktemp)
exec 3> "${temp_file}"

select_option_with_abort "Select a target to launch" "${targets[@]}"
selected_target_index=$(<"$temp_file")

exec 3>&-

if [[ "${selected_target_index}" -eq 1 ]]; then
    target_path="scooter-control.main"
else
    target="${targets[$((selected_target_index - 1))]}"
    target_path="scooter-control.tests.${target}"
fi

printf "\n"

if ! ./scooter-control/.venv/bin/python3 -m "${target_path}"; then
    print_bold "\nFailed to launch the program.\n"

    print_exit_step_and_exit
fi



# Finishing
print_finish_step_and_exit
