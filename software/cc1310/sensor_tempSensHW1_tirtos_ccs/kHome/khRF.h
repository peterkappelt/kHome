/**
 * @file khRF.h
 * @author Peter Kappelt
 */

/**
 * Version History:
 * • V1.1 (20.4.2017)
 *  - added this version history
 * • V1.0 (19.4.2017)
 *  - initial revision
 */

#ifndef KHOME_KHRF_H_
#define KHOME_KHRF_H_

#include <stdint.h>
#include "khTelegram.h"

typedef enum{
    khRFMode_NotOpened = 0,
    khRFMode_Idle,
    khRFMode_TX,
    khRFMode_RX,
}khRFMode;

uint8_t khRFTmpIsInReceiveMode(void);

void khRFInit(void);
void khRFTransmitTelegram(khTelegram telegram);
void khRFBroadcastDataRegister(uint8_t dataRegisterAddress);
void khRFReceiveMode(void);
void khRFReceiveModeStop(void);

#endif /* KHOME_KHRF_H_ */
