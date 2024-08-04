#!/usr/bin/env bash

RELATIVE_PATH_TO_PROJECT_ROOT="../"

SCRIPT_FILE_NAME="common.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}common_functions.sh"
