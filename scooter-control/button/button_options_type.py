# Python Imports
from enum import Enum

# Main
class ButtonOptionsType(Enum):
    """
    FORCE_INIT_DOWN and FORCE_INIT_UP are unaffected by the reverse_signal option on interceptors; they replace the values obtained from interceptors at startup.
    """
    FORCE_INIT_DOWN = 1
    FORCE_INIT_UP = 2
