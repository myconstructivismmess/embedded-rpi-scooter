#!/usr/bin/env bash

BOLD_START="\033[1m"
BOLD_END="\033[0m"
RELATIVE_PATH_TO_PROJECT_ROOT="../"

check_root() {
    if [[ $(/usr/bin/id -u) -ne 0 ]]; then
        print_bold "Please run this script as root.\n"
        exit 1
    fi
}

cd_to_project_root() {
    local script_dir_path="$1"
    cd "${script_dir_path}${RELATIVE_PATH_TO_PROJECT_ROOT}"
}

print_step() {
    local step_text="$1"
    local step_newline_format="$2"

    # Default values for newline counts
    local step_newline_count_before=1
    local step_newline_count_after=1

    # Check if the second parameter matches the format 0-infinite:0-infinite
    if [[ "$step_newline_format" =~ ^[0-9]+:[0-9]+$ ]]; then
        step_newline_count_before="${step_newline_format%%:*}"
        step_newline_count_after="${step_newline_format##*:}"
    fi

    # Initialize the step number if it's not already defined
    if [[ -z "${step_number+x}" ]]; then
        step_number=1
    fi

    # Create newlines based on counts without using external commands
    local newlines_before=""
    for ((i = 0; i < step_newline_count_before; i++)); do
        newlines_before+="\n"
    done
    local newlines_after=""
    for ((i = 0; i < step_newline_count_after; i++)); do
        newlines_after+="\n"
    done

    printf "%b${BOLD_START} %s. %s...${BOLD_END}%b" "$newlines_before" "$step_number" "$step_text" "$newlines_after"

    # Increment the step number
    ((step_number++))
}

print_bold() {
    local text="$1"
    printf "${BOLD_START}%b${BOLD_END}" "$text"
}