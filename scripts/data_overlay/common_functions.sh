DIRECTORIES_TO_IGNORE=(
    ".cache"
)
FILES_TO_IGNORE=(
    ".dataoverlayremove"
    ".dataoverlayignore"
)



# Lists the names of directories within the specified path, excluding directories that match any in the list of directories to ignore.
# Arguments:
#   $1 - The path in which to search for directories.
# Environment Variables:
#   DIRECTORIES_TO_IGNORE - An array of directory names to exclude from the output.
# Output: Prints the names of directories found in the specified path that do not match any names in DIRECTORIES_TO_IGNORE.
get_directories_in_path() {
    local path="$1"

    local directory
    while IFS= read -r -d $'\0' directory; do
        local ignore=false
        local directory_name="$(basename "${directory}")"
        
        local dir_to_ignore
        for dir_to_ignore in "${DIRECTORIES_TO_IGNORE[@]}"; do
            if [[ "${directory_name}" == *"${dir_to_ignore}"* ]]; then
                ignore=true
                break
            fi
        done

        if [ "${ignore}" = false ]; then
            ignore="$(is_file_or_directory_ignored_by_ignore_list "${path}" "${directory}/")"
        fi

        if [ "${ignore}" = false ]; then
            echo "${directory_name}"
        fi
    done < <(find "${path}" -mindepth 1 -maxdepth 1 -type d -print0)
}
# Lists the names of files within the specified path, excluding files that match any in the list of files to ignore or those identified by a custom ignore file.
# Arguments:
#   $1 - The path in which to search for files.
# Environment Variables:
#   FILES_TO_IGNORE - An array of file names to exclude from the output.
# Output: Prints the names of files found in the specified path that do not match any names in FILES_TO_IGNORE and are not ignored by the custom ignore file.
get_files_in_path() {
    local path="$1"

    local file
    while IFS= read -r -d $'\0' file; do
        local ignore=false
        local file_name="$(basename "${file}")"
        
        local file_to_ignore
        for file_to_ignore in "${FILES_TO_IGNORE[@]}"; do
            if [[ "${file_name}" == *"${file_to_ignore}"* ]]; then
                ignore=true
                break
            fi
        done

        if [ "${ignore}" = false ]; then
            ignore="$(is_file_or_directory_ignored_by_ignore_list "${path}" "${file_name}")"
        fi

        if [ "${ignore}" = false ]; then
            echo "${file_name}"
        fi
    done < <(find "${path}" -mindepth 1 -maxdepth 1 -type f -print0)
}
# Determines if a specified file is listed in the ignore file located at `${path}/.dataoverlayignore`.
# Arguments:
#   $1 - The path to the directory containing the ignore file.
#   $2 - The name of the file to check against the ignore list.
# Output: Prints "true" if the file name is found in the ignore list, otherwise prints "false".
is_file_or_directory_ignored_by_ignore_list() {
    local path="$1"
    local file_or_directory_name="$2"

    if [ ! -f "${path}/.dataoverlayignore" ]; then
        echo false
        return
    fi

    local line
    while IFS= read -r line; do
        if [ "${line}" = "${file_or_directory_name}" ]; then
            echo true
            return
        fi
    done < <(read_file_without_comments "${path}/.dataoverlayignore")

    echo false
}



# Checks if a specified directory contains a flag file named '.dataoverlayremove'.
# Arguments:
#   $1 - The path of the directory to check.
# Output: Prints "true" if the flag file exists in the directory, otherwise prints "false".
path_contains_remove_flag() {
    local path="$1"

    if [ -f "${path}/.dataoverlayremove" ]; then
        echo true
        return
    fi

    echo false
}



# Checks if the data_overlay directory exists.
# Output: Prints an error message and exits if the data_overlay directory does not exist.
assert_data_overlay_directory_exists() {
    if [ ! -d "data_overlay" ]; then
        print_bold "\nThe 'data_overlay' directory does not exist.\n"

        print_exit_step_and_exit
    fi
}
# Purges the data overlay by recursively processing the .cache directory within the data_overlay directory,
#              removing directories marked for deletion and ultimately deleting the .cache directory itself.
# Arguments: None
# Output: Prints the status of each processed directory and whether it was deleted.
purge_data_overlay() { # TODO: Improve error handling and security
    # Check if the data_overlay directory exists
    assert_data_overlay_directory_exists

    if [ ! -d "./data_overlay/.cache/" ]; then
        print_bold "\nThe './data_overlay/.cache/' directory does not exist. Skiping the purge process.\n"
        return
    fi

    # Create the array to store the relative directories paths to process
    relative_directory_paths_to_process=(
        ""
    )

    # Iterate through the directories in the data_overlay directory until all directories are processed
    while [ "${#relative_directory_paths_to_process[@]}" -gt 0 ]; do
        # Get the source directory paths
        relative_directory_path_src="${relative_directory_paths_to_process[0]}"
        absolute_directory_path_src="$(pwd)/data_overlay/.cache/${relative_directory_path_src}"

        # Get the destination directory paths
        relative_directory_path_dst="${relative_directory_path_src}"
        absolute_directory_path_dst="/${relative_directory_path_dst}"


        # Get the files and directories names that are not ignored in the source directory
        files_in_directory=($(get_files_in_path "${absolute_directory_path_src}"))
        directories_in_directory=($(get_directories_in_path "${absolute_directory_path_src}"))


        # Check if the remove flag is set
        remove_flag="$(path_contains_remove_flag "${absolute_directory_path_src}")"


        # Remove the current directory from the list of directories to process and add it's children if the remove flag is not set
        relative_directory_paths_to_process=("${relative_directory_paths_to_process[@]:1}")
        if [ ! "${remove_flag}" = true ]; then
            for directory in "${directories_in_directory[@]}"; do
                relative_directory_paths_to_process+=("${relative_directory_path_src}${directory}/")
            done
        fi


        # Check if the destination directory exists
        if [ -d "${absolute_directory_path_dst}" ]; then
            destination_directory_exists=true
        else
            destination_directory_exists=false
        fi


        # Delete the destination directory if the remove flag is set and the destination directory exists
        if [ "${remove_flag}" = true -a "${destination_directory_exists}" = true ]; then
            rm -rf "${absolute_directory_path_dst}"
        fi


        # Start printing directory information
        print_bold "\nProcessing './data_overlay/.cache/${relative_directory_path_src}'\n"

        # Print directory information
        print_bold "\tRemove flag: $(get_boolean_as_string "${remove_flag}")\n"

        printf "\n"

        # Print destination directory information
        print_bold "\tDestination directory: '${absolute_directory_path_dst}'  Exists: $(get_boolean_as_string "${destination_directory_exists}")\n"
        if [ "${remove_flag}" = true -a "${destination_directory_exists}" = true ]; then
            print_bold "\t\tDeleted it.\n"
        fi
    done

    # Delete the cache directory
    print_bold "\nDeleting the './data_overlay/.cache/' directory.\n"
    rm -rf "./data_overlay/.cache/"
}
# Copies the data overlay by recursively processing the data_overlay directory, evaluating paths for environment properties, 
# and copying files to their respective destinations, while creating necessary directories and handling remove flags.
# Arguments: None
# Output: Prints the status of each processed directory, whether directories or cache directories were created, 
#         and the files that have been copied.
copy_data_overlay() { # TODO: Improve error handling and security
    # Check if the data_overlay directory exists
    assert_data_overlay_directory_exists

    # Create the array to store the relative directories paths to process
    relative_directory_paths_to_process=(
        ""
    )

    # Iterate through the directories in the data_overlay directory until all directories are processed
    while [ "${#relative_directory_paths_to_process[@]}" -gt 0 ]; do
        # Get the source directory paths
        relative_directory_path_src="${relative_directory_paths_to_process[0]}"
        absolute_directory_path_src="$(pwd)/data_overlay/${relative_directory_path_src}"

        # Get the destination directory paths
        relative_directory_path_dst_eval="$(echo "${relative_directory_path_src}" | sed 's/{/${ENV_PROPERTY_/g')"
        relative_directory_path_dst="$(eval "echo ${relative_directory_path_dst_eval}")"
        absolute_directory_path_dst="/${relative_directory_path_dst}"


        # Get the files and directories names that are not ignored in the source directory
        files_in_directory=($(get_files_in_path "${absolute_directory_path_src}"))
        directories_in_directory=($(get_directories_in_path "${absolute_directory_path_src}"))


        # Remove the current directory from the list of directories to process and add it's children
        relative_directory_paths_to_process=("${relative_directory_paths_to_process[@]:1}")
        for directory in "${directories_in_directory[@]}"; do
            relative_directory_paths_to_process+=("${relative_directory_path_src}${directory}/")
        done


        # Check if the remove flag is set
        remove_flag="$(path_contains_remove_flag "${absolute_directory_path_src}")"


        # Create the destination directory if it does not exist
        if [ -d "${absolute_directory_path_dst}" ]; then
            destination_directory_exists=true
        else
            mkdir -p "${absolute_directory_path_dst}"
            destination_directory_exists=false
        fi


        # Create the destination cache directory if it does not exist
        if [ -d "./data_overlay/.cache/${relative_directory_path_dst}" ]; then
            destination_cache_directory_exists=true
        else
            mkdir -p "./data_overlay/.cache/${relative_directory_path_dst}"
            destination_cache_directory_exists=false
        fi


        # Create the destination cache directory remove flag file if the remove flag is set
        if [ "${remove_flag}" = true ]; then
            touch "./data_overlay/.cache/${relative_directory_path_dst}/.dataoverlayremove"
        fi


        # Copy the files
        for file in "${files_in_directory[@]}"; do
            cp "${absolute_directory_path_src}${file}" "${absolute_directory_path_dst}${file}"
        done


        # Start printing directory information
        print_bold "\nProcessing './data_overlay/${relative_directory_path_src}'\n"

        # Print directory information
        print_bold "\tRemove flag: $(get_boolean_as_string "${remove_flag}")\n"

        printf "\n"

        # Print destination directory information
        print_bold "\tDestination directory: '${absolute_directory_path_dst}'  Exists: $(get_boolean_as_string "${destination_directory_exists}")\n"
        if [ "${destination_directory_exists}" = false ]; then
            print_bold "\t\tCreated it.\n"
        fi

        # Print the files that have been copied to the destination directory
        for file in "${files_in_directory[@]}"; do
            print_bold "\t\tCopying the file '${file}' to the destination directory.\n"
        done

        # NOTE: Fix spacing later
        printf "\n"
        
        # Print destination cache directory information
        print_bold "\tDestination cache directory: './data_overlay/.cache/${relative_directory_path_dst}'  Exists: $(get_boolean_as_string "${destination_cache_directory_exists}")\n"
        if [ "${destination_cache_directory_exists}" = false ]; then
            print_bold "\t\tCreated it.\n"
        fi

        printf "\n"

        # Print the directories that have been added to the process
        for directory in "${directories_in_directory[@]}"; do
            print_bold "\tAdded sub-directory '${directory}' to the process.\n"
        done
    done
}
