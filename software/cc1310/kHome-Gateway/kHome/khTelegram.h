/**
 * @file khTelegram.h
 * @author Peter Kappelt
 */

/**
 * Version History:
 * � V1.1 (20.4.2017)
 *  - added this version history
 * � V1.0 (19.4.2017)
 *  - initial revision
 */

#ifndef KHOME_KHTELEGRAM_H_
#define KHOME_KHTELEGRAM_H_

#ifndef KHOME_OWN_ADDRESS_CONFIG_REGISTER
//Address of the config register, which contains the device's address
#define KHOME_OWN_ADDRESS_CONFIG_REGISTER 0x00
#endif

#include <stdint.h>

typedef enum{
    khTelType_REG_W = 0x01,
    khTelType_REG_R = 0x02,
    khTelType_REG_B = 0x03,
    khTelType_CNF_W = 0x04,
    khTelType_CNF_R = 0x05,
    khTelType_STS_R = 0x06,
    khTelType_ANS = 0xFF,
}khTelType;

typedef enum{
    khTelStat_OK = 0x00,
    khTelStat_UnsupportedProtocol,
    khTelStat_CRCError,
    khTelStat_MemoryError,
    khTelStat_InvalidTelegram,
}khTelStat;

typedef struct{
    khTelType telegramType;
    uint8_t senderAddress;
    uint8_t receiverAddress;
    uint8_t payloadLength;
    uint8_t* payloadData;
    uint8_t crc;
}khTelegram;

void khTelegramInit(void);
uint8_t khTelegramGetDeviceAddress(void);
uint8_t khTelegramToByteArray(khTelegram telegram, uint8_t* byteArray);
khTelStat khByteArrayToTelegram(uint8_t* telegramArray, uint8_t telegramLength, khTelegram* telegram);
uint8_t khCalculateCRC8OfByteArray(uint8_t* byteArray, uint8_t length);
void khTelegramHandle(khTelegram telegram, khTelegram* answerTelegram, uint8_t* answerTelegramIsNecessary);

#endif /* KHOME_KHTELEGRAM_H_ */
