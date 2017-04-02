/**
 * @file khRF.h
 * @author Peter Kappelt
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

void khRFInit(void);
void khRFTransmitTelegram(khTelegram telegram);
void khRFBroadcastDataRegister(uint8_t dataRegisterAddress);
void khRFReceiveMode(void);

#include "smartRF/smartrf_settings.h"

extern RF_Handle rfHandle;

#endif /* KHOME_KHRF_H_ */
