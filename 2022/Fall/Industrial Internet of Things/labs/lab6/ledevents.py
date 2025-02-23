"""
ledevents.py

Defines the `LEDEvents` class, which uses threading and events to trigger LED on/off cycles (blinking).
"""
import threading
from led import LED
from util import sleep



class LEDEvents():
    def __init__(self, red_led: LED, blue_led: LED, green_led: LED, orange_led: LED) -> None:
        self.red_led = red_led
        self.blue_led = blue_led
        self.green_led = green_led
        self.orange_led = orange_led
        self.red_event = threading.Event()
        self.blue_event = threading.Event()
        self.green_event = threading.Event()
        self.orange_event = threading.Event()
        self.red_continuous_event = threading.Event()
        self.red_thread = None
        self.blue_thread = None
        self.green_thread = None
        self.orange_thread = None

    def start(self) -> None:
        self.red_thread = threading.Thread(target=self.red_blink, daemon=True)
        self.blue_thread = threading.Thread(target=self.blue_blink, daemon=True)
        self.green_thread = threading.Thread(target=self.green_blink, daemon=True)
        self.orange_thread = threading.Thread(target=self.orange_blink, daemon=True)
        self.red_continuous_event = threading.Thread(target=self.red_continuous, daemon=True)
        self.red_thread.start()
        self.blue_thread.start()
        self.green_thread.start()
        self.orange_thread.start()
        self.red_continuous_event.start()


    def red_blink(self) -> None:
        while True:
            self.red_event.wait()
            self.red_led.on()
            sleep(0.5)
            self.red_led.off()
            sleep(0.5)
    
    def red_continuous(self) -> None:
        while True:
            self.red_continuous_event.wait()
            self.red_led.on()

    def blue_blink(self) -> None:
        while True:
            self.blue_event.wait()
            self.blue_led.on()
            sleep(0.5)
            self.blue_led.off()
            sleep(0.5)

    def green_blink(self) -> None:
        while True:
            self.green_event.wait()
            self.green_led.on()
            sleep(0.5)
            self.green_led.off()
            sleep(0.5)

    def orange_blink(self) -> None:
        while True:
            self.orange_event.wait()
            self.orange_led.on()
            sleep(0.5)
            self.orange_led.off()
            sleep(0.5)

    def red_on(self) -> None:
        self.red_event.set()

    def red_off(self) -> None:
        self.red_event.clear()

    def blue_on(self) -> None:
        self.blue_event.set()

    def blue_off(self) -> None:
        self.blue_event.clear()

    def green_on(self) -> None:
        self.green_event.set()

    def green_off(self) -> None:
        self.green_event.clear()

    def orange_on(self) -> None:
        self.orange_event.set()

    def orange_off(self) -> None:
        self.orange_event.clear()
    
    def red_continuous_on(self) -> None:
        self.red_continuous_event.set()

    def red_continuous_off(self) -> None:
        self.red_continuous_event.clear()

