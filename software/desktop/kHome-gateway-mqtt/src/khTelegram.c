/**
 * @file khTelegram.c
 * @author Peter Kappelt
 * @date 8.4.2017
 *
 * Processing of kHome-Telegrams, adjusted for khSerial to MQTT gateway
 */

#include <stddef.h>
#include <stdlib.h>

#include "khTelegram.h"

/**
 * @brief convert a telegram to a byte array
 * @param[in]   telegram        a khTelegram
 * @param[out]  data            a pointer to an uint8_t array (must be 206 bytes long in order to be valid in all cases)
 * @return length of the data that was written
 */
uint8_t khTelegramToByteArray(khTelegram telegram, uint8_t* byteArray){
    byteArray[0] = 0x01;                            //protocol type, currently always 0x01
    byteArray[1] = (uint8_t)telegram.telegramType;  //telegram type
    byteArray[2] = telegram.senderAddress;          //own address
    byteArray[3] = telegram.receiverAddress;        //receiver's address

    if(telegram.payloadLength > 200){
        telegram.payloadLength = 200;               //limit the payload length to 200 byte
    }
    byteArray[4] = telegram.payloadLength;          //length of the payload

    uint8_t i;

    for(i = 0; i < telegram.payloadLength; i++){    //payload data
        byteArray[5 + i] = telegram.payloadData[i];
    }

    byteArray[5 + telegram.payloadLength] = khCalculateCRC8OfByteArray(byteArray, 5 + telegram.payloadLength);

    return (telegram.payloadLength + 6);
}

/**
 * @brief parse a kHome telegram in a byte array
 * @param[in] telegramArray     a pointer to an uint8_t array, which contains the telegram
 * @param[in] telegramLength    the length of the telegramArray
 * @param[out] telegram         a pointer to a khTelegram, where the telegram gets written to
 *
 * @return a element of khTelStat, according to the success
 */
khTelStat khByteArrayToTelegram(uint8_t* telegramArray, uint8_t telegramLength, khTelegram* telegram){
    if(telegramLength < 6){
        return khTelStat_InvalidTelegram;
    }
    if(telegramArray[0] != 0x01){
        return khTelStat_UnsupportedProtocol;
    }

    telegram->telegramType = (khTelType) telegramArray[1];
    telegram->senderAddress = telegramArray[2];
    telegram->receiverAddress = telegramArray[3];
    telegram->payloadLength = telegramArray[4];

    //process payload data and length
    uint8_t* payloadData = (uint8_t*) malloc(telegram->payloadLength);
    if(payloadData == NULL){
        return khTelStat_MemoryError;
    }
    telegram->payloadData = payloadData;
    uint8_t i;
    for(i = 0; i < telegram->payloadLength; i++){
        payloadData[i] = telegramArray[5 + i];
    }

    //telegram crc
    telegram->crc = telegramArray[5 + telegram->payloadLength];
    uint8_t crcCalulated = khCalculateCRC8OfByteArray(telegramArray, 5 + telegram->payloadLength);
    if(crcCalulated != telegram->crc){
        return khTelStat_CRCError;
    }

    //everything went through without an issue
    return khTelStat_OK;
}

/**
 * @brief handle the payload data of a telegram
 */
void khTelegramHandle(khTelegram telegram, khTelegram* answerTelegram, uint8_t* answerTelegramIsNecessary){
    answerTelegram->telegramType = khTelType_ANS;
    answerTelegram->senderAddress = telegram.receiverAddress;              //swap sender and receiver's address
    answerTelegram->receiverAddress = telegram.senderAddress;

    if(telegram.telegramType == khTelType_REG_W){

    }else if(telegram.telegramType == khTelType_REG_R){

    }else if(telegram.telegramType == khTelType_REG_B){

    }else if(telegram.telegramType == khTelType_CNF_W){

    }else if(telegram.telegramType == khTelType_CNF_R){

    }else if(telegram.telegramType == khTelType_STS_R){

    }else if(telegram.telegramType == khTelType_ANS){

    }else{
        //todo: user telegrams

        *answerTelegramIsNecessary = 0;
        return;
    }
}

/**
 * @brief calculate the CRC-8 of a uint8_t array
 * @param[in] byteArray     pointer to a array
 * @param[in] length        length of the array
 *
 * @return CRC-8 of the value
 */
uint8_t khCalculateCRC8OfByteArray(uint8_t* byteArray, uint8_t length){
    //Implementation is from Texas Instruments DN502, but modified for CRC-8
    const uint8_t polynomial = 0x07;        //CRC polynomial
    uint8_t checksum = 0;                   //initial value

    uint8_t i, j, currentCRCByte;

    for(j = 0; j < length; j++){
        currentCRCByte = byteArray[j];
        for(i = 0; i < 8; i++){
            if((checksum & 0x80) ^ (currentCRCByte & 0x80)){
                checksum = (checksum << 1) ^ polynomial;
            }else{
                checksum = (checksum << 1);
            }
            currentCRCByte <<= 1;
        }
    }

    return checksum;
}
