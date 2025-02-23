"""
motor.py

Defines the `Motor` class for controlling the DC motor.
"""
from led import LED
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)



class Motor:
    def __init__(self, pos_pin: int, neg_pin: int, enable_pin: int, frequency: int):
        """
        Motor contructor.
        Takes in pin information & PWM frequency, and initializes it with RPi.GPIO.
        """
        self.state = "off"                      # Keep track of current motor state
        self.direction = "cw"                   # Keep track of current motor direction
        self.duty_cycle = 100                   # Keep track of duty cycle assigned to motor
        self.pos_pin = pos_pin                  # BCM pin# from motor+
        self.neg_pin = neg_pin                  # BCM pin# from motor-
        self.frequency = frequency              # Keep track of the frequency of the PWM pulses
        self.enable_pin = enable_pin            # BCM pin# from L293D ENABLE
        GPIO.setup(self.pos_pin,GPIO.OUT)       # Specify the motor's positive pin as an output
        GPIO.setup(self.neg_pin,GPIO.OUT)       # Specify the motor's negative pin as an output
        GPIO.setup(self.enable_pin,GPIO.OUT)    # Specify the L293D's ENABLE pin as an output
        self.pwm = GPIO.PWM(self.enable_pin,self.frequency) # Create a PWM instance for the motor
        
    def initialize_LEDs(self, red: LED, blue: LED, green: LED, orange: LED):
        """
        Initializes the motor's LEDs.
        """
        self.redLED = red
        self.blueLED = blue
        self.greenLED = green
        self.orangeLED = orange
    
    def update_LEDs(self):
        """
        Updates the LEDs to match the current motor state.
        This is called after the motor's state is changed.
        """
        if self.state == "on":
            self.redLED.off()
            self.greenLED.on()
            if self.direction == "cw":
                self.blueLED.on()
                self.orangeLED.off()
            else:
                self.blueLED.off()
                self.orangeLED.on()
        else:
            self.redLED.on()
            self.blueLED.off()
            self.greenLED.off()
            self.orangeLED.off()

    def set_frequency(self, frequency: int):
        """
        Sets the frequency of the PWM signal sent to the motor.
        """
        print(f">> Frequency set to {frequency}.")
        self.frequency = frequency
        self.pwm = GPIO.PWM(self.enable_pin, frequency)
        
    def set_duty_cycle(self, duty_cycle: int):
        """
        Sets the duty cycle of the PWM signal sent to the motor.
        """
        print(f">> Duty cycle set to {duty_cycle}.")
        self.duty_cycle = duty_cycle            # Update the motor's duty cycle attribute
        self.pwm.ChangeDutyCycle(duty_cycle)    # Change the duty cycle of the PWM signal
        
    def stop(self):
        """
        Turns the motor off.
        Turns the motor off by removing its PWM signal and setting the L293D's ENABLE pin to LOW.
        """
        print(">> Stopping motor.\n")
        self.pwm.stop()                         # Stop sending the PWM signal to the motor.
        GPIO.output(self.enable_pin, GPIO.LOW)  # Set the L293D's ENABLE pin to LOW.
        self.state = "off"                      # Update the motor's state attribute.
        self.update_LEDs()                      # Update the LEDs to match the motor's state.

    def start(self, direction: str, duty_cycle=100):
        """
        Turns the motor on.
        Turns the motor on (with 100% duty cycle if not specified), turning in the given direction.
        """
        print( ">> Starting motor.")
        print(f"   Direction:  {direction}")
        print(f"   Duty cycle: {duty_cycle}\n")
        self.direction = direction
        if duty_cycle != self.duty_cycle:       # Update the motor's duty_cycle attribute if different than current
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
        self.update_LEDs()                    # Update the LEDs to match the motor's state.