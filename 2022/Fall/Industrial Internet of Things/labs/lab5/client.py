"""
client.py

A simple echo client that sends messages to the server for interpretation.
"""
import os
import sys
import socket



class Client:
    def __init__(self, server_ip: str, server_port: int) -> None:
        self.server_ip = server_ip
        self.server_port = server_port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    def connect_to_server(self) -> None:
        self.socket.connect((self.server_ip, self.server_port))
        print(f"\n\n>> Connected to {self.server_ip}:{self.server_port}.\n\n")
    
    def send(self, message: str) -> None:
        self.socket.send(str.encode(message))
        print(f"\n      >>     Sent: \"{message}\"")
    
    def receive(self, buffer_size: int) -> None:
        message = self.socket.recv(buffer_size)
        if message:
            print(f"      >> Received: \"{message.decode('utf-8')}\"\n\n")

    def close_connection(self) -> None:
        self.socket.close()
        print(">> Connection closed.\n\n\n")



if __name__ == "__main__":
    os.system('cls' if os.name == 'nt' else 'clear')

    print("\n\nValid commands:")
    print(    "---------------\n")
    print(    "   >  \"q\":  quit the program")
    print(    "   -  \"c\":  close the connection")
    print(    "   -  \"h\":  return the stepper motor to home position")
    print(    "   -  \"sh\": set current position to new home position")
    print(    "   -  \"rh\": reset the home position to the default home position")
    print(    "   -  \"direction, angle, speed\":  move the stepper motor as specified\n\n")
    
    TCP_PORT = 5005
    BUFFER_SIZE = 1024
    TCP_IP = '192.168.1.13'
    
    client = Client(TCP_IP, TCP_PORT)
    client.connect_to_server()
    
    while True:
        user_input = input("   >> Input command:  ")
        
        if user_input == "q":
            break
        elif user_input == "c":
            client.send(user_input)
            client.receive(BUFFER_SIZE)
            client.close_connection()
            break
        else:
            client.send(user_input)
            client.receive(BUFFER_SIZE)
    
    sys.exit()

