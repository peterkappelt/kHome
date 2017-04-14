/**
 * @file mqtt.h
 * @author Peter Kappelt
 * Handling of the MQTT Interface side of the gateway
 */

//for asprintf()
#define _GNU_SOURCE

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "mqtt.h"
#include "log.h"
#include "serial.h"
#include "kBuffer/kBuffer.h"

#include "paho-1.1.0/inc/MQTTClient.h"

#define MQTT_SERVER_ADDRESS "tcp://hcpi01.int.kappelt.net:1883"
#define TIMEOUT 1000L

#define QOS 1

MQTTClient client;

void mqttMessageDelivered(void *context, MQTTClient_deliveryToken dt){

}

int mqttMessageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message){
    logWrite(1, "MQTT: Received something in topic %s", topicName);

    char* topicNameCopy = strdup(topicName);
    char* subTopics[32];            //there will be at most 32 subtopics
    int i = 0;
    //split the topic string by the "/" character
    char* currentSubTopic = strtok(topicNameCopy, "/");
    while((currentSubTopic != 0) && (i < 32)){
        subTopics[i] = strdup(currentSubTopic);
        currentSubTopic = strtok(NULL, "/");
        i++;
    }

    //copy the message's payload into string
    char* messagePayloadString = malloc(message->payloadlen + 1);
    char* payloadPtr = message->payload;
    for(i = 0; i < message->payloadlen; i++){
        messagePayloadString[i] = *(payloadPtr + i);
    }
    messagePayloadString[message->payloadlen] = 0;

    //subTopics[0] is always "kHome" -> we've only subscribed to such topics
    //subTopics[1] is always "device" -> we've only subscribed to such topics
    //subTopics[2] contains the device address, parse it:
    int deviceAddress;
    if(sscanf(subTopics[2], "%d", &deviceAddress) < 1){
        logWrite(2, "Can't parse device address in string \"%s\"", subTopics[2]);
        goto fcEnd;
    }

    //subTopics[3] is currently either "data", "config" or "status" (according to the type of the register)
    if(!strcmp(subTopics[3], "data")){
        khTelegram commandTelegram;
        commandTelegram.senderAddress = 0xFF;           //we are a gateway, so our address is 0xFF
        commandTelegram.receiverAddress = deviceAddress;

        //subTopics[4] contains the data register's address
        int registerAddress;
        if(sscanf(subTopics[4], "%d", &registerAddress) < 1){
            logWrite(2, "Can't parse register address in string \"%s\"", subTopics[4]);
            goto fcEnd;
        }

        //subTopics[5] contains the command, either "read" for initiating a read command or "write" for writing data
        if(!strcmp(subTopics[5], "read")){
            //we don't care about the message payload here
            logWrite(2, "MQTT: Read command for device %d, data register %d", deviceAddress, registerAddress);

            commandTelegram.telegramType = khTelType_REG_R;
            commandTelegram.payloadLength = 1;              //the payload contains just the register's address
            commandTelegram.payloadData = malloc(sizeof(uint8_t) * commandTelegram.payloadLength);
            commandTelegram.payloadData[0] = registerAddress & 0xFF;

            if(bufferWrite(&bufferMQTTToSerial, commandTelegram) != bufferOK){
                logWrite(1023, "Error while writing routed REG_R telegram to the buffer");
            }
        }else if(!strcmp(subTopics[5], "write")){
            int desiredValue;
            if(sscanf(messagePayloadString, "%d", &desiredValue) < 1){
                logWrite(2, "Can't parse payload data as intenger in string \"%s\"", messagePayloadString);
                goto fcEnd;
            }

            logWrite(2, "MQTT: Write command for device %d, data-register %d with data %d", deviceAddress, registerAddress, desiredValue);

            //todo: for simplicity we always write 4 bytes of data, even if we could save some bytes
            commandTelegram.telegramType = khTelType_REG_W;
            commandTelegram.payloadLength = 5;
            commandTelegram.payloadData = malloc(sizeof(uint8_t) * commandTelegram.payloadLength);

            commandTelegram.payloadData[0] = registerAddress & 0xFF;                //first byte of payload is the register's address
            commandTelegram.payloadData[1] = (desiredValue & 0xFF000000) >> 24; //the data, with MSB first, follows
            commandTelegram.payloadData[2] = (desiredValue & 0x00FF0000) >> 16;
            commandTelegram.payloadData[3] = (desiredValue & 0x0000FF00) >> 8;
            commandTelegram.payloadData[4] = (desiredValue & 0x000000FF) >> 0;

            if(bufferWrite(&bufferMQTTToSerial, commandTelegram) != bufferOK){
                logWrite(1023, "Error while writing routed REG_W telegram to the buffer");
            }
        }
    }else if(!strcmp(subTopics[3], "config")){
        khTelegram commandTelegram;
        commandTelegram.senderAddress = 0xFF;           //we are a gateway, so our address is 0xFF
        commandTelegram.receiverAddress = deviceAddress;

        //subTopics[4] contains the config register's address
        int registerAddress;
        if(sscanf(subTopics[4], "%d", &registerAddress) < 1){
            logWrite(2, "Can't parse register address in string \"%s\"", subTopics[4]);
            goto fcEnd;
        }

        //subTopics[5] contains the command, either "read" for initiating a read command or "write" for writing data
        if(!strcmp(subTopics[5], "read")){
            //we don't care about the message payload here
            logWrite(2, "MQTT: Read command for device %d, config register %d", deviceAddress, registerAddress);

            commandTelegram.telegramType = khTelType_CNF_R;
            commandTelegram.payloadLength = 1;              //the payload contains just the register's address
            commandTelegram.payloadData = malloc(sizeof(uint8_t) * commandTelegram.payloadLength);
            commandTelegram.payloadData[0] = registerAddress & 0xFF;

            if(bufferWrite(&bufferMQTTToSerial, commandTelegram) != bufferOK){
                logWrite(1023, "Error while writing routed CNF_R telegram to the buffer");
            }
        }else if(!strcmp(subTopics[5], "write")){
            int desiredValue;
            if(sscanf(messagePayloadString, "%d", &desiredValue) < 1){
                logWrite(2, "Can't parse payload data as intenger in string \"%s\"", messagePayloadString);
                goto fcEnd;
            }

            logWrite(2, "MQTT: Write command for device %d, config-register %d with data %d", deviceAddress, registerAddress, desiredValue);

            commandTelegram.telegramType = khTelType_CNF_W;
            commandTelegram.payloadLength = 2;
            commandTelegram.payloadData = malloc(sizeof(uint8_t) * commandTelegram.payloadLength);

            commandTelegram.payloadData[0] = registerAddress & 0xFF;                //first byte of payload is the register's address
            commandTelegram.payloadData[1] = desiredValue & 0xFF;

            if(bufferWrite(&bufferMQTTToSerial, commandTelegram) != bufferOK){
                logWrite(1023, "Error while writing routed CNF_W telegram to the buffer");
            }
        }
    }else if(!strcmp(subTopics[3], "status")){
        khTelegram commandTelegram;
        commandTelegram.senderAddress = 0xFF;           //we are a gateway, so our address is 0xFF
        commandTelegram.receiverAddress = deviceAddress;

        //subTopics[4] contains the status register's address
        int registerAddress;
        if(sscanf(subTopics[4], "%d", &registerAddress) < 1){
            logWrite(2, "Can't parse register address in string \"%s\"", subTopics[4]);
            goto fcEnd;
        }

        //subTopics[5] contains the command, is always "read" for status registers
        if(!strcmp(subTopics[5], "read")){
            //we don't care about the message payload here
            logWrite(2, "MQTT: Read command for device %d, status register %d", deviceAddress, registerAddress);

            commandTelegram.telegramType = khTelType_STS_R;
            commandTelegram.payloadLength = 1;              //the payload contains just the register's address
            commandTelegram.payloadData = malloc(sizeof(uint8_t) * commandTelegram.payloadLength);
            commandTelegram.payloadData[0] = registerAddress & 0xFF;

            if(bufferWrite(&bufferMQTTToSerial, commandTelegram) != bufferOK){
                logWrite(1023, "Error while writing routed STS_R telegram to the buffer");
            }
        }
    }else{
        logWrite(2, "MQTT: Don't know how to handle received topic %s", topicName);
    }

    //end of the function label, is used instead of return
    //we need to free memory
    fcEnd:

    if(messagePayloadString != NULL){
        free(messagePayloadString);
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void mqttConnectionLostCallback(void *context, char *cause){
    //todo: log with high priority until we are unable to re-connect automatically
    logWrite(1023, "MQTT: Connection to Broker lost: %s", cause);
}

/**
 * Open a connection to the MQTT Broker
 * If there is an error, print it and exit
 */
void mqttOpen(void){
	int rc;

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_create(&client, MQTT_SERVER_ADDRESS, "kHomeGateway", MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, mqttConnectionLostCallback, mqttMessageArrived, mqttMessageDelivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        logWrite(1023, "Failed to connect to MQTT Server, return code %d", rc);
        exit(EXIT_FAILURE);
    }

    //topic kHome/device/x/data/y/read -> issue a read command to the specific device
    MQTTClient_subscribe(client, "kHome/device/+/data/+/read", QOS);

    //topic kHome/device/x/data/y/read -> issue a write command to the specific device
    MQTTClient_subscribe(client, "kHome/device/+/data/+/write", QOS);

    //topic kHome/device/x/config/y/read -> issue a read command to the specific device
    MQTTClient_subscribe(client, "kHome/device/+/config/+/read", QOS);

    //topic kHome/device/x/config/y/write -> issue a write command to the specific device
    MQTTClient_subscribe(client, "kHome/device/+/config/+/write", QOS);

    //topic kHome/device/x/status/y/read -> issue a read command to the specific device
    MQTTClient_subscribe(client, "kHome/device/+/status/+/read", QOS);
}

/**
 * Close the connection to the MQTT Broker
 */
void mqttClose(void){
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
}

/**
 * This function will publish a device's data register to the topic 'kHome/device/x/data/y/value', where
 * x is the device's address in decimal and
 * y is the address of the data register in decimal
 */
void mqttPublishDataRegister(uint8_t deviceAddress, uint8_t registerAddress, uint32_t value){
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;

	char topicString[64];
	sprintf(topicString, "kHome/device/%d/data/%d/value", deviceAddress, registerAddress);

	char payloadData[16];
	sprintf(payloadData, "%d", value);

	pubmsg.payload = payloadData;
    pubmsg.payloadlen = strlen(payloadData);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, topicString, &pubmsg, &token);
    logWrite(1, "MQTT: Waiting for publication of %s on topic %s", payloadData, topicString);
    /*int rc = */MQTTClient_waitForCompletion(client, token, TIMEOUT);
    logWrite(1, "MQTT: Message with delivery token %d delivered", token);
}

/**
 * This function will publish a device's config register to the topic 'kHome/device/x/config/y/value', where
 * x is the device's address in decimal and
 * y is the address of the config register in decimal
 */
void mqttPublishConfigRegister(uint8_t deviceAddress, uint8_t registerAddress, uint8_t value){
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    char topicString[64];
    sprintf(topicString, "kHome/device/%d/config/%d/value", deviceAddress, registerAddress);

    char payloadData[16];
    sprintf(payloadData, "%d", value);

    pubmsg.payload = payloadData;
    pubmsg.payloadlen = strlen(payloadData);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, topicString, &pubmsg, &token);
    logWrite(1, "MQTT: Waiting for publication of %s on topic %s", payloadData, topicString);
    /*int rc = */MQTTClient_waitForCompletion(client, token, TIMEOUT);
    logWrite(1, "MQTT: Message with delivery token %d delivered", token);
}

/**
 * This function will publish a device's status register to the topic 'kHome/device/x/status/y/value', where
 * x is the device's address in decimal and
 * y is the address of the config register in decimal
 */
void mqttPublishStatusRegister(uint8_t deviceAddress, uint8_t registerAddress, uint8_t value){
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    char topicString[64];
    sprintf(topicString, "kHome/device/%d/status/%d/value", deviceAddress, registerAddress);

    char payloadData[16];
    sprintf(payloadData, "%d", value);

    pubmsg.payload = payloadData;
    pubmsg.payloadlen = strlen(payloadData);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, topicString, &pubmsg, &token);
    logWrite(1, "MQTT: Waiting for publication of %s on topic %s", payloadData, topicString);
    /*int rc = */MQTTClient_waitForCompletion(client, token, TIMEOUT);
    logWrite(1, "MQTT: Message with delivery token %d delivered", token);
}