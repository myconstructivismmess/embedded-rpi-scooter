# Python Imports
import os
import sys
from sys import exit
import logging

# Project Imports
from ..utils.env import load_env_file

from ..button import ButtonBuilder
from ..program import Program

# Main
class HornSoundPlayerTest(Program):
    def create_horn_button(self):
        pass
        # Horn button
        # horn_button_builder = ButtonBuilder()
        # horn_button_builder.add_keyboard_interceptor('h')
        # horn_button = horn_button_builder.build()

    def __init__(self):
        super().__init__(True)

    def update(self):
        pass

if __name__ == '__main__':
    # Check python version
    if sys.version_info.major < 3:
        logging.error("This script requires at least python version 3")
        exit(1)
    
    # Set logging level
    logging.getLogger().setLevel(logging.INFO)

    # Load environment variables
    load_env_file(os.path.realpath(__file__), "../")

    # Create and run the program
    horn_sound_player_test: HornSoundPlayerTest = HornSoundPlayerTest()
    horn_sound_player_test.run_until_quit()
