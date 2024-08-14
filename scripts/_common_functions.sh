BOLD_START="\033[1m"
BOLD_END="\033[0m"
ENV_FILE_PATH_FROM_PROJECT_ROOT=".env"



# Ensures the script is run with root privileges.
# Output: Prints an error message and exits if the script is not run with root privileges.
assert_root() {
    if [[ $(/usr/bin/id -u) -ne 0 ]]; then
        print_bold "Please run this script as root.\n"
        exit 1
    fi
}



# Changes the current directory to the project's root directory based on a relative path.
# Arguments:
#   $1 - The current script directory path.
# Output: Changes the current working directory to the project's root directory.
cd_to_project_root() {
    local script_dir_path="$1"
    
    cd "${script_dir_path}${RELATIVE_PATH_TO_PROJECT_ROOT}"
}



# Reads a file, removes lines that start with comments and strips out comments after useful content, then trims whitespace.
# Arguments:
#   $1 - Path to the file to be read.
# Output: Prints lines without comments and with trimmed whitespace.
read_file_without_comments() {
    local file_path="$1"

    local line
    while IFS= read -r line || [ -n "${line}" ]; do
        # Remove leading and trailing whitespace
        line="$(echo "${line}" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')"
        # Remove comments and trim again in case there was a comment
        line="$(echo "${line}" | sed 's/#.*$//;s/[[:space:]]*$//')"
        # Skip the line if it is empty or a comment
        if [[ -n "${line}" ]]; then
            echo "${line}"
        fi
    done < "${file_path}"
}



# Checks if the .env file exists in the current directory.
# Output: Prints an error message and exits if the .env file is not found.
assert_env_file_exists() {
    if [ ! -f ".env" ]; then
        print_bold "\nThe .env file does not exist.\n"

        print_exit_step_and_exit
    fi
}
# Loads environment variables from a .env file, ensuring they are exported with a specific prefix.
# Output: Exports environment variables defined in the .env file with a prefix "ENV_PROPERTY_".
load_env_variables() {
    # Check if the .env file exists (prevent problems if env_file_exists is not called)
    assert_env_file_exists

    # Enable automatic export of all variables
    set -a

    # Process the .env file using read_file_without_comments
    local line
    while IFS= read -r line; do
        # Skip lines that are empty
        if [[ -z "${line}" ]]; then
            continue
        fi

        # Extract key and value
        local key=$(echo "${line}" | cut -d '=' -f 1)
        local value=$(echo "${line}" | cut -d '=' -f 2-)

        # Remove leading and trailing whitespaces from key and value
        key=$(echo "${key}" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
        value=$(echo "${value}" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')

        # Remove surrounding quotes from value if they exist
        value=$(echo "${value}" | sed -e 's/^"//' -e 's/"$//' -e "s/^'//" -e "s/'$//")

        # Add the prefix to the variable name
        new_key="ENV_PROPERTY_${key}"

        # Export the new variable with the same value
        export "${new_key}"="${value}"
    done < <(read_file_without_comments ".env")

    # Disable automatic export
    set +a
}



# Converts a boolean value (true/false) to a string representation ("True"/"False").
# Arguments:
#   $1 - The boolean value to convert (should be either 'true' or 'false').
# Output: Prints "True" if the input value is 'true', otherwise prints "False".
get_boolean_as_string() {
    local value="$1"

    if [[ "${value}" = true ]]; then
        echo "True"
    else
        echo "False"
    fi
}



# Prints a formatted step message with a specified number of newlines before and after the message.
# Arguments:
#   $1 - The text of the step to be printed.
#   $2 - The format for newlines, specified as "before:after", where both before and after are counts of newlines.
# Output: Prints the step text with the specified formatting and increments the step number.
print_step() {
    local step_text="$1"
    local step_newline_format="$2"

    # Default values for newline counts
    local step_newline_count_before=1
    local step_newline_count_after=1

    # Check if the second parameter matches the format 0-infinite:0-infinite
    if [[ "${step_newline_format}" =~ ^[0-9]+:[0-9]+$ ]]; then
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



# Prints a formatted "Aborting" step message and exits with a status code of 0.
# Output: Prints the "Aborting" message and exits the script.
print_abort_step_and_exit() {
    print_step "Aborting" "1:2"
    exit 0
}
# Prints a formatted "Finishing" step message and exits with a status code of 0.
# Output: Prints the "Finishing" message and exits the script.
print_finish_step_and_exit() {
    print_step "Finishing" "1:2"
    exit 0
}
# Prints a formatted "Exiting" step message and exits with a status code of 1.
# Output: Prints the "Exiting" message and exits the script with an error status.
print_exit_step_and_exit() {
    print_step "Exiting" "1:2"
    exit 1
}



# Prints the provided text in bold using predefined formatting.
# Arguments:
#   $1 - The text to be printed in bold.
# Output: Prints the text surrounded by bold formatting.
print_bold() {
    local text="$1"

    printf "${BOLD_START}%b${BOLD_END}" "$text"
}



# Presents a list of choices to the user with an option to abort the selection process.
#              Captures the selected option's index and outputs it to a specified file descriptor.
# Arguments:
#   $1 - The prompt string to display to the user.
#   $2 - $N - The list of choices presented to the user for selection.
# Output: Outputs the index of the selected choice to file descriptor 3.
select_option_with_abort() {
    local prompt="$1"
    shift
    local choices=("$@")

    local PS3="${prompt} (or select 'Abort' to exit): "
    while true; do
        local choice
        local REPLY
        select choice in "${choices[@]}" "Abort"; do
            if [[ "${REPLY}" -lt 1 || "${REPLY}" -gt "$((${#choices[@]} + 1))" ]]; then
                print_bold "Unknown choice '${REPLY}'\n\n"
            elif [[ "${choice}" == "Abort" ]]; then
                print_abort_step_and_exit
            elif [[ -n "${choice}" ]]; then
                echo "${REPLY}" >&3
                break 2
            fi
        done
    done
}
