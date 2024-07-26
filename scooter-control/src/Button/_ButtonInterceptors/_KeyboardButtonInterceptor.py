# Packages Imports
import keyboard

# Project Imports
from _ButtonInterceptorBase import ButtonInterceptorBase

# Main
class KeyboardButtonInterceptor(ButtonInterceptorBase):
    def __init__(self, key_name: str) -> None:
        self._key_name: str = key_name

    @property
    def value(self) -> bool:
        return self.get_value()
    def get_value(self) -> bool:
        return keyboard.is_pressed(self._key_name)
    
    @property
    def key_name(self) -> str:
        return self.get_key_name()
    def get_key_name(self) -> str:
        return self._key_name