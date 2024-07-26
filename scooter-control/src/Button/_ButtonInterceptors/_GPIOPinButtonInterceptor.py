# Packages Imports
from digitalio import DigitalInOut, Pull
from microcontroller import Pin

# Project Imports
from _ButtonInterceptorBase import ButtonInterceptorBase

# Main
class GPIOPinButtonInterceptor(ButtonInterceptorBase):
    def __init__(self, gpio_pin: Pin) -> None:
        self._gpio_pin: Pin = gpio_pin

        self._digital_input: DigitalInOut = DigitalInOut(gpio_pin)
        self._digital_input.switch_to_input(pull=Pull.DOWN)

    @property
    def value(self) -> bool:
        return self.get_value()
    def get_value(self) -> bool:
        return self._digital_input.value
    
    @property
    def gpio_pin(self) -> Pin:
        return self.get_gpio_pin()
    def get_gpio_pin(self) -> Pin:
        return self._gpio_pin
        