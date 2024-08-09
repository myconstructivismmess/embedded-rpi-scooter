# Python Imports
import wave

from typing import List
from queue import Queue

# Packages Imports
import alsaaudio

# Project Imports
from .horn_sound_player_thread_messages import Message, WarningThreadMessage, ErrorThreadMessage, LogThreadMessage

# Main
class HornSoundData:
    def __init__(self, path: str, is_repeatable: bool) -> None:
        with wave.open(path, "rb") as sound_file:
            # Repeatability
            self._repeatable: bool = is_repeatable

            # Channels
            self._channels: int = sound_file.getnchannels()

            # Framerate
            self._framerate: int = sound_file.getframerate()

            # Period Size
            self._period_size: int = self._framerate // 8

            # Sample Width
            self._sample_width: int = sound_file.getsampwidth()

            # 8bit is unsigned in wav files
            if self._sample_width == 1:
                self._format: int = alsaaudio.PCM_FORMAT_U8
            # Otherwise we assume signed data, little endian
            elif self._sample_width == 2:
                self._format: int = alsaaudio.PCM_FORMAT_S16_LE
            elif self._sample_width == 3:
                self._format: int = alsaaudio.PCM_FORMAT_S24_3LE
            elif self._sample_width == 4:
                self._format: int = alsaaudio.PCM_FORMAT_S32_LE
            else:
                raise ValueError('Unsupported format')
        
            # Save Data
            self._sound_data: List[bytes] = []

            data_block: bytes = sound_file.readframes(self._period_size)
            while data_block:
                self._sound_data.append(data_block)
                data_block = sound_file.readframes(self._period_size)

            # Data Frames
            self._current_frame: int = 0
            self._frame_count: int = len(self._sound_data)

    def define_device_properties(self, device: alsaaudio.PCM) -> None: # NOTE: This method as probably a problem
        device.setchannels(self._channels)
        device.setrate(self._framerate)
        device.setformat(self._format)
        device.setperiodsize(self._framerate)
                
    def play_frame(self, device: alsaaudio.PCM, thread_send_message_queue: Queue[Message]) -> None:
        data = self._sound_data[self._current_frame]

        if device.write(data) < 0:
            thread_send_message_queue.put(WarningThreadMessage("Playback buffer underrun! Continuing nonetheless ..."))
        
        if self._current_frame < (self._frame_count - 1):
            self._current_frame += 1
            
        elif self._repeatable and self._current_frame >= (self._frame_count - 1):
            self._current_frame = 0

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
        
        return self._current_frame >= (self._frame_count - 1)
