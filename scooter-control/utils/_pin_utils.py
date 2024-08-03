# Packages Imports
try:
    from microcontroller import Pin
    import board
except Exception as e:
    print("WARN: IMPORTS: The \"microcontroller\" and \"board\" module cannot be imported; this is normal in a development environment (outside of a board environment).")
    board = None

# Main
class GPIOPin:
    VALID_PIN_INDEXES = [4, 5, 6, 12, 13, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26]

    def __init__(self, pin: str | int) -> None:
        self._pin: int = pin if type(pin) is int else int(float(pin))

        if not self._pin in self.VALID_PIN_INDEXES:
            raise ValueError(f"ERROR: BOARD_PIN: Invalid pin number {self._pin}. Valid pins are {", ".join(self.VALID_PIN_INDEXES[0:-1])} and {self.VALID_PIN_INDEXES[len(self.VALID_PIN_INDEXES) - 1]}.")
    
    @property
    def microcontroller_module_loaded(self) -> bool:
        if board is None or Pin is None:
            return True
        return False

    def get_as_microcontroller_pin(self) -> Pin:
        if board is None:
            raise ImportError("The \"board\" module is not available. Ensure you are running in the correct environment.")

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
            case _:
                raise ValueError(f"ERROR: BOARD_PIN: Unsupported pin number {self._pin}.")
