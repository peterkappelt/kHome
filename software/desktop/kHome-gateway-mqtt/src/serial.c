/**
 * @file serial.c
 * @author Peter Kappelt
 * Contains functions for handling of the serial port (opening, read, ...) and buffering
 */

#include "serial.h"
#include "log.h"
#include "khTelegram.h"
#include "mqtt.h"

#include <stdlib.h>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

/**
 * String of ther serial port's device
 * @todo ask the user (like with a command line option) -> is currently hardcoded
 */
char* serialPortName = "/dev/ttyACM0";
int fileHandle;

uint8_t receiveBuffer[256];
/**
 * the receiveBufferIndex contains the index of the element of the array, that isn't yet written
 * If it is 2, for instance, receiveBuffer[2] is undefined, but the two values before are defined
 * Due to the zero-shift this value also indicates the number of bytes in the buffer
 */
unsigned int receiveBufferIndex = 0;


/**
 * set various attributes of the serial port, like the baudrate, parity, ...
 */
int serialSetInterfaceAttributes(int fd, int speed, int parity){
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                logWrite(1023, "error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //raw mode
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
        tty.c_iflag &= ~(ICRNL | INLCR);	//neither a Carriage Return, nor a Newline will be replaced.
        									//There was a problem, where the CR-Character was replaced with a NL. The line above fixes this.

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                logWrite(1023, "error %d from tcsetattr", errno);
                return -1;
        }
        return 0;
}

/**
 * set, whether a read operation is blocking or non-blocking
 */
void serialSetBlocking(int fd, int should_block){
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                logWrite(1023, "error %d from tggetattr", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                logWrite(1023, "error %d setting term attributes", errno);
}

/**
 * Open the serial port, if there is an error, print it and exit
 */
void serialOpen(void){
	fileHandle = open (serialPortName, O_RDWR | O_NOCTTY | O_SYNC);
	if (fileHandle < 0)
	{
	        logWrite(1023, "error %d opening %s: %s", errno, serialPortName, strerror(errno));
	        exit(0);
	}

	serialSetInterfaceAttributes(fileHandle, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
	serialSetBlocking(fileHandle, 0);                // set no blocking
}

/**
 * get the serial port's file handle, e.g. for external port operation
 */
int serialGetFileHandle(void){
	return fileHandle;
}

/**
 * Internal low-level function which is called, if a telegram is received that is of type REG_B
 */
void serialReceivedCompletePacket_LL_REG_B(khTelegram telegram){
	if(telegram.payloadLength < 2){
		//there must be at least two bytes of payload
		logWrite(2, "Payload Length of REG_B to short: is %d, must be at least 2", telegram.payloadLength);
		return;
	}
	if(telegram.payloadLength > 5){
		//there must be at most five bytes of payload
		logWrite(2, "Payload Length of REG_B to long: is %d, must be at most 2", telegram.payloadLength);
		return;
	}

	uint32_t registerData;

	switch(telegram.payloadLength){
		case 5:
			registerData = (telegram.payloadData[1] & 0xFF) << 24;
			registerData |= (telegram.payloadData[2] & 0xFF) << 16;
			registerData |= (telegram.payloadData[3] & 0xFF) << 8;
			registerData |= (telegram.payloadData[4] & 0xFF) << 0;
			break;
		case 3:
			registerData = (telegram.payloadData[1] & 0xFF) << 8;
			registerData |= (telegram.payloadData[2] & 0xFF) << 0;
			break;
		case 2:
			registerData = (telegram.payloadData[1] & 0xFF) << 0;
			break;	
		default:
			registerData = 0;
	}

	mqttPublishDataRegister(telegram.senderAddress, telegram.payloadData[0], registerData);
}

/**
 * This function should be colled, if a complete packet is received
 * A complete packet exists, if the data start with 0xAA and a \r\n is at the end
 */
void serialReceivedCompletePacket(void){
	khTelegram receivedTelegram;

	khTelStat returnCode = khByteArrayToTelegram(&receiveBuffer[1], receiveBufferIndex - 2, &receivedTelegram);

	switch(returnCode){
		case khTelStat_UnsupportedProtocol:
			logWrite(2, "Packet: Unsupported Protocol");
			break;
			return;
		case khTelStat_CRCError:
			logWrite(2, "Packet: CRC Error");
			break;
			return;
		case khTelStat_InvalidTelegram:
			logWrite(2, "Packet: Invalid Telegram");
			break;
			return;
		default:
			break;
	}

	char telTypeString[16];

	switch(receivedTelegram.telegramType){
		case khTelType_REG_W:
			sprintf(telTypeString, "REG_W");
			break;
		case khTelType_REG_R:
			 sprintf(telTypeString, "REG_R");
			break;
		case khTelType_REG_B:
			 sprintf(telTypeString, "REG_B");
			break;
		case khTelType_CNF_R:
			 sprintf(telTypeString, "CNF_R");
			break;
		case khTelType_CNF_W:
			 sprintf(telTypeString, "CNF_W");
			break;
		case khTelType_STS_R:
			 sprintf(telTypeString, "STS_R");
			break;
		case khTelType_ANS:
			 sprintf(telTypeString, "ANS");
			break;
		default:
			 sprintf(telTypeString, "Unknown");
	}

	//Start log writing
	char payloadString[200 * 3];
	receivedTelegram.payloadLength = (receivedTelegram.payloadLength > 200) ? 0:receivedTelegram.payloadLength;

	for(int i = 0; i < receivedTelegram.payloadLength; i++){
		sprintf(&payloadString[i * 3], "%02X ", receivedTelegram.payloadData[i]);
	}
	payloadString[receivedTelegram.payloadLength * 3] = 0;

	logWrite(2, "%s from %d to %d: %s", telTypeString, receivedTelegram.senderAddress, receivedTelegram.receiverAddress, payloadString);
	//End log

	switch(receivedTelegram.telegramType){
		case khTelType_REG_B:
			serialReceivedCompletePacket_LL_REG_B(receivedTelegram);
			break;
		default:
			logWrite(1023, "Packet: Telegram type not supported!");
	}

	if(receivedTelegram.payloadData != NULL){
		free(receivedTelegram.payloadData);
	}
}

/**
 * call this function regularly. It checks for new bytes in the buffer, and whether a complete packet was received.
 * If so, this function initiates the handling of it
 */
void serialLoop(void){
	char c;
 
	if(read(fileHandle, &c, 1) > 0){
		logWrite(0, "Byte Received: %02X", c & 0xFF);			//debug the byte value in loglevel zero
		if(receiveBufferIndex > 254){
			receiveBufferIndex = 0;
		}
		receiveBuffer[receiveBufferIndex++] = c & 0xFF;

		if(receiveBuffer[receiveBufferIndex - 1] == '\n'){		//the character was a newline. check the previous
			//for logging -> convert it to a hex string
			char* hexString = (char*)malloc(receiveBufferIndex * 3);
			unsigned int i;
			for(i = 0; i < receiveBufferIndex; i++){
				sprintf(&hexString[i * 3], "%02X ", receiveBuffer[i] & 0xFF);
			}
			hexString[receiveBufferIndex * 3] = 0;
			logWrite(1, "Packet: %s", hexString);
			if(hexString != NULL){
				free(hexString);
			}

			//in a telegram must be at least four byte
			if(receiveBufferIndex > 3){
				if(receiveBuffer[receiveBufferIndex - 2] == '\r'){
					if(receiveBuffer[0] == 0xAA){
						//handle telegram:
						serialReceivedCompletePacket();
					}
				}else{
					return;
				}
			}

			receiveBufferIndex = 0;
		}
	}
}