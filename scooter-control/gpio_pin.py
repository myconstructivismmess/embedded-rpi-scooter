# Python Imports
import logging

# Packages Imports
try:
    from microcontroller import Pin
    import board
    MICROCONTROLLER_MODULE_LOADED = True
except ImportError:
    logging.warning("imports: The 'microcontroller' and 'board' modules cannot be imported; this is normal in a development environment (outside of a board environment).")
    MICROCONTROLLER_MODULE_LOADED = False
    class Pin():
        pass

# Main
class GPIOPin:
    VALID_PIN_INDEXES = [4, 5, 6, 12, 13, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26]

    def __init__(self, pin: str | int) -> None:
        self._pin: int = int(float(pin)) if isinstance(pin, str) else pin

        if self._pin not in self.VALID_PIN_INDEXES:
            valid_pins_str = [str(pin) for pin in self.VALID_PIN_INDEXES]
            raise ValueError(f"GPIOPin: Invalid pin number {self._pin}. Valid pins are {', '.join(valid_pins_str[0:-1])} and {valid_pins_str[len(valid_pins_str)]}.")
    
    @property
    def microcontroller_module_loaded(self) -> bool:
        return MICROCONTROLLER_MODULE_LOADED

    def get_as_microcontroller_pin(self) -> Pin:
        if not MICROCONTROLLER_MODULE_LOADED:
            raise ImportError("GPIOPin: The 'board' module is not available. Ensure you are running in the correct environment.")

        match self._pin:
            case 4:
                return board.D4
            case 5:
                return board.D5
            case 6:
                return board.D6
            case 12:
                return board.D12
            case 13:
                return board.D13
            case 16:
                return board.D16
            case 17:
                return board.D17
            case 18:
                return board.D18
            case 19:
                return board.D19
            case 20:
                return board.D20
            case 21:
                return board.D21
            case 22:
                return board.D22
            case 23:
                return board.D23
            case 24:
                return board.D24
            case 25:
                return board.D25
            case 26:
                return board.D26
            
    def __eq__(self, other) -> bool:
        if not isinstance(other, GPIOPin):
            return False
        
        return self._pin == other._pin

    def __ne__(self, other) -> bool:
        if not isinstance(other, GPIOPin):
            return True
        
        return self._pin != other._pin
