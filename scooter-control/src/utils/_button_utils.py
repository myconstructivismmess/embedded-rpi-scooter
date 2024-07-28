# Packages Imports
from microcontroller import Pin

# Project Imports
from Button import ButtonBuilder

# Main
def add_keyboard_interceptor_if_enabled(builder: ButtonBuilder, debug_key_name: str, enabled: bool = False, reverse_signal: bool = False) -> None:    
    if enabled:
        builder.add_keyboard_interceptor(debug_key_name, reverse_signal)

def add_gpio_pin_interceptor_if_enabled(builder: ButtonBuilder, gpio_pin: Pin, enabled: bool = False, reverse_signal: bool = False) -> None:
    if enabled:
        builder.add_gpio_pin_interceptor(gpio_pin, reverse_signal)