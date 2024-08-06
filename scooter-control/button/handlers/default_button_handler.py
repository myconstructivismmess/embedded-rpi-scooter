# Python Imports
from typing import Callable

# Project Imports
from .button_handler_base import ButtonHandlerBase

# Main
class DefaultButtonHandler(ButtonHandlerBase):
    def __init__(self, on_value_changed: Callable[[bool], None]) -> None:
        super().__init__()
        
        self._real_value: bool = False
        self._value: bool = False
        self._on_value_changed: Callable[[bool], None] = on_value_changed

    def on_enable(self) -> None:
        if self._real_value != self._value:
            self._value = self._real_value
            self._on_value_changed(self._value)
    def on_disable(self) -> None:
        pass

    def on_value_raised(self) -> None:
        self._real_value = True
        if self.enabled:
            self._value = True
            self._on_value_changed(True)
    def on_value_recessed(self) -> None:
        self._real_value = False
        if self.enabled:
            self._value = False
            self._on_value_changed(False)

    def update(self) -> None:
        pass
