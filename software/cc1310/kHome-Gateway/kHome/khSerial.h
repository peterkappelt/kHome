/**
 * @file khSerial.h
 * @author Peter Kappelt
 */

/**
 * Version History:
 * • V1.1 (20.4.2017)
 *  - added this version history
 * • V1.0 (19.4.2017)
 *  - initial revision
 */

#ifndef KHOME_KHSERIAL_H_
#define KHOME_KHSERIAL_H_

#define khSerial_Baudrate 115200

#include "khTelegram.h"

void khSerialInit(void);
void khSerialTransmitTelegram(khTelegram telegram);

#endif /* KHOME_KHSERIAL_H_ */
