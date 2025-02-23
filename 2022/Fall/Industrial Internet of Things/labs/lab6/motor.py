"""
motor.py

Defines the `Motor` class for controlling the DC motor.
"""
import math
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)


def duty_cycle(dc) -> int:
    """
    Adjusts the duty cycle such that it falls in the range [45, 90].
    """
    if dc < 1 and dc != 0:
        dc *= 100
    if dc <= 0:
        dc = 45
    elif dc >= 90:
        dc = 90
    else:
        dc = 45 + (dc/100)*45.5
    return dc


def actual_duty_cycle(dc) -> int:
    """
    Returns the actual duty cycle.
    """
    return int(math.floor((20/9)*dc - 100))



class Motor:
    def __init__(self, pos_pin: int, neg_pin: int, enable_pin: int, frequency: int) -> None:
        """
        Motor contructor.
        Takes in pin information & PWM frequency, and initializes it with RPi.GPIO.
        """
        self.state = "off"                      # Keep track of current motor state
        self.direction = "cw"                   # Keep track of current motor direction
        self.duty_cycle = 90                     # Keep track of duty cycle assigned to motor
        self.pos_pin = pos_pin                  # BCM pin# from motor+
        self.neg_pin = neg_pin                  # BCM pin# from motor-
        self.frequency = frequency              # Keep track of the frequency of the PWM pulses
        self.enable_pin = enable_pin            # BCM pin# from L293D ENABLE
        GPIO.setup(self.pos_pin, GPIO.OUT)      # Specify the motor's positive pin as an output
        GPIO.setup(self.neg_pin, GPIO.OUT)      # Specify the motor's negative pin as an output
        GPIO.setup(self.enable_pin, GPIO.OUT)   # Specify the L293D's ENABLE pin as an output
        self.pwm = GPIO.PWM(self.enable_pin, self.frequency)  # Create a PWM instance for the motor

    def set_frequency(self, frequency: int) -> None:
        """
        Sets the frequency of the PWM signal sent to the motor.
        """
        self.frequency = frequency
        self.pwm = GPIO.PWM(self.enable_pin, frequency)
        
    def set_duty_cycle(self, duty_cycle: int) -> None:
        """
        Sets the duty cycle of the PWM signal sent to the motor.
        """
        self.duty_cycle = duty_cycle(duty_cycle)      # Update the motor's duty cycle attribute
        self.pwm.ChangeDutyCycle(self.duty_cycle)     # Change the duty cycle of the PWM signal
    
    def get_duty_cycle(self) -> int:
        """
        Returns the current duty cycle of the motor.
        """
        return self.duty_cycle
    
    def get_actual_duty_cycle(self) -> int:
        """
        Returns the actual duty cycle of the motor.
        """
        if self.state == "off":
            return 0
        return actual_duty_cycle(self.duty_cycle)
        
    def stop(self) -> None:
        """
        Turns the motor off.
        Turns the motor off by removing its PWM signal and setting the L293D's ENABLE pin to LOW.
        """
        self.pwm.stop()                         # Stop sending the PWM signal to the motor.
        GPIO.output(self.enable_pin, GPIO.LOW)  # Set the L293D's ENABLE pin to LOW.
        self.state = "off"                      # Update the motor's state attribute.

    def start(self, direction: str, duty_cycle: int = 90) -> None:
        """
        Turns the motor on.
        Turns the motor on (with 100% duty cycle if not specified), turning in the given direction.
        """
        self.direction = direction
        self.set_duty_cycle(duty_cycle)
        self.pwm.start(self.duty_cycle)         # Start sending the PWM signal to the motor.
        if self.direction == "cw":
            GPIO.output(self.pos_pin, GPIO.HIGH)
            GPIO.output(self.neg_pin, GPIO.LOW)
            GPIO.output(self.enable_pin, GPIO.HIGH)
        else:
            GPIO.output(self.pos_pin, GPIO.LOW)
            GPIO.output(self.neg_pin, GPIO.HIGH)
            GPIO.output(self.enable_pin, GPIO.HIGH)
        self.state = "on"
    
    def cleanup(self):
        """
        Cleans up the GPIO pins.
        """
        GPIO.cleanup()

