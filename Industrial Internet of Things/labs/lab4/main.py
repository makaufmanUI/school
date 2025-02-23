"""
Lab 4
DC motor control

How it works:
    Cycles between two different directions of rotation, clockwise and counterclockwise.
    Each direction is run at 100% duty cycle for 5 seconds, then ran at half speed for 10 seconds (50% duty cycle).
    The motor is then stopped for 5 seconds before repeating the cycle.
"""
import os
import sys
import time
from led import LED
from motor import Motor
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)



if __name__ == '__main__':
    # Initialize LEDs
    redLED = LED(24)        # signals motor OFF status   (red jumper wire)
    blueLED = LED(25)       # signals motor CW rotation  (blue jumper wire)
    greenLED = LED(23)      # signals motor ON status    (green jumper wire)
    orangeLED = LED(12)     # signals motor CCW rotation (orange jumper wire)
    
    # Initialize motor
    motor = Motor(pos_pin = 20, neg_pin = 16, enable_pin = 21, frequency = 100)
    motor.initialize_LEDs(redLED, blueLED, greenLED, orangeLED)
    
    # Clear terminal
    os.system('cls' if os.name == 'nt' else 'clear')
    
    # Main loop
    print("Program started.")
    print("Press CTRL+C to exit.\n\n")
    
    time.sleep(3)

    try:
        while True:
            motor.start("cw")
            time.sleep(5)
        
            motor.set_duty_cycle(50)
            time.sleep(10)
        
            motor.stop()
            time.sleep(5)
        
            motor.start("ccw")
            time.sleep(5)
        
            motor.set_duty_cycle(50)
            time.sleep(10)
        
            motor.stop()
            time.sleep(5)
    
    except KeyboardInterrupt:
        print("Program ended.")
        
        redLED.off()
        blueLED.off()
        greenLED.off()
        orangeLED.off()
        GPIO.cleanup()
        sys.exit()

