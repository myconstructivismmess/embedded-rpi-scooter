# Python Imports
from typing import Callable

# Project Imports
from _ButtonHandlerBase import ButtonHandlerBase

# Main
class LatchButtonHandler(ButtonHandlerBase):
    def __init__(self, on_value_changed: Callable[[bool], None], start_value: bool=False) -> None:
        super().__init__()
        
        self._value: bool = start_value
        self._on_value_changed: Callable[[bool], None] = on_value_changed

    def on_value_raised(self) -> None:
        if self._enabled:
            self._value = not self._value
            self._on_value_changed(self._value)

    def on_value_recessed(self) -> None:
        pass

    def update(self) -> None:
        pass
