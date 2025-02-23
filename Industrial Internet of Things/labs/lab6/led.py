"""
led.py

Defines the `LED` class, which gives basic control over the LEDs.
"""
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)



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
        Turns the LED on.
        """
        GPIO.output(self.pin,GPIO.HIGH)
    
    def off(self) -> None:
        """
        Turns the LED off.
        """
        GPIO.output(self.pin,GPIO.LOW)
    
    def cleanup(self) -> None:
        """
        Cleans up the GPIO pins.
        """
        GPIO.cleanup()

