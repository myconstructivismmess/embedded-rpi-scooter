# Python Imports
from typing import List, Callable, TypeVar

# Main
T = TypeVar('T')
def find_index(arr: List[T], fn: Callable[[T], bool]) -> int:
    for i, el in enumerate(arr):
        if fn(el):
            return i
    return -1

def find_last_index(arr: List[T], fn: Callable[[T], bool]) -> int:
    for i, el in reversed(list(enumerate(arr))):
        if fn(el):
            return i
    return -1

def find_last(arr: List[T], fn: Callable[[T], bool]) -> int:
    for el in reversed(arr):
        if fn(el):
            return el
    return None

def find(arr: List[T], fn: Callable[[T], bool]) -> int:
    for el in arr:
        if fn(el):
            return el
    return None