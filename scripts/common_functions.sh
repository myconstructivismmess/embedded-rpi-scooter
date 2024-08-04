BOLD_START="\033[1m"
BOLD_END="\033[0m"
ENV_FILE_PATH_FROM_PROJECT_ROOT=".env"



assert_root() {
    if [[ $(/usr/bin/id -u) -ne 0 ]]; then
        print_bold "Please run this script as root.\n"
        exit 1
    fi
}



cd_to_project_root() {
    local script_dir_path="$1"
    cd "${script_dir_path}${RELATIVE_PATH_TO_PROJECT_ROOT}"
}



env_file_exists() {
    if [ ! -f ".env" ]; then
        print_bold "\nThe .env file does not exist.\n"

        print_exit_step_and_exit
    fi
}
load_env_variables() {
    # Check if the .env file exists (prevent problems if env_file_exists is not called)
    env_file_exists

    # Enable automatic export of all variables
    set -a

    # Source the .env file line by line
    while IFS= read -r line || [ -n "${line}" ]; do
    # Remove leading and trailing whitespaces
    line=$(echo "${line}" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')

    # Skip empty lines and lines starting with a comment
    if [[ -z "${line}" || "${line}" == \#* ]]; then
        continue
    fi

    # Remove comments after the variable definition
    line=$(echo "${line}" | sed 's/[[:space:]]*#.*//')

    # Extract key and value
    key=$(echo "${line}" | cut -d '=' -f 1)
    value=$(echo "${line}" | cut -d '=' -f 2-)

    # Remove leading and trailing whitespaces from key and value
    key=$(echo "${key}" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
    value=$(echo "${value}" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')

    # Remove surrounding quotes from value if they exist
    if [[ "${value}" == \"*\" && "${value}" == *\" ]]; then
        value=$(echo "${value}" | sed 's/^"//;s/"$//')
    elif [[ "${value}" == \'*\' && "${value}" == *\' ]]; then
        value=$(echo "${value}" | sed "s/^'//;s/'$//")
    fi

    # Add the prefix to the variable name
    new_key="ENV_PROPERTY_${key}"

    # Export the new variable with the same value
    export "${new_key}"="${value}"
    done < .env

    # Disable automatic export
    set +a
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



print_abort_step_and_exit() {
    print_step "Aborting" "1:2"
    exit 0
}
print_finish_step_and_exit() {
    print_step "Finishing" "1:2"
    exit 0
}
print_exit_step_and_exit() {
    print_step "Exiting" "1:2"
    exit 1
}



print_bold() {
    local text="$1"
    printf "${BOLD_START}%b${BOLD_END}" "$text"
}
