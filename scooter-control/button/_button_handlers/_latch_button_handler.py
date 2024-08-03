# Python Imports
from typing import Callable

# Project Imports
from ._button_handler_base import ButtonHandlerBase

# Main
class LatchButtonHandler(ButtonHandlerBase):
    def __init__(self, on_value_changed: Callable[[bool], None], start_value: bool=False) -> None:
        super().__init__()
        
        self._value: bool = start_value
        self._on_value_changed: Callable[[bool], None] = on_value_changed

    def on_value_raised(self) -> None:
        if self.enabled:
            self.set_value(not self._value)
    def on_value_recessed(self) -> None:
        pass

    def update(self) -> None:
        pass

    @property
    def value(self) -> bool:
        return self.get_value()
    def get_value(self) -> bool:
        return self._value
    def set_value(self, value: bool, trigger_on_value_change_callback: bool = True) -> None:
        self._value = value
        if trigger_on_value_change_callback:
            self._on_value_changed(self._value)
