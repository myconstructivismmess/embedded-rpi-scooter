# Python Imports
from abc import ABC, abstractmethod

# Main
class ButtonHandlerBase(ABC):
    def __init__(self) -> None:
        self._enabled: bool = True

    def enable(self) -> None:
        self._enabled = True
    def disable(self) -> None:
        self._enabled = False
    @property
    def enabled(self) -> bool:
        return self._enabled

    @abstractmethod
    def on_value_raised(self) -> None:
        pass

    @abstractmethod
    def on_value_recessed(self) -> None:
        pass

    @abstractmethod
    def update(self) -> None:
        pass