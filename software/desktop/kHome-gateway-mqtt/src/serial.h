/**
 * @file serial.h
 * @author Peter Kappelt
 */

/**
 * Software will wait this long for an answer to a command, that was send previously
 * The unit is µs (microseconds)
 * This value must be less than 1 second (1 000 000 µs)
 */
#define answerWaitTimeout 500000

#include "kBuffer/kBuffer.h"

extern buffer_t bufferMQTTToSerial;

void serialInit(void);
void serialOpen(void);
int serialGetFileHandle(void);
void serialLoop(void);