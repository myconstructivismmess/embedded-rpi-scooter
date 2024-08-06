# Python Imports
from abc import ABC, abstractmethod

# Project Imports
from .button import Button, ButtonBuilder

# Main
class Program(ABC):
    def __init__(self, initialize_quit_button: bool = False) -> None:
        self._should_exit: bool = False

        if initialize_quit_button:
            def on_quit_button_value_change(value: bool) -> None:
                if value:
                    self.quit()
                    print("") # TODO: Check later why this is needed

            quit_button_builder = ButtonBuilder()
            quit_button_builder.add_keyboard_interceptor('q')
            quit_button_builder.set_default_button_handler(on_quit_button_value_change)
            self._quit_button: Button = quit_button_builder.build()
        else:
            self._quit_button: None = None

    @property
    def should_exit(self) -> bool:
        return self._should_exit
    
    def quit(self) -> None:
        self._should_exit = True

    def run_until_quit(self) -> None:
        while not self.should_exit:
            if isinstance(self._quit_button, Button):
                self._quit_button.update()
            self.update()

    @abstractmethod
    def update(self) -> None:
        pass
