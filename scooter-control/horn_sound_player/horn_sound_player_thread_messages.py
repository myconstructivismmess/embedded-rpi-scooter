# Python Imports
import logging

from enum import Enum
from typing import Dict

# Main
class MessageType(Enum):
    SOUNDS_LOADED_RESULT = 1
    START_SOUND = 2
    STOP_REPEATABLE_SOUND = 3
    LOG = 4
    WARNING = 5
    ERROR = 6

class Message:
    def __init__(self, type: MessageType) -> None:
        self._type = type

    @property
    def type(self) -> MessageType:
        return self._type
    
class SoundsLoadedResultMessage(Message):
    def __init__(self, sounds_load_success_status: Dict[int, bool]) -> None:
        super().__init__(MessageType.SOUNDS_LOADED_RESULT)
        self._sounds_load_success_status: Dict[int, bool] = sounds_load_success_status

    def is_sound_loaded(self, index: int) -> bool:
        return self._sounds_load_success_status.get(index, False)
    
    @property
    def sounds_load_success_status(self) -> Dict[int, bool]:
        return self._sounds_load_success_status.copy()

class StartSoundMessage(Message):
    def __init__(self, index: int) -> None:
        super().__init__(MessageType.START_SOUND)
        self._index: int = index

    @property
    def index(self) -> int:
        return self._index

class StopRepeatableSoundMessage(Message):
    def __init__(self) -> None:
        super().__init__(MessageType.STOP_REPEATABLE_SOUND)


class LogThreadMessage(Message):
    def __init__(self, log_message: str) -> None:
        super().__init__(MessageType.LOG)
        self._log_message: str = log_message

    def log(self) -> None:
        logging.info(f"horn_sound_player: {self._log_message}")

class WarningThreadMessage(Message):
    def __init__(self, warning_message: str) -> None:
        super().__init__(MessageType.WARNING)
        self._warning_message: str = warning_message

    def log(self) -> None:
        logging.warning(f"horn_sound_player: {self._warning_message}")

class ErrorThreadMessage(Message):
    def __init__(self, exception: Exception) -> None:
        super().__init__(MessageType.ERROR)
        self._exception: Exception = exception

    def log(self) -> None:
        logging.error(f"horn_sound_player: {self._exception}")
