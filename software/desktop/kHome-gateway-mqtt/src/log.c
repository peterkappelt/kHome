/**
 * @file log.c
 * @author Peter Kappelt
 * Contains functions for logging, log levels, ...
 */

#include "log.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

/**
 * The current log level defines how many messages (and how important one's) get displayed/ logged/ saved
 * The loglevel may be in between 0 and 1023 (including these values)
 *
 * A loglevel of 0 means, that every message which occures will be logged. For example, the serialport will log, if a single byte was received
 * A loglevel of 1023 means, that only the important messages (events that have an impact on the program's execution) will be logged
 * A value inbetween differentiates between those states
 */
int currentLogLevel = 1;

/**
 * @brief log a message
 * It is used like the printf-function, but with the loglevel as the first argument
 * You could call it like this: logWrite(1, "This is a message: %d", value);
 * Please note, that a newline in the string is unnecessary. The function will apend a newline.
 * Furthermore, a timestamp will be in fron of the log line
 *
 * Currently, this function only prints to STDOUT
 *
 * @param level 	Loglevel of the message. Can be between 0 and 1023 (including these)
 * @param template 	Template string, like you'd use with printf()
 */
void logWrite(int level, char* template, ...){
	if(level >= currentLogLevel){
		time_t ltime; /* calendar time */
    	ltime=time(NULL); /* get current cal time */
    	printf("[%s] %d:\t", strtok(asctime(localtime(&ltime)), "\n"), level);

		va_list args;
		va_start(args, template);
		vfprintf(stdout, template, args);
		va_end(args);
		printf("\r\n");
	}
}