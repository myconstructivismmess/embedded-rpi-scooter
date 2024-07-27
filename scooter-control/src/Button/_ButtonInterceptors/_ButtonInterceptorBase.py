# Python Imports
from abc import ABC, abstractmethod

# Main
class ButtonInterceptorBase(ABC):
    def __init__(self, reverse_signal: bool=False) -> None:
        self._reverse_signal = reverse_signal

    @property
    def value(self) -> bool:
        return self.get_value()
    def get_value(self) -> bool:
        if self._reverse_signal:
            return not self._get_value()
        
        return self._get_value()
    
    @abstractmethod
    def _get_value() -> bool:
        return False