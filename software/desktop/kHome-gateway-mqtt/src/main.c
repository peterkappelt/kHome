#include <stdio.h>
#include <unistd.h>

#include "serial.h"
#include "mqtt.h"

int main(int argc, char* argv[]){

	//open the serial port and the interface to the MQTT broker
	//the functions themselves care about error handling and may exit (with printing an error)
	serialInit();
	serialOpen();
	mqttOpen();

	while(1){
		//the serial loop functions reads from the serial port, buffers the data and handles it, if a full packet was received.
		serialLoop();
	}

    mqttClose();

	return 0;
}