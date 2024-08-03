# Python Imports
import os
from threading import Thread

# Packages Imports
import alsaaudio

# Local Imports
from constants import PROGRAM, SOUND_COLLECTION

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
    def __init__(self, sound_collection_directory_path: str = SOUND_COLLECTION.DEFAULT_PATH):
        sound_collection_directory_path = sound_collection_directory_path.format(PROGRAM.PROGRAM_NAME)

        if not os.path.isabs(sound_collection_directory_path):
            raise ValueError(f"\"{sound_collection_directory_path}\" is not an absolute path.")
        
        if not os.path.exists(sound_collection_directory_path):
            raise ValueError(f"\"{sound_collection_directory_path}\" does not exist.")
        
        if not os.path.isdir(sound_collection_directory_path):
            raise ValueError(f"\"{sound_collection_directory_path}\" is not a directory.")
        
        self._collection_directory_path = sound_collection_directory_path


        sounds_paths = []
        sounds_repeatability = []

        for child_name in os.listdir(sound_collection_directory_path):
            child_path = os.path.join(sound_collection_directory_path, child_name)

            if os.path.isdir(child_path):
                continue

            if not os.path.isfile(child_path):
                continue

            if child_name.endswith(SOUND_COLLECTION.SUPPORTED_AUDIO_FILE_EXTENSIONS):

            else
        self._sounds = 

        self._sound_thread = Thread(name="sound", target=self._sound_playing_task)
        self._sound_thread.start()

    def _sound_playing_task(self):


    def _update_sound_collection(self):
        pass

    @property
    def horn(self):
        pass

    @property
    def sound_count(self) -> int:
        pass

# Tests

def command_line_interface_play_test(sound_player: HornSoundPlayer):
    pass

if __name__ == '__main__':
    sound_player = HornSoundPlayer()

    assert sound_player.sound_count == 2, ""

    command_line_interface_play_test(sound_player)