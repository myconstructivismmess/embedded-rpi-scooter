# Python Imports
import os

import sys
from sys import exit

import logging

from typing import Tuple

# Packages Imports
#from neopixel import NeoPixel, GRB

# Project Imports
from .utils.button import add_keyboard_interceptor_if_enabled, add_gpio_pin_interceptor_if_enabled
from .utils.env import load_env_file, format_str_with_env

from .timer import Timer, TimerHandler

from .button import ButtonBuilder, Button, handlers as ButtonHandlers
from .horn_button_handler import HornButtonHandler

from .horn_sound_player import HornSoundPlayer

from .program import Program

from .pins import Pins

from .boolean import Boolean

from .turn_signal_state_type import TurnSignalStateType

# Types
class CreateButtonOptions:
    def __init__(self, enable_gpio_pin_buttons: bool, enable_keyboard_buttons: bool) -> None:
        self.enable_gpio_pin_buttons: bool = enable_gpio_pin_buttons
        self.enable_keyboard_buttons: bool = enable_keyboard_buttons

# Main
class ScooterControl(Program):
    # Turn Signal
    def _on_turn_signal_button_value_changed(self, is_left_side: bool, state: bool) -> None:
        if state:
            self._turn_signal_state = TurnSignalStateType.LEFT if is_left_side else TurnSignalStateType.RIGHT
            # TODO
            if is_left_side:
                self._turn_right_signal_button_handler.set_value(False, False)
            else:
                self._turn_left_signal_button_handler.set_value(False, False)
            self._turn_signal_last_time_enabled_side_left = is_left_side
            self._turn_signal_last_time_enabled_time_seconds = self._timer.time_seconds
        else:
            self._turn_signal_state = TurnSignalStateType.NONE
        if self._log_turn_signal_value_changes:
            logging.info(f"TURN_SIGNAL: Value changed to {self._turn_signal_state.name}")

    def _create_turn_left_signal_button(self, create_button_options: CreateButtonOptions) -> Tuple[Button, ButtonHandlers.LatchButtonHandler]:
        builder: ButtonBuilder = ButtonBuilder()
        add_keyboard_interceptor_if_enabled(
            builder,
            os.getenv("TURN_LEFT_SIGNAL_KEYBOARD_KEY"),
            create_button_options.enable_keyboard_buttons
        )
        add_gpio_pin_interceptor_if_enabled(
            builder,
            self._pins.turn_left_signal_button_pin,
            create_button_options.enable_gpio_pin_buttons
        )
        button_handler: ButtonHandlers.LatchButtonHandler = builder.set_latch_button_handler(
            lambda state: self._on_turn_signal_button_value_changed(True, state)
        )
        button: Button = builder.build()
        return (button, button_handler)

    def _create_turn_right_signal_button(self, create_button_options: CreateButtonOptions) -> Tuple[Button, ButtonHandlers.LatchButtonHandler]:
        builder: ButtonBuilder = ButtonBuilder()
        add_keyboard_interceptor_if_enabled(
            builder,
            os.getenv("TURN_RIGHT_SIGNAL_KEYBOARD_KEY"),
            create_button_options.enable_keyboard_buttons
        )
        add_gpio_pin_interceptor_if_enabled(
            builder,
            self._pins.turn_right_signal_button_pin,
            create_button_options.enable_gpio_pin_buttons
        )
        button_handler: ButtonHandlers.LatchButtonHandler = builder.set_latch_button_handler(
            lambda state: self._on_turn_signal_button_value_changed(False, state)
        )
        button: Button = builder.build()
        return (button, button_handler)



    # Light On/Off
    def _on_light_on_off_button_value_changed(self, state: bool) -> None:
        if self._log_light_on_off_value_changes:
            logging.info(f"LIGHT_ON_OFF: Value changed to {state}")

    def _create_light_on_off_button(self, create_button_options: CreateButtonOptions) -> Tuple[Button, ButtonHandlers.DefaultButtonHandler]:
        builder: ButtonBuilder = ButtonBuilder()
        add_keyboard_interceptor_if_enabled(
            builder,
            os.getenv("LIGHT_ON_OFF_KEYBOARD_KEY"),
            create_button_options.enable_keyboard_buttons
        )
        add_gpio_pin_interceptor_if_enabled(
            builder,
            self._pins.light_on_off_button_pin,
            create_button_options.enable_gpio_pin_buttons
        )
        button_handler: ButtonHandlers.DefaultButtonHandler = builder.set_default_button_handler(self._on_light_on_off_button_value_changed)
        button: Button = builder.build()
        return (button, button_handler)
    


    # Horn
    def _on_horn_button_start(self, index: int) -> None:
        self._horn_sound_player.play_sound(index)
        if self._log_horn_triggered_events:
            is_long_press: bool = index % 2 == 1
            press_count: int = index // 2 + 1
            logging.info(f"HORN: Start ({'long' if is_long_press else 'short'} press, count: {press_count})")

    def _on_horn_button_stop(self) -> None:
        self._horn_sound_player.stop_repeatable_sound()
        if self._log_horn_triggered_events:
            logging.info("HORN: Stop")

    def _create_horn_button(self, create_button_options: CreateButtonOptions) -> Tuple[Button, HornButtonHandler]:
        builder: ButtonBuilder = ButtonBuilder()
        add_keyboard_interceptor_if_enabled(
            builder,
            os.getenv("HORN_KEYBOARD_KEY"),
            create_button_options.enable_keyboard_buttons
        )
        add_gpio_pin_interceptor_if_enabled(
            builder,
            self._pins.horn_button_pin,
            create_button_options.enable_gpio_pin_buttons
        )
        button_handler: HornButtonHandler = HornButtonHandler(
            self._timer,
            self._on_horn_button_start,
            self._on_horn_button_stop,
            5,
            [True, False],
            float(os.getenv("HORN_MAX_SHORT_PRESS_DURATION_SECONDS")),
            float(os.getenv("HORN_MAX_PRESS_INTERVAL_DURATION_SECONDS"))
        )
        builder.set_handler(button_handler)
        button: Button = builder.build()
        return (button, button_handler)



    def __init__(self) -> None:
        super().__init__()

        # Debugging
        self._log_turn_signal_value_changes: bool = Boolean(os.getenv("LOG_TURN_SIGNAL_VALUE_CHANGES")).value
        self._log_light_on_off_value_changes: bool = Boolean(os.getenv("LOG_LIGHT_ON_OFF_VALUE_CHANGES")).value
        self._log_horn_triggered_events: bool = Boolean(os.getenv("LOG_HORN_TRIGGERED_EVENTS")).value

        # Buttons enable/disable
        enable_gpio_pin_buttons: bool = Boolean(os.getenv("ENABLE_GPIO_PIN_BUTTONS")).value
        enable_keyboard_buttons: bool = Boolean(os.getenv("ENABLE_KEYBOARD_BUTTONS")).value

        # Pins (Nothing more to setup here)
        self._pins: Pins = Pins()

        # Timer
        self._timer: Timer = Timer()

        # Horn Sound Player
        self._horn_sound_player: HornSoundPlayer = HornSoundPlayer(
            format_str_with_env(os.getenv("DEFAULT_HORN_SOUND_COLLECTION_PATH")),
            os.getenv("DEFAULT_AUDIO_DEVICE")
        )

        # Turn Signal
        self._turn_signal_last_time_enabled_time_seconds: float = -9999.0
        self._turn_signal_last_time_enabled_side_left: bool = False

        self._turn_signal_state: TurnSignalStateType = TurnSignalStateType.NONE
        self._turn_signal_timer_handler: TimerHandler = self._timer.create_handler()

        # Buttons
        create_button_options: CreateButtonOptions = CreateButtonOptions(enable_gpio_pin_buttons, enable_keyboard_buttons)

        self._turn_left_signal_button, self._turn_left_signal_button_handler = self._create_turn_left_signal_button(create_button_options)
        self._turn_right_signal_button, self._turn_right_signal_button_handler = self._create_turn_right_signal_button(create_button_options)
        self._light_on_off_button, self._light_on_off_button_handler = self._create_light_on_off_button(create_button_options)
        self._horn_button, self._horn_button_handler = self._create_horn_button(create_button_options)

    # Update
    def update(self):
        # Update timer
        self._timer.update()
        
        # Update buttons
        self._turn_left_signal_button.update()
        self._turn_right_signal_button.update()
        self._light_on_off_button.update()
        self._horn_button.update()

        # Update horn sound player
        self._horn_sound_player.update()

if __name__ == "__main__":
    # Check python version
    if sys.version_info.major < 3:
        logging.error("This script requires at least python version 3")
        exit(1)
    
    # Set logging level
    logging.getLogger().setLevel(logging.INFO)

    # Load environment variables
    load_env_file(os.path.realpath(__file__), "../")

    # Create and run the program
    scooter_control = ScooterControl()
    scooter_control.run_until_quit()
