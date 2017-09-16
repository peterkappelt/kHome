#!/usr/bin/python

import sys
import re
import json
import serial
import io

import paho.mqtt.client as mqtt

#serial port definitions
serialPortName = "/dev/ttyACM0"
serialPortBaud = 115200

#mqtt definitions
mqttServer = "hcpi01.int.kappelt.net"

def handleMessage(msg):
    msgParts = msg.split('|');
    if len(msgParts) < 2:
        print "Unknown Message \"" + msg + "\" (len too small)"
        return
    
    rssi = msgParts[0]

    data = json.loads(msgParts[1])
    if not 'hwtype' in data:
        print "HWType not specified: " + msg
        return
    
    if data['hwtype'] == 'khtempv1':
        if not ('temp' in data and 'battpct' in data and 'batt' in data and 'rh' in data and 'id' in data):
            print "A necessary key is not specified: " + msg
            return

        mqttclient.publish('kh/temperatureSensor/' + data['id'] + '/rssi', rssi)
        mqttclient.publish('kh/temperatureSensor/' + data['id'] + '/temperature', data['temp'])
        mqttclient.publish('kh/temperatureSensor/' + data['id'] + '/battery', data['battpct'])
        mqttclient.publish('kh/temperatureSensor/' + data['id'] + '/batteryVoltage', data['batt'])
        mqttclient.publish('kh/temperatureSensor/' + data['id'] + '/humidity', data['rh'])
    else:
        print "Unknown HWType: " + msg
        return


serial = serial.Serial(serialPortName, serialPortBaud, timeout=None)

if serial.isOpen() != True:
    print "Port wasn't opened, exiting!"
    sys.exit

port = io.TextIOWrapper(io.BufferedRWPair(serial, serial, 1), newline = '\r\n', encoding = 'ascii')

mqttclient = mqtt.Client()
mqttclient.connect(mqttServer)

while 1:
    recv = port.readline()

    recv = re.sub('\r', '', recv)
    recv = re.sub('\n', '', recv)

    handleMessage(recv)