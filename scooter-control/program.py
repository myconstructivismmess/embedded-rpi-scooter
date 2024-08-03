# Python Imports
from abc import ABC, abstractmethod

# Main
class Program(ABC):
    def __init__(self) -> None:
        self._should_exit: bool = False

    @property
    def should_exit(self) -> bool:
        return self._should_exit
    
    def quit(self) -> None:
        self._should_exit = True

    @abstractmethod
    def update(self) -> None:
        pass