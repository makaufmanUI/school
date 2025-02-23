/*
*
*   ./examples/UART/arduino/arduino.ino
*
*   Reworked code from the "1_UART-Example.zip" example.
*
*
*   Added:
*       - 
*
*   Todo:
*       - 
*/


#include <ArduinoBLE.h>
#include "TimeoutTimer.h"
#define BUFSIZE 20


// Simulate UART connection using the ArduinoBLE library
BLEService uartService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
BLEStringCharacteristic txChar("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWrite, 20);
BLEStringCharacteristic rxChar("6E400003-B5A3-F393-E0A9-E50E24DCCA9E", BLERead | BLENotify, 20);



void setup()
{
    // Initialize Serial
    Serial.begin(9600);
    while (!Serial);
    
    // Initialize the BLE module
    if(!BLE.begin())
    {
        Serial.println(">> Starting BLE failed.");
        while (1);
    }
    
    // Get the Arduino's BT address
    String deviceAddress = BLE.address();
    
    // Set the device name to advertise with
    BLE.setLocalName("ArduinoBLE UART");
    
    // Get UART service ready
    BLE.setAdvertisedService(uartService);  // Set the service to be advertised
    uartService.addCharacteristic(txChar);  // Set the UART service's TX characteristic
    uartService.addCharacteristic(rxChar);  // Set the UART service's RX characteristic
    BLE.addService(uartService);            // Add the UART service
    
    // Start advertising the new service
    BLE.advertise();
    Serial.println(">> Bluetooth device (" + deviceAddress + ") active, waiting for connections...");
}



void loop()
{
    // Wait for a BLE central device to connect
    BLEDevice central = BLE.central();
    
    // If a central device is connected to the peripheral...
    if (central)
    {
        // Print the central device's BT address
        Serial.print(">> Connected to central device: ");
        Serial.println(central.address());
        
        // While the central device is connected...
        while (central.connected())
        {
            // Get input from the user (via Serial Monitor) and send it to the central device
            char inputs[BUFSIZE+1];
            if (getUserInput(inputs, BUFSIZE))
            {
                Serial.print(">> [Send] ");
                Serial.println(inputs);
                rxChar.writeValue(inputs);
            }
            
            // Receive data from the central device (if written is true) and print it to the Serial Monitor
            if (txChar.written())
            {
                Serial.print(">> [Recv] ");
                Serial.println(txChar.value());
            }
        }
        
        // Print the central device's BT address upon disconnect
        Serial.print(">> Disconnected from central device: ");
        Serial.println(central.address());
    }
}









/**************************************************************************/
/*!
    @brief  Checks for user input (via the Serial Monitor)
        From: https://github.com/adafruit/Adafruit_BluefruitLE_nRF51
*/
/**************************************************************************/
bool getUserInput(char buffer[], uint8_t maxSize)
{
    // Create a TimeoutTimer object with a 100 millisecond timeout
    TimeoutTimer timeout(100);
    
    // Clear the buffer
    memset(buffer, 0, maxSize);
    
    // Wait for Serial data or until the timeout expires
    while ( (!Serial.available()) && !timeout.expired() ) { delay(1); }
    
    // If the timeout expired, return false (no data received)
    if ( timeout.expired() ) return false;
    
    // Wait for a short delay to allow the Serial buffer to fill up
    delay(2);
    
    // Read data from the Serial buffer into 'buffer' until maxSize is reached or no more data is available
    uint8_t count = 0;
    do {
        count += Serial.readBytes(buffer+count, maxSize);
        delay(2);
    }   while ( (count < maxSize) && (Serial.available()) );
    
    // If data has been read into 'buffer', return true
    return true;
}



