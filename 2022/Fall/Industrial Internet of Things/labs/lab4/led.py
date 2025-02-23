"""
led.py

Defines the `LED` class for controlling the state of an LED.
"""
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)



class LED:
    def __init__(self, pin: int) -> None:
        self.pin = pin
        GPIO.setup(self.pin,GPIO.OUT)   # Initialize the LED's pin as an output
        self.off()                      # Make sure the LED is off when starting
    
    @property
    def is_on(self) -> bool:
        """
        Returns `True` if the LED is on, `False` if it is off.
        """
        return GPIO.input(self.pin)
        
    def on(self) -> None:
        """
        Turns the LED on, if not already.
        """
        if not self.is_on: GPIO.output(self.pin,GPIO.HIGH)
    
    def off(self) -> None:
        """
        Turns the LED off, if not already.
        """
        if self.is_on: GPIO.output(self.pin,GPIO.LOW)
    
    def toggle(self) -> None:
        """
        Toggles the LED on or off, inverting its current state.
        """
        self.off() if self.is_on else self.on()

