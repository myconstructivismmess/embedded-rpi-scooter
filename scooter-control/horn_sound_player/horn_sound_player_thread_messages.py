# Python Imports
from enum import Enum

import logging

# Main
class MessageType(Enum):
    START_SOUND = 1
    STOP_REPEATABLE_SOUND = 2
    LOG = 3
    WARNING = 4
    ERROR = 5

class Message:
    def __init__(self, type: MessageType) -> None:
        self._type = type

    @property
    def type(self) -> MessageType:
        return self._type


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
        logging.info(self._log_message)

class WarningThreadMessage(Message):
    def __init__(self, warning_message: str) -> None:
        super().__init__(MessageType.WARNING)
        self._warning_message: str = warning_message

    def log(self) -> None:
        logging.warning(self._warning_message)

class ErrorThreadMessage(Message):
    def __init__(self, exception: Exception) -> None:
        super().__init__(MessageType.ERROR)
        self._exception: Exception = exception

    def log(self) -> None:
        logging.error(self._exception)
