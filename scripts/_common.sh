#!/usr/bin/env bash

RELATIVE_PATH_TO_PROJECT_ROOT="../"

SCRIPT_FILE_NAME="_common.sh"
SCRIPT_DIR_PATH="${BASH_SOURCE:0:-${#SCRIPT_FILE_NAME}}"
source "${SCRIPT_DIR_PATH}_common_functions.sh"
