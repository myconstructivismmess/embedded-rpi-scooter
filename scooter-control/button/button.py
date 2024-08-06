# Python Imports
from typing import List, Dict

# Project Imports
from .interceptors import ButtonInterceptorBase
from .handlers import ButtonHandlerBase

from .button_options_type import ButtonOptionsType

# Main
class Button:
    def __init__(self, interceptors: List[ButtonInterceptorBase], handler: ButtonHandlerBase, options: Dict[ButtonOptionsType, bool]):
        self._interceptors: List[ButtonInterceptorBase] = interceptors
        self._interceptors_count: int = len(interceptors)
        self._handler: ButtonHandlerBase = handler
        
        force_init_down: bool = options.get(ButtonOptionsType.FORCE_INIT_DOWN, False)
        force_init_up: bool = options.get(ButtonOptionsType.FORCE_INIT_UP, False)

        if force_init_down:
            self._interceptors_values: List[bool] = [False for _ in range(self._interceptors_count)]
        elif force_init_up:
            self._interceptors_values: List[bool] = [True for _ in range(self._interceptors_count)]
        else:
            self._interceptors_values: List[bool] = [interceptors[i].value for i in range(self._interceptors_count)]
        
        self._value: bool = self._interceptors_values.count(True)

    def update(self):
        self._interceptors_values = [self._interceptors[i].value for i in range(self._interceptors_count)]

        newValue: bool = self._interceptors_values.count(True)

        if (newValue and not self._value):
            self._handler.on_value_raised()
        elif (not newValue and self._value):
            self._handler.on_value_recessed()

        self._value = newValue

        self._handler.update()

    def disable_handler(self):
        self._handler.disable()
    def enable_handler(self):
        self._handler.enable()
