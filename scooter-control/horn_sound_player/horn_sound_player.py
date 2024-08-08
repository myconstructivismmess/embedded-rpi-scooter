# Python Imports
import os

import re as regex

from logging import warning as log_warning, error as log_error, info as log_info

from threading import Thread

from time import time

from queue import Queue
from typing import cast, List, Dict, Tuple

# Project Imports
from .horn_sound_player_thread_messages import MessageType, Message, StartSoundMessage, StopRepeatableSoundMessage, LogThreadMessage, WarningThreadMessage, ErrorThreadMessage, SoundsLoadedResultMessage
from .horn_sound_player_thread import horn_sound_player_thread

# Class
class HornSoundPlayer:
    """
    Sound player for horn sounds.

    Handles sounds loading and sound playing.

    Parameters
    ----------
    sound_collection_path : str
        The absolute path to the directory containing horn sounds file.\n
        First braces will be replaced by the program name.
    """
    def __init__(self, horn_sound_collection_path: str, audio_device: str):
        if not os.path.isabs(horn_sound_collection_path):
            raise ValueError(f"'{horn_sound_collection_path}' is not an absolute path.")
        
        if not os.path.exists(horn_sound_collection_path):
            raise ValueError(f"'{horn_sound_collection_path}' does not exist.")
        
        if not os.path.isdir(horn_sound_collection_path):
            raise ValueError(f"'{horn_sound_collection_path}' is not a directory.")
        
        pattern = regex.compile(r"^(short\d+\.wav|long\d+(_repeatable)?\.wav)$")
        
        content: List[str] = os.listdir(horn_sound_collection_path)
        sounds_to_load: Dict[int, Tuple[str, bool]] = {}
        self._long_sound_repeatability_table: Dict[int, bool] = {}
        for child_name in content:
            child_path = os.path.join(horn_sound_collection_path, child_name)

            if not os.path.isfile(child_path):
                continue

            if not child_name.endswith(".wav"):
                continue
            
            if not pattern.match(child_name):
                log_warning(f"horn_sound_player: '{child_name}' is not a valid horn sound file name.")
                continue

            index: int = -1
            is_long: bool = False
            is_repeatable: bool = False
            if child_name.startswith("short"):
                index: int = int(child_name[5:-4]) - 1
            elif child_name.startswith("long"):
                is_long = True
                is_repeatable = child_name.endswith("_repeatable.wav")
                cut_index_left: int = -4 if not is_repeatable else -15
                index: int = int(child_name[4:cut_index_left]) - 1

            if index < 0:
                raise log_error(f"horn_sound_player: '{child_name}' is not a valid horn sound file name.")
            
            real_index = index * 2 + (1 if is_long else 0)

            log_info(f"horn_sound_player: {'Long' if is_long else 'Short'} sound at '{child_path}' with index {index} detected as a {'repeatable' if is_repeatable else 'non-repeatable'} sound with real index {real_index}.")

            self._long_sound_repeatability_table[index] = is_repeatable
            sounds_to_load[real_index] = (child_path, is_repeatable)

        self._thread_send_message_queue: Queue[Message] = Queue()
        self._thread_receive_message_queue: Queue[Message] = Queue()

        self._thread: Thread = Thread(
            target=horn_sound_player_thread,
            daemon=True,
            name="sound_player",
            args=(
                self._thread_send_message_queue,
                self._thread_receive_message_queue,
                sounds_to_load,
                audio_device
            )
        )
        self._thread.start()

        current_time = time()
        sounds_loaded_result_message: SoundsLoadedResultMessage | None = None
        log_info("horn_sound_player: Waiting for sound thread to answer with sounds loading results...")
        while time() - current_time < 15:
            if self._thread_receive_message_queue.empty():
                continue

            update_result = self._update()

            if update_result is not None:
                sounds_loaded_result_message = update_result
                break

        if sounds_loaded_result_message is None:
            raise RuntimeError("horn_sound_player: Sound thread did not answer in time.")
        
        for index, is_success in sounds_loaded_result_message.sounds_load_success_status.items():
            if not is_success:
                log_error(f"horn_sound_player: Sound with index {index} failed to load.")
            else:
                log_info(f"horn_sound_player: Sound with index {index} loaded successfully.")

        self._sounds_load_success_status: Dict[int, bool] = sounds_loaded_result_message.sounds_load_success_status.copy()

    def play_sound(self, index: int) -> None:
        """
        Play a sound.

        Parameters
        ----------
        index : int
            The index of the sound to play.
        """
        self._thread_send_message_queue.put(StartSoundMessage(index))
    
    def stop_repeatable_sound(self) -> None:
        """
        Stop the current repeatable sound.
        """
        self._thread_send_message_queue.put(StopRepeatableSoundMessage())


    def update(self):
        """
        Update the sound player.

        This function is mainly used to log errors, warnings and messages from the internal thread.
        """
        self._update()

    def _update(self) -> StartSoundMessage | None:
        while not self._thread_receive_message_queue.empty():
            message = self._thread_receive_message_queue.get()
            match message.type:
                case MessageType.LOG:
                    cast(LogThreadMessage, message).log()
                case MessageType.WARNING:
                    cast(WarningThreadMessage, message).log()
                case MessageType.ERROR:
                    cast(ErrorThreadMessage, message).log()
                case MessageType.SOUNDS_LOADED_RESULT:
                    sounds_loaded_result_message = cast(SoundsLoadedResultMessage, message)
                    return sounds_loaded_result_message
        return None

    @property
    def long_sound_repeatability_table(self) -> Dict[int, bool]:
        """
        The long sound repeatability table.

        Returns
        -------
        list[bool]
            The long sound repeatability table.
        """
        return self.get_long_sound_repeatability_table()
    def get_long_sound_repeatability_table(self) -> Dict[int, bool]:
        """
        Get the long sound repeatability table.

        Returns
        -------
        list[bool]
            The long sound repeatability table.
        """
        return self._long_sound_repeatability_table.copy()
    
    @property
    def sounds_load_success_status(self) -> Dict[int, bool]:
        """
        The sounds load success status.

        Returns
        -------
        Dict[int, bool]
            The sounds load success status.
        """
        return self.get_sounds_load_success_status()
    def get_sounds_load_success_status(self) -> Dict[int, bool]:
        """
        Get the sounds load success status.

        Returns
        -------
        Dict[int, bool]
            The sounds load success status.
        """
        return self._sounds_load_success_status.copy()

    @property
    def sound_count(self) -> int:
        """
        The number of sounds.

        Returns
        -------
        int
            The number of sounds.
        """
        return self.get_sound_count()
    def get_sound_count(self) -> int:
        """
        Get the number of sounds.

        Returns
        -------
        int
            The number of sounds.
        """
        return len(self._sounds)
