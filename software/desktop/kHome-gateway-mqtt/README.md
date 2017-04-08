This is a software which acts as a gateway between a khSerial-Device and MQTT.

Currently, only rudimentary functions are implemented.
This software is only able to route REG_B-Packets (Register Broadcasts, see Specification) to the MQTT interface.
The code isn't well documented yet.
Command-line options aren't available yet, the serial port and the MQTT Broker's address are hardcoded.

The Paho MQTT-Libs must be copied to a system folder, like /usr/lib, in order for proper functionality of the software.

The following libraries are included:
	- Paho MQTT C Client