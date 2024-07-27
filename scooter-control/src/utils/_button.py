# Packages Imports
from microcontroller import Pin

# Project Imports
from Button import ButtonBuilder

# Main
def add_debug_interceptor_if_debug(builder: ButtonBuilder, debug_key_name: str, debug: bool = False) -> None:    
    if debug:
        builder.add_keyboard_interceptor(debug_key_name)