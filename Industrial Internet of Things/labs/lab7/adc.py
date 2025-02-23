"""
adc.py

Defines the `ADC` class, a wrapper for the `ADCDevice` class.
Provides a simpler interface to the ADC with only the methods needed for this lab.
"""
import RPi.GPIO as GPIO
from ADCDevice import *



class ADC:
    def __init__(self, input_channel: int = 0) -> None:
        """
        Instantiates the ADC object.
        
        ## Parameters
        - `input_channel`:  The input channel to read analog values from.
        """
        self.adc = ADCDevice()
        self.adc = ADS7830()
        self.input_channel = input_channel
    
    def read(self) -> int:
        """
        Returns the analog value of the input channel.
        """
        return self.adc.analogRead(self.input_channel)
    
    def close(self) -> None:
        """
        Closes the connection to the ADC and performs a GPIO cleanup.
        """
        self.adc.close()
        GPIO.cleanup()
    
    def __del__(self):
        """
        Explicit destructor which calls `self.close()`.
        """
        self.close()

