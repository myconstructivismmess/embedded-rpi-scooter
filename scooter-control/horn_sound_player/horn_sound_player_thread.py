# Python Imports
from queue import Queue

from typing import cast, Dict, Tuple

# Packages Imports
import alsaaudio

# Project Imports
from .horn_sound_player_thread_messages import Message, MessageType, LogThreadMessage, WarningThreadMessage, ErrorThreadMessage, StartSoundMessage, SoundsLoadedResultMessage
from .horn_sound_data import HornSoundData

from time import time, sleep

# Main
def horn_sound_player_thread(thread_receive_message_queue: Queue[Message], thread_send_message_queue: Queue[Message], sounds_to_load: Dict[int, Tuple[str, bool]], audio_device: str) -> None:
    current_sound: HornSoundData | None = None

    sounds: Dict[int, HornSoundData] = {}
    sounds_load_success_status: Dict[int, bool] = {}
    for index, (sound_path, is_repeatable) in sounds_to_load.items():
        try:
            sounds[index] = HornSoundData(
                sound_path,
                is_repeatable
            )
            sounds_load_success_status[index] = True
        except Exception as e:
            sounds[index] = None
            sounds_load_success_status[index] = False
            thread_send_message_queue.put(ErrorThreadMessage(e))

    thread_send_message_queue.put(LogThreadMessage("Connecting to audio device..."))
    device = alsaaudio.PCM(alsaaudio.PCM_PLAYBACK, device=audio_device)
    thread_send_message_queue.put(LogThreadMessage("Connected to audio device."))

    thread_send_message_queue.put(SoundsLoadedResultMessage(sounds_load_success_status))

    while True:
        try:
            if isinstance(current_sound, HornSoundData):
                if current_sound.finished:
                    current_sound.reset()
                    current_sound = None
                else:
                    current_sound.play_frame(device, thread_send_message_queue)

            if thread_receive_message_queue.empty():
                continue

            message: Message = thread_receive_message_queue.get()

            match message.type:
                case MessageType.START_SOUND:
                    start_sound_message: StartSoundMessage = cast(StartSoundMessage, message)

                    if isinstance(current_sound, HornSoundData):
                        current_sound.reset()
                        current_sound = None

                    current_sound = sounds.get(start_sound_message.index, None)

                    if current_sound is None:
                        thread_send_message_queue.put(ErrorThreadMessage(f"Sound not found for index {start_sound_message.index}."))
                        continue

                    current_sound.define_device_properties(device)
                    current_sound.reset()

                    thread_send_message_queue.put(LogThreadMessage(f"Playing sound {start_sound_message.index}."))
                case MessageType.STOP_REPEATABLE_SOUND:
                    if isinstance(current_sound, HornSoundData):
                        current_sound.reset()
                        current_sound = None

                        thread_send_message_queue.put(LogThreadMessage("Stop repeatable sound."))

        except Exception as e:
            thread_send_message_queue.put(ErrorThreadMessage(f"Error in sound player thread: {e}"))
        