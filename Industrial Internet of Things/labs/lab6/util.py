"""
util.py

Various utility functions.
"""
import datetime
from time import perf_counter
from subprocess import check_output
from socket import socket, AF_INET, SOCK_DGRAM



def sleep(duration: float) -> None:
    """
    More accurate sleep function than `time.sleep()`.
    """
    start = perf_counter()
    while perf_counter() - start < duration:
        pass


def usleep(duration: float) -> None:
    """
    Microsecond version of `sleep()`.
    """
    duration /= 1000000.0
    start = perf_counter()
    while perf_counter() - start < duration:
        pass


def get_mac_address() -> str:
    """
    Gets the MAC address of a Bluetooth adapter.
    """
    output = check_output(["hciconfig"]).decode("utf-8")
    address_line = output.splitlines()[1]
    mac_address = address_line[13:30]
    return mac_address


def convert(value: float, unit: str) -> float:
    """
    Converts the given value into the given units.
    It is assumed that the value being passed in has units of cm.
    """
    if unit == "cm":
        return value
    elif unit == "m":
        return value/100.0
    elif unit == "mm":
        return value*10.0
    elif unit == "in":
        return value/2.54
    elif unit == "ft":
        return value/30.48
    else:
        raise ValueError("Invalid unit: {}".format(unit))


def get_ip() -> str:
    """
    Returns the IP address of the thing calling this function.
    """
    s = socket(AF_INET,SOCK_DGRAM)
    s.connect(("8.8.8.8",80))
    ip_address = s.getsockname()[0]
    s.close()
    return ip_address


def parse(data: str, types: list = [float, float, float, float], delimiter: str = ',') -> list:
    """
    Parses the given data into a list with the given types.
    Both `types` and `delimiter` have the default values needed for proximity limits.

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


def seconds_since(dtime: datetime.datetime) -> int:
    """
    Returns the number of seconds that have elapsed
    between now and the time of the given `datetime` object.

    Parameters
    ----------
    `dtime` :   The `datetime` object to measure elapsed time since.
    """
    return int((datetime.datetime.now() - dtime).total_seconds())


def ask_for_input() -> None:
    """
    Asks the user for input, if hasn't already.
    """
    return input(">>> ")

