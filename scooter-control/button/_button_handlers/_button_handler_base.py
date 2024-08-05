# Python Imports
from abc import ABC, abstractmethod

# Main
class ButtonHandlerBase(ABC):
    def __init__(self) -> None:
        self._enabled: bool = True

    def enable(self) -> None:
        self._enabled = True
        self.on_enable()
    def disable(self) -> None:
        self._enabled = False
        self.on_disable()
    @property
    def enabled(self) -> bool:
        return self._enabled
    
    @abstractmethod
    def on_enable(self) -> None:
        pass
    @abstractmethod
    def on_disable(self) -> None:
        pass

    @abstractmethod
    def on_value_raised(self) -> None:
        pass
    @abstractmethod
    def on_value_recessed(self) -> None:
        pass

    @abstractmethod
    def update(self) -> None:
        pass