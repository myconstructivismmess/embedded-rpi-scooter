#!/usr/bin/env bash
set -e

BOLD_START="\033[1m"
BOLD_END="\033[0m"
RELATIVE_PATH_FROM_SCRIPT_DIR="../"

SCRIPT_DIR_PATH="${BASH_SOURCE:0:-8}"
cd "$SCRIPT_DIR_PATH"

# Set working location to the project root directory
printf "\n${BOLD_START} 1. Setting script working location...${BOLD_END}\n"

cd "${RELATIVE_PATH_FROM_SCRIPT_DIR}"

# Remove python 3 virtual environment
printf "\n${BOLD_START} 2. Remove python 3 virtual environment...${BOLD_END}\n"

sudo rm -rf ./scooter-control/.venv/

# Remove python 3 cache directories
printf "\n${BOLD_START} 3. Remove python 3 cache...${BOLD_END}\n"

find ./scooter-control/ | grep -E "(/__pycache__$|\.pyc$|\.pyo$)" | xargs rm -rf

# Finishing
printf "\n${BOLD_START} 4. Finishing...${BOLD_END}\n\n"