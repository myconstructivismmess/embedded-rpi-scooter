env_file_exists() {
    if [ ! -f ".env" ]; then
        print_bold "\nThe .env file does not exist.\n"

        print_exit_step_and_exit
    fi
}



get_directories_in_path() {
    local path="$1"
    local directory

    while IFS= read -r -d $'\0' directory; do
        echo "$(basename "${directory##*/}")"
    done < <(find "${path}" -mindepth 1 -maxdepth 1 -type d -print0)
}
# Get files in a directory that are not named REMOVE_CONTENT_ON_COPY, REMOVE_DIRECTORY_ON_PURGE or FILES_TO_IGNORE_ON_COPY
get_files_in_path() {
    local path="$1"
    local remove_files_to_ignore_on_copy_flag="$2" 
    if [ -z "${remove_files_to_ignore_on_copy_flag}" ]; then
        remove_files_to_ignore_on_copy_flag=false
    fi
    local file

    local files_to_ignore=()
    if [ "${remove_files_to_ignore_on_copy_flag}" = true ]; then
        get_files_to_ignore_on_copy_in_path "${path}" | while IFS= read -r line; do
            files_to_ignore+=("${line}")
        done
    fi

    while IFS= read -r -d $'\0' file; do
        if [[ "${file}" != *REMOVE_CONTENT_ON_COPY* && "${file}" != *REMOVE_DIRECTORY_ON_PURGE* && "${file}" != *FILES_TO_IGNORE_ON_COPY* && ! " ${files_to_ignore[@]} " =~ " $(basename "${file}") " ]]; then
            echo "$(basename "${file}")"
        fi
    done < <(find "${path}" -mindepth 1 -maxdepth 1 -type f -print0)
}
get_files_to_ignore_on_copy_in_path() {
    local path="$1"
    local file

    while IFS= read -r -d $'\0' file; do
        if [[ "${file}" == *FILES_TO_IGNORE_ON_COPY* ]]; then
            while IFS= read -r line || [ -n "${line}" ]; do
                echo "$(echo "${line}" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')"
            done < "${file}"
        fi
    done < <(find "${path}" -mindepth 1 -maxdepth 1 -type f -print0)
}



path_contains_remove_content_on_copy_flag() {
    local path="$1"
    local files=()
    local file

    while IFS= read -r -d $'\0' file; do
        if [[ "${file}" == *REMOVE_CONTENT_ON_COPY* ]]; then
            echo true
            exit 0
        fi
    done < <(find "${path}" -mindepth 1 -maxdepth 1 -type f -print0)

    echo false
}
path_contains_remove_directory_on_purge_flag() {
    local path="$1"
    local files=()
    local file

    while IFS= read -r -d $'\0' file; do
        if [[ "${file}" == *REMOVE_DIRECTORY_ON_PURGE* ]]; then
            echo true
            exit 0
        fi
    done < <(find "${path}" -mindepth 1 -maxdepth 1 -type f -print0)

    echo false
}
