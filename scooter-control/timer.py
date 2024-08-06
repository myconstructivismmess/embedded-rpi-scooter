# Python Imports
from time import time
from math import trunc
from typing import List

# Main
class TimerHandler:
    """
    A class meant to be used with the Timer class to track and manage elapsed time.

    This class provides functionality to track elapsed time, reset the time, update the time, 
    and consume time in discrete blocks. The concept of consuming blocks of time is particularly 
    useful in scenarios where tasks or events need to be triggered at regular intervals.
    """
    def __init__(self):
        self._elapsed_time_seconds: float = 0

    def reset(self):
        """
        Reset the elapsed time to zero.
        """
        self._elapsed_time_seconds = 0
    
    def _update(self, elapsed_time_seconds: float):
        """
        Update the elapsed time.

        (This method is meant to be called by the Timer class)

        Parameters:
        -----------
        elapsed_time_seconds : float
            The amount of time to add to the elapsed time.
        """
        self._elapsed_time_seconds += elapsed_time_seconds

    def consume_available_time_blocks(self, block_duration_seconds: int):
        """
        Consume available time blocks and return the count of full blocks consumed.

        Parameters:
        -----------
        block_duration_seconds : int
            The duration of each time block in seconds.

        Returns:
        --------
        int
            The number of complete blocks of time that have been consumed.

        This method calculates how many complete blocks of the specified duration can fit 
        into the currently elapsed time, subtracts the equivalent duration of these blocks 
        from the elapsed time, and returns the count of these blocks.
        """
        count: float = self._elapsed_time_seconds // block_duration_seconds
        self._elapsed_time_seconds -= (count * block_duration_seconds)
        return trunc(count)

class Timer: 
    """
    A class to manage multiple TimerHandler instances and update them based on the elapsed time.
    """
    def __init__(self):
        self._time_seconds: float = time()
        self._handlers: List[TimerHandler] = []

    def update(self):
        """
        Updates the elapsed time for all registered TimerHandler instances.

        This method should be called periodically to ensure that all TimerHandler instances
        are updated with the latest elapsed time.
        """
        current_time_seconds: float = time()
        elapsed_time_seconds: float = current_time_seconds - self._time_seconds
        if (elapsed_time_seconds == 0):
            return
        self._time_seconds = current_time_seconds

        for handler in self._handlers:
            handler._update(elapsed_time_seconds)

    @property
    def time_seconds(self) -> float:
        """
        The current time in seconds.

        This property returns the current time in seconds since the epoch.
        """
        return self._time_seconds

    def create_handler(self) -> TimerHandler:
        """
        Creates a new TimerHandler instance, adds it to the list of handlers, and returns it.

        Returns:
        --------
        TimerHandler
            The newly created TimerHandler instance.
        """
        handler: TimerHandler = TimerHandler()
        self.add_handler(handler)
        return handler
    def add_handler(self, handler: TimerHandler) -> None:
        """
        Adds an existing TimerHandler instance to the list of handlers.

        Parameters:
        -----------
        handler : TimerHandler
            The TimerHandler instance to add.
        """
        self._handlers.append(handler)
    def remove_handler(self, handler: TimerHandler) -> None:
        """
        Removes an existing TimerHandler instance from the list of handlers.

        Parameters:
        -----------
        handler : TimerHandler
            The TimerHandler instance to remove.
        """
        self._handlers.remove(handler)
