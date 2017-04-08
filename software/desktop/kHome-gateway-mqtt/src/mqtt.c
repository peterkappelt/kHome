/**
 * @file mqtt.h
 * @author Peter Kappelt
 * Handling of the MQTT Interface side of the gateway
 */

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "mqtt.h"
#include "log.h"

#include "paho-1.1.0/inc/MQTTClient.h"

#define MQTT_SERVER_ADDRESS "tcp://hcpi01.int.kappelt.net:1883"
#define TIMEOUT 1000L

MQTTClient client;

/**
 * Open a connection to the MQTT Broker
 * If there is an error, print it and exit
 */
void mqttOpen(void){
	int rc;

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_create(&client, MQTT_SERVER_ADDRESS, "kHomeGateway",
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        logWrite(1023, "Failed to connect to MQTT Server, return code %d", rc);
        exit(EXIT_FAILURE);
    }
}

/**
 * Close the connection to the MQTT Broker
 */
void mqttClose(void){
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
}

/**
 * This function will publish a device's data register to the topic 'kHome/device_x/data_y/value', where
 * x is the device's address in decimal and
 * y is the address of the data register in decimal
 */
void mqttPublishDataRegister(uint8_t deviceAddress, uint8_t registerAddress, uint32_t value){
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;

	char topicString[32];
	sprintf(topicString, "kHome/device_%d/data_%d/value", deviceAddress, registerAddress);

	char payloadData[16];
	sprintf(payloadData, "%d", value);

	pubmsg.payload = payloadData;
    pubmsg.payloadlen = strlen(payloadData);
    pubmsg.qos = 1;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, topicString, &pubmsg, &token);
    logWrite(1, "MQTT: Waiting for publication of %s on topic %s", payloadData, topicString);
    /*int rc = */MQTTClient_waitForCompletion(client, token, TIMEOUT);
    logWrite(1, "MQTT: Message with delivery token %d delivered", token);
}