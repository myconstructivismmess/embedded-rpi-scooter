# Python Imports
from abc import ABC, abstractmethod

# Main
class ButtonInterceptorBase(ABC):
    def __init__(self) -> None:
        pass

    @property
    @abstractmethod
    def value(self) -> bool:
        return self.get_value()
    @abstractmethod
    def get_value(self) -> bool:
        return False