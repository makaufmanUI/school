"""
led.py

Defines the `LED` class, which gives basic control over the LEDs.
"""
import RPi.GPIO as GPIO



class LED:
    def __init__(self, pin: int) -> None:
        self.pin = pin
        GPIO.setup(self.pin, GPIO.OUT)
        self.off()
    
    @property
    def is_on(self) -> bool:
        return GPIO.input(self.pin)
    
    def on(self) -> None:
        """
        Turns the LED on if not already on.
        """
        if not self.is_on:
            GPIO.output(self.pin, GPIO.HIGH)
    
    def off(self) -> None:
        """
        Turns the LED off if not already off.
        """
        if self.is_on:
            GPIO.output(self.pin, GPIO.LOW)

