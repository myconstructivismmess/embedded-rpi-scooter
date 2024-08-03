# Packages Imports
import keyboard

# Project Imports
from ._button_interceptor_base import ButtonInterceptorBase

# Main
class KeyboardButtonInterceptor(ButtonInterceptorBase):
    def __init__(self, key_name: str, reverse_signal: bool=False) -> None:
        super().__init__(reverse_signal)
        self._key_name: str = key_name

    def _get_value(self) -> bool:
        return keyboard.is_pressed(self._key_name)
    
    @property
    def key_name(self) -> str:
        return self.get_key_name()
    def get_key_name(self) -> str:
        return self._key_name
    