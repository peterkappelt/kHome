/**
 * @file khSerial.h
 * @author Peter Kappelt
 */

#ifndef KHOME_KHSERIAL_H_
#define KHOME_KHSERIAL_H_

#define khSerial_Baudrate 115200

#include "khTelegram.h"

void khSerialInit(void);
void khSerialTransmitTelegram(khTelegram telegram);

#endif /* KHOME_KHSERIAL_H_ */
