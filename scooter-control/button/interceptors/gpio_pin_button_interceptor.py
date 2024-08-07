# Python Imports
import logging

# Packages Imports
try:
    from digitalio import DigitalInOut, Pull
    MICROCONTROLLER_MODULE_LOADED = True
except ImportError:
    logging.warning("imports: The 'digitalio' module cannot be imported; this is normal in a development environment (outside of a board environment).")
    MICROCONTROLLER_MODULE_LOADED = False

# Project Imports
from .button_interceptor_base import ButtonInterceptorBase

from ...gpio_pin import GPIOPin

# Main
class GPIOPinButtonInterceptor(ButtonInterceptorBase):
    def __init__(self, gpio_pin: GPIOPin, reverse_signal: bool=False) -> None:
        super().__init__(reverse_signal)

        self._gpio_pin: GPIOPin = gpio_pin

        if MICROCONTROLLER_MODULE_LOADED and gpio_pin.microcontroller_module_loaded:
            try:
                self._digital_input: DigitalInOut = DigitalInOut(gpio_pin.get_as_microcontroller_pin())
                self._digital_input.switch_to_input(pull=Pull.DOWN)
            except:
                self._digital_input = None
    
    def _get_value(self) -> bool:
        if self._digital_input is None:
            return False
        return self._digital_input.value
    
    @property
    def gpio_pin(self) -> GPIOPin:
        return self.get_gpio_pin()
    def get_gpio_pin(self) -> GPIOPin:
        return self._gpio_pin
        