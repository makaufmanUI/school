"""
util.py

Various utility functions.
"""
import time
from math import log as ln
from datetime import datetime

B  = 3_950   # K
T1 = 298.15  # K
R1 = 10_000  # Ohms

V_IN = 3.30  # V



def R2(v_out: float) -> float:
    """
    Calculates the resistance of the thermistor.

    ## Parameters
    - `v_out`:  The output voltage of the thermistor.
    """
    return R1 / (V_IN/v_out - 1)


def T2(analog_read: int) -> float:
    """
    Calculates the temperature of the thermistor.

    ## Parameters
    - `analog_read`:  The analog value read from the ADC.
    """
    v_out = 3.30 * analog_read/255.0
    return 1 / ( (1/T1) + (ln(R2(v_out)/R1) / B) )


def to_celsius(K: float) -> float:
    """
    Converts a temperature in Kelvin to Celsius.

    ## Parameters
    - `K`:  The temperature in Kelvin.
    """
    return K - 273.15


def to_fahrenheit(K: float) -> float:
    """
    Converts a temperature in Kelvin to Fahrenheit.

    ## Parameters
    - `K`:  The temperature in Kelvin.
    """
    return (K - 273.15) * 9/5 + 32


def get_datetime_str() -> str:
    """
    Returns the current date in `YYYY-MM-DD` format,
    and the current time in `HH:MM:SS` format, as a string.
    """
    dt = datetime.now()
    return f"{dt.strftime('%Y-%m-%d')} &nbsp; {dt.strftime('%H:%M:%S')}"


def get_avg(l: list[int|float], n: int = 10) -> float:
    """
    Returns the average of the last `n` elements in the given list.

    ## Parameters
    - `l`:  The list to average.
    - `n`:  The number of elements to average. Defaults to 10.
    """
    return sum(l[-n:]) / n

