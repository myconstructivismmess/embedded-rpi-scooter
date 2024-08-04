#!/usr/bin/env bash
set -e



# Base
SCRIPT_FILE_NAME="copy.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}common.sh"
assert_root



# Set working location to project root directory
print_step "Setting working location to project root directory"

if ! cd_to_project_root "${SCRIPT_DIR_PATH}"; then
    print_bold "Failed to set working location to project root directory.\n"
    
    print_exit_step_and_exit
fi



# Check if the .env file exists
print_step "Checking if the .env file exists"

env_file_exists



# Load environment variables
print_step "Loading environment variables"

# TODO: Make the following code block a function in common.sh
set -a # Enable automatic export of all variables

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



# Copy data overlay
print_step "Copying data overlay"

relative_directory_paths_to_copy=(
    ""
)

while [ "${#relative_directory_paths_to_copy[@]}" -gt 0 ]; do
    # Get the source directory paths
    relative_directory_path_src="${relative_directory_paths_to_copy[0]}"
    absolute_directory_path_src="$(pwd)/data_overlay/${relative_directory_path_src}"

    # Get the destination directory paths
    relative_directory_path_dst_eval="$(echo "${relative_directory_path_src}" | sed 's/{/${ENV_PROPERTY_/g')"
    relative_directory_path_dst="$(eval "echo ${relative_directory_path_dst_eval}")"
    absolute_directory_path_dst="/${relative_directory_path_dst}"

    # Get the files that are not ignored and directories in the source directory
    files_in_directory=($(get_files_in_path "${absolute_directory_path_src}" true))
    directories_in_directory=($(get_directories_in_path "${absolute_directory_path_src}"))

    # Remove the current directory from the list and add to it's childs
    relative_directory_paths_to_copy=("${relative_directory_paths_to_copy[@]:1}")
    for directory in "${directories_in_directory[@]}"; do
        relative_directory_paths_to_copy+=("${relative_directory_path_src}${directory}/")
    done

    # Check if the REMOVE_DIRECTORY_ON_COPY file exists
    remove_content_on_copy="$(path_contains_remove_content_on_copy_flag "${absolute_directory_path_src}")"

    # Start printing the path information
    print_bold "\nNavigating to './data_overlay/${relative_directory_path_src}'\n"

    # Create the destination directory if it does not exist and generate the text to print
    if [ -d "${absolute_directory_path_dst}" ]; then
        exist_text="True"
    else
        exist_text="False"
        mkdir -p "${absolute_directory_path_dst}"
    fi

    # Remove the content of the destination directory if the REMOVE_CONTENT_ON_COPY file exists and generate the text to print
    if [ "${remove_content_on_copy}" = true ]; then
        remove_content_on_copy_text="True"
        sudo rm -rf "${absolute_directory_path_dst}"/*
    else
        remove_content_on_copy_text="False"
    fi

    # Print the path information
    print_bold "\tTarget directory: '${absolute_directory_path_dst}'  Exists: ${exist_text}  Remove content: ${remove_content_on_copy_text}\n"
    
    # Print the directories that will be added to the copy process
    for directory in "${directories_in_directory[@]}"; do
        print_bold "\tAdded sub-directory '${directory}' to the copy process.\n"
    done

    # Copy the files
    for file in "${files_in_directory[@]}"; do
        print_bold "\tCopying the file '${file}'.\n"
        cp "${absolute_directory_path_src}${file}" "${absolute_directory_path_dst}${file}"
    done
done



# Finishing
print_finish_step_and_exit
