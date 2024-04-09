ESP-Now is a wireless communication protocol developed by Espressif Systems, primarily designed for low-power and low-latency communication between ESP devices. It operates in the 2.4 GHz frequency range and utilizes a simple, peer-to-peer communication model, enabling devices to transmit data directly without the need for a traditional Wi-Fi network or access point. ESP-Now allows pairing with a maximum of 20 devices (unencrypted mode) or 6 devices (encrypted mode) with a range of ~150m between two communicating devices. Use of repeaters could extend both range and number of interconnected devices.

ESP-Now is well-suited for scenarios requiring quick and efficient communication, making it suitable for applications like sensor networks, home automation, and IoT devices where minimizing power consumption 
and latency are crucial. Ref. https://www.espressif.com/en/solutions/low-power-solutions/esp-Now

Project description

Messages from multiple sensors (ESP-based devices) are sent using ESP-Now to an ESP gateway (directly or via repeaters). Each ESP node could be either a "sensor", a "repeater" or a "gateway" (conditional compiling directives in /node/include/node_config.h). Data send/receive use a structure (node ID, payload and data payload type). Data payload types are defined in /node/include/datatypes.h. The structure definition and payload data types can be modified to send whichever variable types are suitable for a particular project.

The role of the gateway is to collect data received over ESP-Now and retransmit it to AWS IoT Core over WiFi, using the MQTT protocol.
A gateway consists of two ESP devices interconnected over a serial link:
- a gateway node (to collect ESP-Now messages and retransmit them over the serial link)
- a wifi transmitter (to receive messages over the serial link from the gateway node and retransmit them over WiFi to AWS IoT Core using WiFi)
The reason of using two ESP modules are wireless channel limitations in using both ESP-Now and WiFi protocols on the same ESP board. 
Ref. https://docs.espressif.com/projects/esp-faq/en/latest/application-solution/esp-Now.html

The code in /node works for any of the three types of nodes (sensor, repeter or gateway) on both ESP8266 or ESP32 boards - the configuration is done through conditional compiling directives in node_config.h.
The code has been tested on the following boards:

- ESP8266 ESP-01 (sensor only)
- ESP8266 ESP-12E (sensor, repeater, gateway)
- ESP32 DevKit v1 (sensor, repeater, gateway)

It has the following parts:
- Initialize hardware (microcontroller board and/or sensors);
- Initialize ESP-Now;
- Register callback functions for sending and/or receiving data;
- Add a peer device (MAC address) as the next hop (repeater of gateway);
- Loop to read/send/receive/retransmit data (depending on the node's role.

Files:
- /node/include/node_config.h - generic configuration for the node
- /node/include/node_espNow.h - ESP-Now specific
- /node/include/node.h - node specific
- /node/include/atatypes.h - payload data taypes
- /node/main.cpp - main program
- /node/platformio.ini - PlatformIO initialization

The code in /wifi_transmitter receives data over the serial link, and retransmit it as MQTT messages to AWS IoT Core. It has been tested on ESP32 DevKit v1, but it could be easily adjusted for other ESP-based boards.

Files:
- /wifi_transmitter/include/secrets.h - configuration (WiFi credentials, certificates)
- /wifi_transmitter/main.cpp - main program
- /wifi_transmitter/platformio.ini - PlatformIO initialization
