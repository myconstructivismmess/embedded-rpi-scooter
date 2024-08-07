# Python Imports
import os

import re as regex

from queue import Queue

import logging

from threading import Thread

from typing import cast, List, Dict

# Project Imports
from .horn_sound_player_thread_messages import MessageType, Message, StartSoundMessage, StopRepeatableSoundMessage, LogThreadMessage, WarningThreadMessage, ErrorThreadMessage
from .horn_sound_player_thread import horn_sound_player_thread
from .horn_sound_data import HornSoundData

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
            raise ValueError(f"\"{horn_sound_collection_path}\" is not an absolute path.")
        
        if not os.path.exists(horn_sound_collection_path):
            raise ValueError(f"\"{horn_sound_collection_path}\" does not exist.")
        
        if not os.path.isdir(horn_sound_collection_path):
            raise ValueError(f"\"{horn_sound_collection_path}\" is not a directory.")
        
        horn_sound_file_name_pattern = regex.compile(r"^(short\d+\.wav|long\d+(_repeatable)?\.wav)$")
        
        horn_sound_collection_path_content: List[str] = os.listdir(horn_sound_collection_path)
        self._sounds: Dict[int, HornSoundData] = {}
        for child_name in horn_sound_collection_path_content:
            child_path = os.path.join(horn_sound_collection_path, child_name)

            if not os.path.isfile(child_path):
                continue
            if not child_name.endswith(".wav"):
                continue
            
            if not horn_sound_file_name_pattern.match(child_name):
                raise ValueError(f"\"{child_name}\" is not a valid horn sound file name.")
            
            logging.info(f"horn_sound_player: Loading sound file \"{child_name}\"...")

            index: int = -1
            is_long: bool = False
            is_repeatable: bool = False
            if child_name.startswith("short"):
                index: int = int(child_name[5:-4])
            elif child_name.startswith("long"):
                is_long = True
                is_repeatable = child_name.endswith("_repeatable.wav")
                cut_index_left: int = -4 if not is_repeatable else -15
                index: int = int(child_name[4:cut_index_left])

            if index < 0:
                raise ValueError("Hoops! Something went wrong while loading the sound files.")
            
            real_index = (index - 1) * 2 + (1 if is_long else 0)

            if real_index in self._sounds:
                raise ValueError("Hoops! Something went wrong while loading the sound files. A sound is already loaded with the same real index.")

            try:
                self._sounds[real_index] = HornSoundData(
                    child_path,
                    is_repeatable
                )
            except Exception as e:
                raise ValueError(f"An error occurred while loading the sound file \"{child_name}\".") from e
            
            logging.info(f"horn_sound_player: {'Long' if is_long else 'Short'} sound {index} loaded as a {'repeatable' if is_repeatable else 'non-repeatable'} sound with real index {real_index}.")

        missing_sounds: List[int] = []
        for i in range(0, len(self._sounds)):
            if i not in self._sounds:
                missing_sounds.append(i)
        
        if len(missing_sounds) > 0:
            raise ValueError(f"Missing sounds: {', '.join([str(i) for i in missing_sounds])}")

        self._thread_send_message_queue: Queue[Message] = Queue()
        self._thread_receive_message_queue: Queue[Message] = Queue()

        self._thread: Thread = Thread(target=horn_sound_player_thread, daemon=True, name="sound_player", args=(self._thread_send_message_queue, self._thread_receive_message_queue, self._sounds, audio_device))
        self._thread.daemon = True
        self._thread.start()

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
        while not self._thread_receive_message_queue.empty():
            message = self._thread_receive_message_queue.get()
            match message.type:
                case MessageType.LOG:
                    cast(LogThreadMessage, message).log()
                case MessageType.WARNING:
                    cast(WarningThreadMessage, message).log()
                case MessageType.ERROR:
                    cast(ErrorThreadMessage, message).log()

    @property
    def long_sound_repeatability_table(self) -> list[bool]:
        """
        The long sound repeatability table.

        Returns
        -------
        list[bool]
            The long sound repeatability table.
        """
        return self.get_long_sound_repeatability_table()
    def get_long_sound_repeatability_table(self) -> list[bool]:
        """
        Get the long sound repeatability table.

        Returns
        -------
        list[bool]
            The long sound repeatability table.
        """
        return self._long_sound_repeatability_table
    
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
