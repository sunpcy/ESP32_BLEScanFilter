# ESP32_BLEScanFilter
**Project Description:**

This repository contains code for a **BLE (Bluetooth Low Energy)** communication system between a client and server using ESP32 devices. The server code initializes a BLE service and characteristic, allowing data exchange with connected clients. The project focuses on leveraging BLE technology for efficient data transmission and interaction.

**Key Features:**

- **BLE Communication:** Utilizes BLEDevice, BLEServer, and BLECharacteristic classes from the ESP32 BLE library to establish a server capable of handling data exchanges.
- **UUID Configuration:** Defines unique UUIDs (_Service UUID_ and _Characteristic UUID_) for identifying and communicating between BLE devices.
- **Data Handling:** Implements basic data handling functionalities such as reading and writing characteristics.
- **Advertising:** Sets up BLE advertising to make the server discoverable to nearby BLE devices.

**Technologies and Concepts:**

- **RSSI (Received Signal Strength Indication):** Utilizes RSSI for proximity estimation between BLE devices.
- **Moving Average:** Implements moving average techniques to smooth RSSI readings, improving accuracy in distance estimation.
