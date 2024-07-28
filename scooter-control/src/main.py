# Python Imports
import sys
from enum import Enum

# Packages Imports
#from neopixel import NeoPixel, GRB

# Project Imports
import utils
from types import *
import CONSTANTS
from Button import ButtonBuilder, Button, Handlers as ButtonHandlers

# Debug Parameters
ENABLE_DEVELOPMENT_KEYBOARD_BUTTONS = True
ENABLE_GPIO_PIN_BUTTONS = False

PRINT_TURN_SIGNAL_VALUE_CHANGES = True
PRINT_LIGHT_ON_OFF_VALUE_CHANGES = True

# Types
class TurnSignalStateType(Enum):
    NONE = 1
    LEFT = 2
    RIGHT = 3

# Main
class Main:
    def _on_turn_signal_button_value_changed(self, is_left_side: bool, state: bool) -> None:
        if state:
            self._turn_signal_state = TurnSignalStateType.LEFT if is_left_side else TurnSignalStateType.RIGHT
            if is_left_side:
                self._turn_right_signal_button_handler.set_value(False, False)
            else:
                self._turn_left_signal_button_handler.set_value(False, False)
        else:
            self._turn_signal_state = TurnSignalStateType.NONE
        
        if PRINT_TURN_SIGNAL_VALUE_CHANGES:
            print(f"INFO: TURN_SIGNAL: Value changed to {self._turn_signal_state.name}")

    def _on_light_on_off_button_value_changed(self, state: bool) -> None:
        if PRINT_LIGHT_ON_OFF_VALUE_CHANGES:
            print(f"INFO: LIGHT_ON_OFF: Value changed to {state}")

    def _create_horn_button(self) -> None:
        builder: ButtonBuilder = ButtonBuilder()

        utils.button.add_keyboard_interceptor_if_enabled(
            builder,
            "h",
            ENABLE_DEVELOPMENT_KEYBOARD_BUTTONS
        )

        utils.button.add_gpio_pin_interceptor_if_enabled(
            builder,
            CONSTANTS.PIN.HORN_BUTTON,
            ENABLE_GPIO_PIN_BUTTONS
        )
        
        # TODO: Implement Horn Button Handler
        #self._horn_button: Button = builder.build()

    def _create_turn_left_signal_button(self) -> None:
        builder: ButtonBuilder = ButtonBuilder()

        utils.button.add_keyboard_interceptor_if_enabled(
            builder,
            "j",
            ENABLE_DEVELOPMENT_KEYBOARD_BUTTONS
        )

        utils.button.add_gpio_pin_interceptor_if_enabled(
            builder,
            CONSTANTS.PIN.TURN_LEFT_SIGNAL_BUTTON,
            ENABLE_GPIO_PIN_BUTTONS
        )

        self._turn_left_signal_button_handler: ButtonHandlers.LatchButtonHandler = builder.set_latch_button_handler(lambda state: self._on_turn_signal_button_value_changed(True, state))
        self._turn_left_signal_button: Button = builder.build()

    def _create_turn_right_signal_button(self) -> None:
        builder: ButtonBuilder = ButtonBuilder()

        utils.button.add_keyboard_interceptor_if_enabled(
            builder,
            "k",
            ENABLE_DEVELOPMENT_KEYBOARD_BUTTONS
        )

        utils.button.add_gpio_pin_interceptor_if_enabled(
            builder,
            CONSTANTS.PIN.TURN_RIGHT_SIGNAL_BUTTON,
            ENABLE_GPIO_PIN_BUTTONS
        )

        self._turn_right_signal_button_handler: ButtonHandlers.LatchButtonHandler = builder.set_latch_button_handler(lambda state: self._on_turn_signal_button_value_changed(False, state))
        self._turn_right_signal_button: Button = builder.build()

    def _create_light_on_off_button(self) -> None:
        builder: ButtonBuilder = ButtonBuilder()

        utils.button.add_keyboard_interceptor_if_enabled(
            builder,
            "l",
            ENABLE_DEVELOPMENT_KEYBOARD_BUTTONS
        )

        utils.button.add_gpio_pin_interceptor_if_enabled(
            builder,
            CONSTANTS.PIN.LIGHT_ON_OFF_BUTTON,
            ENABLE_GPIO_PIN_BUTTONS
        )

        self._light_on_off_button_handler: ButtonHandlers.LatchButtonHandler = builder.set_latch_button_handler(self._on_light_on_off_button_value_changed)
        self._light_on_off_button: Button = builder.build()

    def __init__(self) -> None:
        # Turn Signal
        self._turn_signal_state: TurnSignalStateType = TurnSignalStateType.NONE

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
        print("ERROR: This script requires at least python version 3")
        sys.exit(1)
    
    main = Main()
    while True:
        if main.should_exit:
            break

        main.update()
