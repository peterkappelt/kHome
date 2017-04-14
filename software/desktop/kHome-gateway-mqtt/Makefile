CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=src/main.c src/log.c src/serial.c src/khTelegram.c src/mqtt.c src/kBuffer/kBuffer.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=kH-MQTT-gateway

all: $(SOURCES) $(EXECUTABLE)
run: $(EXECUTABLE)
	./kH-MQTT-gateway

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -Lsrc/paho-1.1.0/lib/ -lpaho-mqtt3c -o $@

.c.o:
	$(CC) $(CFLAGS) -ggdb -DbufferDatatype=khTelegram $< -o $@
