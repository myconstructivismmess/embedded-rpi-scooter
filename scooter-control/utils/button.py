# Project Imports
from ..button import ButtonBuilder
from ..gpio_pin import GPIOPin

# Main
def add_keyboard_interceptor_if_enabled(builder: ButtonBuilder, debug_key_name: str, enabled: bool = False, reverse_signal: bool = False) -> None:    
    if enabled:
        builder.add_keyboard_interceptor(debug_key_name, reverse_signal)

def add_gpio_pin_interceptor_if_enabled(builder: ButtonBuilder, gpio_pin: GPIOPin, enabled: bool = False, reverse_signal: bool = False) -> None:
    if enabled:
        builder.add_gpio_pin_interceptor(gpio_pin, reverse_signal)
