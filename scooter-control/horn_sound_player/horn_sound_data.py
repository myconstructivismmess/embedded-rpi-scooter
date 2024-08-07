# Python Imports
import wave

from typing import List

from queue import Queue

# Packages Imports
import alsaaudio

# Project Imports
from .horn_sound_player_thread_messages import Message, MessageType, LogThreadMessage, WarningThreadMessage, ErrorThreadMessage, StartSoundMessage, StopRepeatableSoundMessage

# Main
class HornSoundData:
    def __init__(self, path: str, is_repeatable: bool) -> None:
        self._repeatable: bool = is_repeatable
        self._channels: int = 1
        self._nframes: int = 0
        self._period_size: int = 0
        self._sound_data: List[bytes] = []
        self._sound_format = None

        self._current_frame: int = 0

        with wave.open(path, "rb") as sound_file:
            self._nframes = sound_file.getnframes()
            self._period_size = sound_file.getframerate()
            self._channels = sound_file.getnchannels()

            data_block: bytes = sound_file.readframes(self._period_size)
            while data_block:
                self._sound_data.append(data_block)
                data_block = sound_file.readframes(self._period_size)

            match sound_file.getsampwidth():
                case 1:
                    self._sound_format = alsaaudio.PCM_FORMAT_U8
                case 2:
                    self._sound_format = alsaaudio.PCM_FORMAT_S16_LE
                case 3:
                    self._sound_format = alsaaudio.PCM_FORMAT_S24_3LE
                case 4:
                    self._sound_format = alsaaudio.PCM_FORMAT_S32_LE
                case _:
                    raise ValueError('Unsupported format')
                
    def define_device_properties(self, device: alsaaudio.PCM) -> None:
        device.setchannels(self._channels)
        device.setrate(self._period_size)
        device.setformat(self._sound_format)
        device.setperiodsize(self._period_size)
                
    def play_frame(self, device: alsaaudio.PCM, thread_send_message_queue: Queue[Message]) -> None:
        data = self._sound_data[self._current_frame]

        if device.write(data) < 0:
            thread_send_message_queue.warning("Playback buffer underrun! Continuing nonetheless ...")
        
        if self._current_frame >= self._nframes:
            if self._repeatable:
                self._current_frame = 0
        else:
            self._current_frame += 1

    def reset(self) -> None:
        self._current_frame = 0

    @property
    def repeatable(self) -> bool:
        return self.is_repeatable()
    def is_repeatable(self) -> bool:
        return self._repeatable
    
    @property
    def finished(self) -> bool:
        return self.is_finished()
    def is_finished(self) -> bool:
        if self._repeatable:
            return False
        
        return self._current_frame >= self._nframes
    
    @property
    def channels(self) -> int:
        return self.get_channels()
    def get_channels(self) -> int:
        return self._channels
    
    @property
    def period_size(self) -> int:
        return self.get_period_size()
    def get_period_size(self) -> int:
        return self._period_size
    
    @property
    def nframes(self) -> int:
        return self.get_nframes()
    def get_nframes(self) -> int:
        return self._nframes
    
    @property
    def format(self) -> int:
        return self.get_format()
    def get_format(self) -> int:
        return self._sound_format
