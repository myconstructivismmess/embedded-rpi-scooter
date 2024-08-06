# Main
class Boolean:
    def __init__(self, value: object):
        if isinstance(value, str):
            if value.lower() in ("true", "1"):
                self._value: bool = True
            elif value.lower() in ("false", "0"):
                self._value: bool = False
            else:
                raise ValueError(f"Invalid string value: {value}")
        elif isinstance(value, int):
            self._value: bool = value != 0
        elif isinstance(value, None):
            raise ValueError("None is not a valid boolean value")
        else:
            self._value: bool = bool(value)

    def __bool__(self) -> bool:
        return self._value
    
    @property
    def value(self) -> bool:
        return self._value
