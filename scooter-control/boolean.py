class Boolean(bool):
    def __init__(self, value: object):
        if isinstance(value, str):
            if value.lower() in ("true", "1"):
                super().__init__(True)
            elif value.lower() in ("false", "0"):
                super().__init__(False)
            else:
                raise ValueError(f"Invalid string value: {value}")
        elif isinstance(value, int):
            super().__init__(value != 0)
        elif isinstance(value, None):
            raise ValueError("None is not a valid boolean value")
        else:
            super().__init__(value)
