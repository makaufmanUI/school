"""
hcsr04.py

Defines the `HCSR04` class for easy interfacing with the ultrasonic sensor.
"""
import time
import RPi.GPIO as GPIO
from util import usleep, convert

# Set the GPIO pin description to BCM and turning off warnings
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

SPEED_OF_SOUND = 34300  # cm/s



class HCSR04:
    def __init__(self, trigger_pin: int, echo_pin: int) -> None:
        # Set default values
        self.l1 =  5
        self.l2 = 10
        self.l3 = 15
        self.l4 = 20
        self.unit = "cm"
        self.last_distance = 0

        # Assign pins
        self.echo_pin = echo_pin
        self.trigger_pin = trigger_pin

        # Set GPIO pin modes
        GPIO.setup(self.echo_pin, GPIO.IN)
        GPIO.setup(self.trigger_pin, GPIO.OUT)

        # Set trigger pin low and give time to settle
        GPIO.output(self.trigger_pin, GPIO.LOW)
        time.sleep(1)
    
    def set_limits(self, l1: float, l2: float, l3: float, l4: float) -> None:
        """
        Sets the proximity limits that will determine motor state.
        """
        self.l1 = l1
        self.l2 = l2
        self.l3 = l3
        self.l4 = l4
    
    def get_limits(self) -> tuple[float, float, float, float]:
        return (self.l1, self.l2, self.l3, self.l4)
    
    def reset_limits(self) -> None:
        """
        Resets the proximity limits to their default values.
        """
        self.l1 =  5.0
        self.l2 = 10.0
        self.l3 = 15.0
        self.l4 = 20.0
    
    def set_unit(self, unit: str) -> None:
        self.unit = unit

    def trigger_pulse(self) -> None:
        # Sends a 10us pulse to trigger the sensor
        GPIO.output(self.trigger_pin, GPIO.HIGH)
        usleep(10)
        GPIO.output(self.trigger_pin, GPIO.LOW)
    
    def time_echo(self) -> float:
        # Measures the length of the echo pulse
        while GPIO.input(self.echo_pin) == 0:
            pulse_start = time.time()
        while GPIO.input(self.echo_pin) == 1:
            pulse_end = time.time()
        return pulse_end - pulse_start

    def measure_distance(self, unit: str = "cm") -> float:
        self.trigger_pulse()
        pulse_duration = self.time_echo()
        distance = (pulse_duration*SPEED_OF_SOUND) / 2.0
        self.last_distance = round(convert(distance,unit), 2)
        return self.last_distance

    def measure_average(self, n: int = 5, unit: str = "cm") -> float:
        # ~65ms for 5 measurement average
        measurements = []
        for i in range(n):
            measurements.append(self.measure_distance())
            time.sleep(0.01)
        average = sum(measurements) / len(measurements)
        self.last_distance = round(convert(average,unit), 2)
        return self.last_distance

    def get_last_distance(self) -> float:
        return self.last_distance

    def cleanup(self):
        GPIO.cleanup()

