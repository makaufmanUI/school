"""
stepper.py

Defines the `Stepper` class, which defines several methods for controlling the stepper motor.
"""
from led import LED
import RPi.GPIO as GPIO
from util import sleep, normalize, to_angle

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)



class Stepper:
    def __init__(self, enable_pin: int, coil_a1_pin: int, coil_a2_pin: int, coil_b1_pin: int, coil_b2_pin: int) -> None:
        # Set initial values
        self.delay = 0.00195                # delay between steps (this delay corresponds to maximum speed)
        self.steps_from_home = 0            # keeps track of steps away from home (+cw, -ccw); i.e. if pos., must rotate cw to go home
        self.steps_from_default_home = 0    # keeps track of steps away from default home, even if user sets a new home
        
        # Set speed limits
        self.min_speed = 10     # 10s / rotation, minimium speed
        self.max_speed =  4     #  4s / rotation, maximum speed
        
        # Initialize pins
        self.enable_pin  = enable_pin
        self.coil_a1_pin = coil_a1_pin
        self.coil_a2_pin = coil_a2_pin
        self.coil_b1_pin = coil_b1_pin
        self.coil_b2_pin = coil_b2_pin

        # Set pin modes
        GPIO.setup(self.enable_pin,  GPIO.OUT)
        GPIO.setup(self.coil_a1_pin, GPIO.OUT)
        GPIO.setup(self.coil_a2_pin, GPIO.OUT)
        GPIO.setup(self.coil_b1_pin, GPIO.OUT)
        GPIO.setup(self.coil_b2_pin, GPIO.OUT)

        # Set enable pin to high
        GPIO.output(self.enable_pin, 1)
        
    def set_LEDs(self, red: LED, blue: LED, green: LED, orange: LED) -> None:
        """
        Gives the stepper object access to the LEDs.
        Allows the motor to control their state when operating.
        """
        self.red_led = red
        self.blue_led = blue
        self.green_led = green
        self.orange_led = orange
        self.red_led.on()
    
    def set_delay(self, delay: float) -> None:
        """
        Sets the delay between steps that
        the stepper motor will use when rotating.
        Never used directly, but used by `set_speed()` and `set_speed_pct()`.
        """
        self.delay = delay
    
    def set_speed(self, sec_per_rev: float) -> None:
        """
        Sets the speed of the stepper motor in absolute terms.
        Never used directly, but used by `set_speed_pct()`.
        Using this method as an intermediate step allows for easier exception handling.
        """
        if sec_per_rev < self.max_speed:
            raise ValueError("The stepper cannot complete a revolution in under 4 seconds.")
        self.delay = sec_per_rev / 2050.0
    
    def set_speed_pct(self, pct: float) -> None:
        """
        Sets the speed of the stepper motor as a percentage of maximum speed.
        The given percentage can be expressed as a normal percentage or decimal percentage.
        """
        if pct <=  1: pct *= 100    # if given as decimal, convert to normal
        if pct > 100: pct = 100     # adjust any values above 100 to 100
        if pct <   0: pct = 0       # adjust any values below 0 to 0
        self.set_speed(self.min_speed - (pct/100)*(self.min_speed-self.max_speed))
    
    def set_step(self, a1: int, a2: int, b1: int, b2: int) -> None:
        """
        Sets the stepper motor to the given step.
        Never used directly, but used by `rotate()` and `deactivate()`.
        """
        GPIO.output(self.coil_a1_pin, a1)
        GPIO.output(self.coil_a2_pin, a2)
        GPIO.output(self.coil_b1_pin, b1)
        GPIO.output(self.coil_b2_pin, b2)
        
    def deactivate(self) -> None:
        """
        Deactivates the stepper motor and removes any holding torque.
        """
        self.set_step(0, 0, 0, 0)
        self.red_led.on()
        self.blue_led.off()
        self.green_led.off()
        self.orange_led.off()
    
    def num_steps_required(self, angle: float) -> int:
        """
        Returns the number of steps required to rotate the stepper motor to the given angle (lowball).
        """
        return int(round(1.4222*angle, 0))
    
    def rotate(self, direction: str, angle: float = None, steps: int = None) -> None:
        """
        Rotates the stepper motor in the specified direction.
        The direction can be specified as either 'cw' or 'ccw'.

        The angle or number of steps can be specified to dictate rotation,
        as homing requires a specific number of steps to return to home accurately.
        """
        # Ensure the specified direction is valid
        if direction.lower() not in ['cw', 'ccw']:
            print("   >>   [Stepper] Invalid direction: {}".format(direction))
            return
        
        # Set the number of steps to rotate, regardless of whether angle or steps was given
        num_steps = self.num_steps_required(angle) if angle else steps if steps else None
        
        if not num_steps:    # exit method if angle nor steps was given
            print("   >>   [Stepper] No angle or steps specified.")
            return
        if not angle:       # if steps was given, calculate the approximate angle
            angle = f"~{to_angle(num_steps)}"

        self.red_led.off()
        if direction.lower() == 'cw':
            self.blue_led.on()
            self.green_led.on()

            # Update steps_from_home
            self.steps_from_home -= num_steps
            self.steps_from_default_home -= num_steps
            print("      [Stepper] Rotating {} degrees CW ({} steps).\n\n".format(angle,num_steps))
            
            # Rotate clockwise
            for _ in range(num_steps):
                self.set_step(1, 0, 0, 1)
                sleep(self.delay)
                self.set_step(0, 1, 0, 1)
                sleep(self.delay)
                self.set_step(0, 1, 1, 0)
                sleep(self.delay)
                self.set_step(1, 0, 1, 0)
                sleep(self.delay)
        
        elif direction.lower() == 'ccw':
            self.green_led.on()
            self.orange_led.on()

            # Update steps_from_home
            self.steps_from_home += num_steps
            self.steps_from_default_home += num_steps
            print("      [Stepper] Rotating {} degrees CCW ({} steps).\n\n".format(angle,num_steps))
            
            # Rotate counter-clockwise
            for _ in range(num_steps):
                self.set_step(1, 0, 1, 0)
                sleep(self.delay)
                self.set_step(0, 1, 1, 0)
                sleep(self.delay)
                self.set_step(0, 1, 0, 1)
                sleep(self.delay)
                self.set_step(1, 0, 0, 1)
                sleep(self.delay)
        
        self.deactivate()   # deactivate the stepper motor when done rotating

    def go_home(self, default: bool = True) -> None:
        """
        Rotates the stepper motor to the home position.
        The default home position (where it was on program start) is assumed to be the target home position.
        If default is set to `False`, the stepper will go to the user-defined home position instead.
        """
        if default: steps_from_home = self.steps_from_default_home
        else:       steps_from_home = self.steps_from_home
        
        # Print the steps required to go home, and in what direction
        if steps_from_home > 0:
            print("      [Stepper] Going Home: {} steps CW from here (~{} degrees).".format(steps_from_home, to_angle(steps_from_home)))
        else:
            print("      [Stepper] Going Home: {} steps CCW from here (~{} degrees).".format(abs(steps_from_home), to_angle(steps_from_home)))
        
        # Normalize the steps required to go home
        steps = normalize(steps_from_home, -256, 256)
        
        # If already at home, exit
        if steps == 0:
            print("      [Stepper] Already home.")
            return
        
        if steps < 0:
            self.rotate('ccw', steps = abs(steps))  # rotate CCW if steps_from_home is negative
        else:
            self.rotate('cw', steps = steps)        # rotate CW if steps_from_home is positive
        
        if default:
            self.steps_from_default_home = 0
        else:
            self.steps_from_home = 0
        
        self.steps_from_home = 0
    
    def set_home(self) -> None:
        """
        Sets the current position as the home position.
        """
        self.steps_from_home = 0
        print("      [Stepper] New home set.\n\n")
    
    def reset_home(self) -> None:
        """
        Resets the home position to the default home position.
        """
        self.steps_from_home = self.steps_from_default_home
        print("      [Stepper] Home reset.\n\n")

