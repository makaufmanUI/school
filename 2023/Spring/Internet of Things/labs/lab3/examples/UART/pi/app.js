/*
*
*   ./examples/UART/pi/app.js
*
*   Reworked code from the "1_UART-Example.zip" example.
*
*/


const { createBluetooth } = require('node-ble');
const ARDUINO_BLUETOOTH_ADDR = '64:ab:df:6a:07:2c';     // TODO: Replace this with Arduino's BT address


// Define Nordic UART service profile UUIDs
const UART_SERVICE_UUID      = '6E400001-B5A3-F393-E0A9-E50E24DCCA9E';
const TX_CHARACTERISTIC_UUID = '6E400002-B5A3-F393-E0A9-E50E24DCCA9E';
const RX_CHARACTERISTIC_UUID = '6E400003-B5A3-F393-E0A9-E50E24DCCA9E';




async function main()
{
    // Reference the BLE adapter and begin device discovery
    const { bluetooth, destroy } = createBluetooth();
    const adapter = await bluetooth.defaultAdapter();
    const discovery = await adapter.startDiscovery();
    console.log('>> Discovering...');
    
    // Attempt to connect to the device with specified BT address
    const device = await adapter.waitDevice(ARDUINO_BLUETOOTH_ADDR.toUpperCase());
    console.log('>> Found device. Attempting connection...');
    await device.connect();
    console.log('>> Connected to device!');
    
    // Get references to the desired UART service and its characteristics
    const gattServer = await device.gatt();
    const uartService = await gattServer.getPrimaryService(UART_SERVICE_UUID.toLowerCase());
    const txChar = await uartService.getCharacteristic(TX_CHARACTERISTIC_UUID.toLowerCase());
    const rxChar = await uartService.getCharacteristic(RX_CHARACTERISTIC_UUID.toLowerCase());
    
    // Register for notifications on the RX characteristic
    await rxChar.startNotifications();
    
    // Callback for when data is received on RX characteristic
    rxChar.on('valuechanged', buffer => {
        console.log('>> Received: ' + buffer.toString());
    });
    
    // Set up listener for console input
    // When console input is received, write it to TX characteristic
    const stdin = process.openStdin();
    stdin.addListener('data', async function(d)
    {
        let inStr = d.toString().trim();
        
        // Disconnect and exit if user types 'exit'
        if (inStr === 'exit')
        {
            console.log('>> Disconnecting...');
            await device.disconnect();
            console.log('>> Disconnected.');
            destroy();
            process.exit();
        }
        
        // Specification limits packets to 20 bytes; truncate string if too long
        inStr = (inStr.length > 20) ? inStr.slice(0,20) : inStr;
        
        // Attempt to write/send value to TX characteristic
        await txChar.writeValue( Buffer.from(inStr) ).then(() => {
            console.log('>> Sent: ' + inStr);
        });
    });
}





main().then((ret) => {
    if (ret) console.log( ret );
}).catch((err) => {
    if (err) console.error( err );
});

