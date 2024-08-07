# Python Imports
from typing import Callable, List

# Project Imports
from .button.handlers import ButtonHandlerBase

from .timer import Timer

# Main
class HornButtonHandler(ButtonHandlerBase):
    def __init__(self, timer: Timer, on_horn_start: Callable[[int], None], on_horn_stop: Callable[[], None], sound_count: int, long_sound_repeatability_table: List[bool], max_short_press_duration: float, max_press_interval_duration: float) -> None:
        super().__init__()

        self._real_value: bool = False
        self._value: bool = False

        self._timer: Timer = timer

        self._on_horn_start: Callable[[int], None] = on_horn_start
        self._on_horn_stop: Callable[[], None] = on_horn_stop

        if sound_count < 1:
            raise ValueError("sound_count must be greater than 0")
        
        self._long_press_sound_count: int = sound_count // 2
        self._short_press_sound_count: int = sound_count - self._long_press_sound_count
        self._is_last_possible_press_short: bool = self._short_press_sound_count > self._long_press_sound_count
        
        if self._long_press_sound_count < len(long_sound_repeatability_table):
            raise ValueError("Too long sound repeatability array")

        while len(long_sound_repeatability_table) < self._long_press_sound_count:
            long_sound_repeatability_table.append(False)

        self._long_sound_repeatability_table: List[bool] = long_sound_repeatability_table

        self._max_short_press_duration: float = max_short_press_duration
        self._max_press_interval_duration: float = max_press_interval_duration
        
        self._press_count: int = 0
        self._last_release_time_seconds: float = 0.0
        self._last_press_time_seconds: float = 0.0
        self._repeatable_sound_initiated: bool = False


    def on_enable(self) -> None:
        pass
    def on_disable(self) -> None:
        if self._value:
            self._value = False
            self._on_value_changed(False)


    def on_value_raised(self) -> None:
        if self.enabled:
            self._value = True
            self._on_value_changed(True)
    def on_value_recessed(self) -> None:
        if self._value:
            self._value = False
            self._on_value_changed(False)


    def _on_value_changed(self, value: bool) -> None:
        time_seconds: float = self._timer.time_seconds
        if value:
            self._last_press_time_seconds = time_seconds
            self._press_count += 1
            if self._is_last_possible_press_short:
                if self._press_count == self._short_press_sound_count:
                    # Last possible press can only be short so it triggers immediately
                    sound_index: int = (self._press_count - 1) * 2
                    self._press_count = 0
                    self._on_horn_start(sound_index)
        else:
            self._last_release_time_seconds = time_seconds

            if self._repeatable_sound_initiated:
                # A repeatable sound with a long press has been initiated and it should be stopped when the button is released
                self._repeatable_sound_initiated = False
                self._on_horn_stop()

    def update(self) -> None:
        if self._press_count == 0:
            return

        time_seconds: float = self._timer.time_seconds
        
        if self._value:
            time_since_press_initiated_seconds: float = time_seconds - self._last_press_time_seconds
            if time_since_press_initiated_seconds >= self._max_short_press_duration:
                # The press has been too long to be a short press so it's the final long press
                is_repeatable: bool = self._long_sound_repeatability_table[self._press_count - 1]
                sound_index: int = (self._press_count - 1) * 2 + 1
                self._press_count = 0
                if is_repeatable:
                    self._repeatable_sound_initiated = True
                self._on_horn_start(sound_index)
        else:
            time_since_release_initiated_seconds: float = time_seconds - self._last_release_time_seconds
            if time_since_release_initiated_seconds >= self._max_press_interval_duration:
                # The button has been released for too long so it's short press sound
                sound_index: int = (self._press_count - 1) * 2
                self._press_count = 0
                self._on_horn_start(sound_index)                
