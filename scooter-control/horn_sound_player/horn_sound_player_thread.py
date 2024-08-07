# Python Imports
from queue import Queue

from typing import cast, Dict

# Packages Imports
import alsaaudio

# Project Imports
from .horn_sound_player_thread_messages import Message, MessageType, LogThreadMessage, WarningThreadMessage, ErrorThreadMessage, StartSoundMessage, StopRepeatableSoundMessage
from .horn_sound_data import HornSoundData

# Main
def horn_sound_player_thread(thread_receive_message_queue: Queue[Message], thread_send_message_queue: Queue[Message], sounds: Dict[int, HornSoundData], audio_device: str) -> None:
    current_sound: HornSoundData | None = None

    device: alsaaudio.PCM = alsaaudio.PCM(channels=sounds[0].channels, rate=sounds[0].period_size, format=sounds[0].format, periodsize=sounds[0].period_size, device=audio_device)

    while True:
        if isinstance(current_sound, HornSoundData):
            if current_sound.finished:
                current_sound.reset()
                current_sound = None
            else:
                current_sound.play_frame(device)

        if thread_receive_message_queue.empty():
            continue

        message: Message = thread_receive_message_queue.get()

        match message.type:
            case MessageType.START_SOUND:
                start_sound_message: StartSoundMessage = cast(StartSoundMessage, message)
                current_sound = sounds.get(start_sound_message.index, None)
                if isinstance(current_sound, None):
                    thread_send_message_queue.put(ErrorThreadMessage(f"Sound not found for index {start_sound_message.index}."))
                    continue
                thread_send_message_queue.put(LogThreadMessage(f"Playing sound {start_sound_message.index}."))
                current_sound.define_device_properties(device)
                current_sound.reset()
            case MessageType.STOP_REPEATABLE_SOUND:
                if isinstance(current_sound, HornSoundData):
                    current_sound.reset()
                    current_sound = None
        