# Python Imports
import os
from sys import exit
import logging

# Main
def load_env_file(script_path: str, relative_path_to_env_directory: str) -> None:
    """
    Load environment variables from a file and exit if environment file cannot be loaded.
    """
    script_dir_path = os.path.dirname(script_path)
    env_dir_path = os.path.join(script_dir_path, relative_path_to_env_directory)
    file_path = os.path.join(env_dir_path, ".env")

    if not os.path.exists(file_path):
        logging.error(f"env: Environment file not found: {file_path}")
        exit(1)

    if not os.path.isfile(file_path):
        logging.error(f"env: Environment file is not a file: {file_path}")
        exit(1)

    if not os.access(file_path, os.R_OK):
        logging.error(f"env: Environment file is not readable: {file_path}")
        exit(1)

    try:
        # Set a new environment variable
        os.environ['TEST_ENV_VAR'] = 'test_value'
        
        # Check if it was successfully set
        if os.environ.get('TEST_ENV_VAR') == 'test_value':
            logging.info("env: You have access to edit environment variables.")
        else:
            logging.error("env: You do NOT have access to edit environment variables.")
            exit(1)
    except Exception as e:
        logging.error(f"env: An error occurred while trying to set a new environment variable: {e}")
        exit(1)

    with open(file_path, "r") as file:
        logging.info(f"env: Loading environment variables from {file_path}")
        
        while True:
            line = file.readline()
            if not line:
                break
            
            line = line.strip()
            
            if not line or line.startswith("#"):
                continue
            
            parts = line.split("=")
            if len(parts) != 2:
                logging.warning(f"env: Invalid line in environment file: {line}")
                continue
            
            key, value = parts

            key = key.strip()
            value = value.strip()

            if value.startswith('"'):
                value = value[1:]

                try:
                    index_of_second_string_delimiter = value.index('"')
                    value = value[:index_of_second_string_delimiter]
                except ValueError:
                    logging.warning(f"env: Invalid line in environment file: {line}")
            else:
                value = value.split("#")[0].strip()

            logging.info(f"env: Setting environment variable: {key}={value}")
            os.environ[key] = value

def format_str_with_env(path: str) -> str:
    """
    Format a string with environment variables in it and return the result.
    """
    environment_variables = os.environ
    path = path.format(**environment_variables)
    return path
