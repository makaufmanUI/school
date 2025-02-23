"""
server.py
=========

A simple echo server that receives messages from a client,
interprets the commands, and relays them to the stepper motor.
"""
import os
import sys
import time
import socket
import datetime
import threading
from led import LED
from motor import Motor
from hcsr04 import HCSR04
from ledevents import LEDEvents
from googlesheet import GoogleSheet
from util import get_ip, parse



class Server:
    def __init__(self, ip: str, tcp_port: int) -> None:
        self.ip = ip
        self.port = tcp_port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.client_address = None
        self.socket.bind((self.ip, self.port))
        
    def wait_for_connection(self) -> None:
        self.socket.listen(1)
        print(f"\n\n>> Listening on {self.ip}:{self.port} ... ", end="")
    
    def accept_connection(self) -> None:
        self.connection, client_address = self.socket.accept()
        if self.client_address is None:
            print("connection with {}:{} opened.\n\n".format(*client_address))
        elif client_address[0] != self.client_address[0]:
            print("connection with {}:{} opened.\n\n".format(*client_address))
        self.client_address = client_address
    
    def receive_data(self, buffer_size: int) -> str:
        self.data = self.connection.recv(buffer_size)
        if not self.data:
            return None
        print(f"   >> Received: \"{self.data.decode('utf-8')}\"")
        return self.data.decode("utf-8")
    
    def send(self, message: str|list[str]) -> None:
        if isinstance(message, list):
            message = ",".join([str(item) for item in message])
        self.message = str.encode(message)
        self.connection.sendall(message.encode("utf-8"))
        print(f"   >>     Sent: \"{message}\"\n\n")
    
    def close_connection(self) -> None:
        self.send("Closing connection.")
        self.connection.close()
        print(">> Connection with {}:{} closed.\n\n\n".format(*self.client_address))



def system_control():
    global distance
    while True:
        distance = sensor.measure_average()
        l1, l2, l3, l4 = sensor.get_limits()
        if distance <= l1:
            motor.stop()
            led_events.red_off()
            led_events.blue_off()
            led_events.green_off()
            led_events.orange_off()
            led_events.red_continuous_on()
        if distance > l1 and distance <= l2:
            motor.start("cw", 25)
            led_events.red_on()
            led_events.blue_off()
            led_events.green_off()
            led_events.orange_off()
            led_events.red_continuous_off()
        if distance > l2 and distance <= l3:
            motor.start("cw", 50)
            led_events.red_off()
            led_events.blue_off()
            led_events.green_off()
            led_events.orange_on()
            led_events.red_continuous_off()
        if distance > l3 and distance <= l4:
            motor.start("cw", 75)
            led_events.red_off()
            led_events.blue_on()
            led_events.green_off()
            led_events.orange_off()
            led_events.red_continuous_off()
        if distance > l4:
            motor.start("cw", 100)
            led_events.red_off()
            led_events.blue_off()
            led_events.green_on()
            led_events.orange_off()
            led_events.red_continuous_off()
        else:
            led_events.red_off()
            led_events.blue_off()
            led_events.green_off()
            led_events.orange_off()
            led_events.red_continuous_off()
        time.sleep(1)



if __name__ == "__main__":
    global motor, sensor, red, blue, green, orange
    os.system('cls' if os.name == 'nt' else 'clear')
    
    # Initialize LEDs
    red = LED(22)
    blue = LED(5)
    green = LED(27)
    orange = LED(6)
    LEDs = [red, blue, green, orange]

    # Initialize motor
    motor = Motor(pos_pin = 20, neg_pin = 16, enable_pin = 21, frequency = 100)
    motor.stop()

    # Initialize ultrasonic sensor
    sensor = HCSR04(trigger_pin = 23, echo_pin = 17)

    # Initialize LED events and start thread
    led_events = LEDEvents(red, blue, green, orange)
    led_events.start()

    # Initialize control thread
    control = threading.Thread(target = system_control, daemon = True)

    # Initialize Google Sheet
    sheet = GoogleSheet("Lab6")

    # Get IP and initialize server
    IP = get_ip()
    server = Server(IP, 5005)
    server.wait_for_connection()
    
    # Loop
    while True:
        server.accept_connection()
        last_update = datetime.datetime.now()
        control.start()
        
        while True:
            data = server.receive_data(1024)
            if not data:
                break
            
            if data.lower() == "c":
                server.close_connection()
                motor.cleanup()
                sensor.cleanup()
                for led in LEDs:
                    led.cleanup()
                sys.exit()
            
            if ',' in data:
                limits = parse(data)        # parse the message/data into a list of proximity limits
                if limits:
                    sensor.set_limits(*limits)
                    server.send("Proximity limits set.")
                else:
                    server.send("TypeError in command.")
            
            elif data.lower() == "r":
                sensor.reset_limits()
                server.send("Proximity limits reset.")
            
            elif data.lower() == "u":
                # Send data back to the client, for upload to google sheets
                distance = str(sensor.get_last_distance())
                duty_cycle = str(motor.get_actual_duty_cycle())
                limits = ", ".join([str(l) for l in sensor.get_limits()])
                server.send([distance, duty_cycle, limits])
                sheet.push_data(distance, duty_cycle, limits)

            else:
                server.send("Invalid command.")

