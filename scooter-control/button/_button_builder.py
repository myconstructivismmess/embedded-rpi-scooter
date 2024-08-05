# Python Imports
from typing import List, Callable, Dict

# Project Imports
from utils import find_index

from _button import Button

from ._button_interceptors import ButtonInterceptorBase, GPIOPinButtonInterceptor, KeyboardButtonInterceptor
from ._button_handlers import ButtonHandlerBase, DefaultButtonHandler, LatchButtonHandler

from ._button_options_type import ButtonOptionsType

# Functions
def is_interceptor_gpio_pin_equal(interceptor: GPIOPinButtonInterceptor | KeyboardButtonInterceptor, gpio_pin: Pin) -> bool:
    if isinstance(interceptor, KeyboardButtonInterceptor):
        return False
    
    return interceptor.gpio_pin == gpio_pin

def is_interceptor_key_name_equal(interceptor: GPIOPinButtonInterceptor | KeyboardButtonInterceptor, key_name: str) -> bool:
    if isinstance(interceptor, GPIOPinButtonInterceptor):
        return False
    
    return interceptor.key_name == key_name

# Main
class ButtonBuilder:
    def __init__(self) -> None:
        # Interceptors
        self._gpio_pin_interceptors: List[GPIOPinButtonInterceptor] = []
        self._keyboard_interceptors: List[KeyboardButtonInterceptor] = []

        # Options
        self._options: Dict[ButtonOptionsType, bool] = {
            ButtonOptionsType.FORCE_INIT_DOWN: False,
            ButtonOptionsType.FORCE_INIT_UP: False,
        }

        # Handler
        self._handler: ButtonHandlerBase = None

    # Interceptors
    def add_gpio_pin_interceptor(self, gpio_pin: Pin, reserse_signal: bool=False) -> None:
        self._gpio_pin_interceptors.append(GPIOPinButtonInterceptor(gpio_pin, reserse_signal))
    def remove_gpio_pin_interceptor(self, gpio_pin: Pin) -> None:
        index = find_index(self._gpio_pin_interceptors, lambda interceptor: is_interceptor_gpio_pin_equal(interceptor, gpio_pin))
        if index != 1:
            self._gpio_pin_interceptors.pop(index)

    def add_keyboard_interceptor(self, key_name: str, reserse_signal: bool=False) -> None:
        self._keyboard_interceptors.append(KeyboardButtonInterceptor(key_name, reserse_signal))
    def remove_keyboard_interceptor(self, key_name: str) -> None:
        index = find_index(self._keyboard_interceptors, lambda interceptor: is_interceptor_key_name_equal(interceptor, key_name))
        if index != 1:
            self._keyboard_interceptors.pop(index)

    # Options
    def define_option_state(self, option_type: ButtonOptionsType, state: bool) -> None:
        self._options[option_type] = state
        match option_type:
            case ButtonOptionsType.FORCE_INIT_UP:
                if (state and self._options[ButtonOptionsType.FORCE_INIT_DOWN]):
                    print("WARN: FORCE_INIT_DOWN is active but you enabled FORCE_INIT_UP, FORCE_INIT_DOWN will be disabled.")
                    self._options[ButtonOptionsType.FORCE_INIT_DOWN] = False
            case ButtonOptionsType.FORCE_INIT_DOWN:
                if (state and self._options[ButtonOptionsType.FORCE_INIT_UP]):
                    print("WARN: FORCE_INIT_UP is active but you enabled FORCE_INIT_DOWN, FORCE_INIT_UP will be disabled.")
                    self._options[ButtonOptionsType.FORCE_INIT_UP] = False
    def enable_option(self, option_type: ButtonOptionsType) -> None:
        self.define_option_state(option_type, True)
    def disable_option(self, option_type: ButtonOptionsType) -> None:
        self.define_option_state(option_type, False)

    # Handler
    def set_handler(self, handler: ButtonHandlerBase) -> None:
        if self._handler != None:
            print("WARN: BUTTON_BUILDER: A handler is already set; the previous handler will be replaced by this one.")
        self._handler = handler
    def set_default_button_handler(self, on_value_changed: Callable[[bool], None]) -> DefaultButtonHandler:
        handler: DefaultButtonHandler = DefaultButtonHandler(on_value_changed)
        self.set_handler(handler)
        return handler
    def set_latch_button_handler(self, on_value_changed: Callable[[bool], None], start_value: bool=False, switch_on_recess: bool=False) -> LatchButtonHandler:
        handler: LatchButtonHandler = LatchButtonHandler(on_value_changed, start_value, switch_on_recess)
        self.set_handler(handler)
        return handler

    # Build
    def build(self) -> Button:
        if self._handler == None:
            raise ValueError("Handler not setup.")
        interceptors: List[ButtonInterceptorBase] = self._gpio_pin_interceptors + self._keyboard_interceptors
        button = Button(interceptors, self._handler, self._options)
        return button
