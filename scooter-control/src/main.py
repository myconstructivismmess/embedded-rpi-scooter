# Python Imports
import sys

# Packages Imports
from neopixel import NeoPixel, GRB
from microcontroller import Pin

# Project Imports
import utils
from Button import ButtonBuilder, Button

# Debug Parameters
ENABLE_DEVELOPMENT_KEYBOARD_KEYS = True

# Main
class Main:
    def _create_horn_button(self) -> None:
        builder: ButtonBuilder = ButtonBuilder()

        utils.button.add_debug_interceptor_if_debug(
            builder,
            "h",
            ENABLE_DEVELOPMENT_KEYBOARD_KEYS
        )
        
        self._horn_button: Button = builder.build()

    def _create_turn_left_signal_button(self) -> None:
        builder: ButtonBuilder = ButtonBuilder()

        utils.button.add_debug_interceptor_if_debug(
            builder,
            "j",
            ENABLE_DEVELOPMENT_KEYBOARD_KEYS
        )

        self._turn_left_signal_button: Button = builder.build()

    def _create_turn_right_signal_button(self) -> None:
        builder: ButtonBuilder = ButtonBuilder()

        utils.button.add_debug_interceptor_if_debug(
            builder,
            "k",
            ENABLE_DEVELOPMENT_KEYBOARD_KEYS
        )

        self._turn_right_signal_button: Button = builder.build()

    def _create_light_on_off_button(self) -> None:
        builder: ButtonBuilder = ButtonBuilder()

        utils.button.add_debug_interceptor_if_debug(
            builder,
            "l",
            ENABLE_DEVELOPMENT_KEYBOARD_KEYS
        )

        self._light_on_off_button: Button = builder.build()

    def __init__(self) -> None:
        # Buttons
        self._create_horn_button()
        self._create_turn_left_signal_button()
        self._create_turn_right_signal_button()
        self._create_light_on_off_button()

        # Exit
        self._should_exit: bool = False

    @property
    def should_exit(self) -> bool:
        return self._should_exit

    def update(self):
        pass


if __name__ == "__main__":
    if sys.version_info.major < 3:
        print("This script requires at least python version 3")
        sys.exit(1)
    
    main = Main()
    while True:
        if main.should_exit:
            break

        main.update()
