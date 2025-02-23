"""
util.py

Various utility functions.
"""
from math import floor
from time import perf_counter
from socket import socket, AF_INET, SOCK_DGRAM



def sleep(duration: float) -> None:
    """
    More accurate sleep function than `time.sleep()`.
    """
    start = perf_counter()
    while perf_counter() - start < duration:
        pass


def normalize(value: int, range_start: int, range_end: int) -> int:
    """
    Normalizes the given value to the specified range.
    Used in minimizing the steps required for the stepper to return home.
    
    For example, if the stepper needs to move `380` degrees CW to return home,
    on the range `[-180, 180]` degrees, this will return `20` degrees, meaning `20` degrees CW.
    """
    shift = value - range_start
    range_width = range_end - range_start
    return ( shift - (floor(shift/range_width) * range_width) ) + range_start


def get_ip() -> str:
    """
    Returns the IP address of the thing calling this function.
    """
    s = socket(AF_INET, SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    ip = s.getsockname()[0]
    s.close()
    return ip


def parse(data: str, types: list = [str, float, float], delimiter: str = ',') -> list:
    """
    Parses the given data into a list with the given types.
    Both `types` and `delimiter` have the default values needed for stepper commands.

    `None` will be returned if there is an issue casting an element to a given type,
     which is used by the server to detect a problem without needing to halt the program.
    """
    data = data.split(delimiter)
    for i in range(len(data)):
        try:
            data[i] = types[i](data[i])
        except:
            return None
    return data


def to_angle(steps: int) -> float:
    """
    Converts the given number of steps to a positive angle.
    """
    return round(abs(steps/1.42222), 1)

