# Python Imports
import os
from sys import exit
import logging

# Packages Imports
from dotenv import load_dotenv

# Main
def load_env_file(script_path: str, relative_path_to_env_directory: str) -> None:
    """
    Load environment variables from a file and exit if environment file cannot be loaded.
    """
    script_dir_path = os.path.dirname(script_path)
    env_dir_path = os.path.join(script_dir_path, relative_path_to_env_directory)
    file_path = os.path.join(env_dir_path, ".env")

    if not os.path.exists(file_path):
        logging.error(f"Environment file not found: {file_path}")
        exit(1)

    if not os.path.isfile(file_path):
        logging.error(f"Environment file is not a file: {file_path}")
        exit(1)

    if load_dotenv(dotenv_path=file_path):
        logging.info(f"Loaded environment variables from {file_path}")
    else:
        logging.error(f"Failed to load environment variables from {file_path}")
        exit(1)

def format_str_with_env(path: str) -> str:
    """
    Format a string with environment variables in it and return the result.
    """
    environment_variables = os.environ
    path = path.format(**environment_variables)
    return path
