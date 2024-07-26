# Packages Imports
from microcontroller import Pin

# Project Imports
from Button import ButtonBuilder

# Main
def create_base_button_builder(gpio_pin: Pin, debug_key_name: str, debug: bool = False) -> ButtonBuilder:
    builder = ButtonBuilder()

    builder.add_gpio_pin_interceptor(gpio_pin)
    
    if debug:
        builder.add_keyboard_interceptor(debug_key_name)

    return builder