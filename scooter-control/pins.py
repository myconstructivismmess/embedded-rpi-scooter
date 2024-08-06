# Python Imports
import os

# Project Imports
from .gpio_pin import GPIOPin

# Main
class Pins:
    def __init__(self) -> None:
        self._front_leds_pin: GPIOPin = GPIOPin(os.environ.get("FRONT_LEDS_PIN"))
        self._back_leds_pin: GPIOPin = GPIOPin(os.environ.get("BACK_LEDS_PIN"))
        self._turn_left_signal_button_pin: GPIOPin = GPIOPin(os.environ.get("TURN_LEFT_SIGNAL_BUTTON_PIN"))
        self._turn_right_signal_button_pin: GPIOPin = GPIOPin(os.environ.get("TURN_RIGHT_SIGNAL_BUTTON_PIN"))
        self._light_on_off_button_pin: GPIOPin = GPIOPin(os.environ.get("LIGHT_ON_OFF_BUTTON_PIN"))
        self._horn_button_pin: GPIOPin = GPIOPin(os.environ.get("HORN_BUTTON_PIN"))
        self._brake_detector_pin: GPIOPin = GPIOPin(os.environ.get("BRAKE_DETECTOR_PIN"))

    @property
    def front_leds_pin(self) -> GPIOPin:
        return self._front_leds_pin
    
    @property
    def back_leds_pin(self) -> GPIOPin:
        return self._back_leds_pin
    
    @property
    def turn_left_signal_button_pin(self) -> GPIOPin:
        return self._turn_left_signal_button_pin
    
    @property
    def turn_right_signal_button_pin(self) -> GPIOPin:
        return self._turn_right_signal_button_pin
    
    @property
    def light_on_off_button_pin(self) -> GPIOPin:
        return self._light_on_off_button_pin
    
    @property
    def horn_button_pin(self) -> GPIOPin:
        return self._horn_button_pin
    
    @property
    def brake_detector_pin(self) -> GPIOPin:
        return self._brake_detector_pin
