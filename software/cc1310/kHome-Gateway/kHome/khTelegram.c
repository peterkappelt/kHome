/**
 * @file khTelegram.c
 * @author Peter Kappelt
 * @date 25.3.2017
 *
 * Processing of kHome-Telegrams
 */

#include <stddef.h>
#include <stdlib.h>

#include "khRegister.h"
#include "khTelegram.h"

uint8_t* ownAddress;

/**
 * @brief init the telegram functions
 */
void khTelegramInit(void){
    khReg* ownAddressConfigRegister;

    khRegRet ownAddressReturn = khGetRegByAddress(khRegType_Config, KHOME_OWN_ADDRESS_CONFIG_REGISTER, &ownAddressConfigRegister);

    if(ownAddressReturn != khRegRet_OK){
        ownAddress = (uint8_t*)malloc(sizeof(uint8_t));
        *ownAddress = 0;
    }else{
        ownAddress = ownAddressConfigRegister->dataLen1;
    }

}

/**
 * @brief get the address of the register
 */
uint8_t khTelegramGetDeviceAddress(void){
    return *ownAddress;
}

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
    if(telegram.receiverAddress != *ownAddress){
        answerTelegramIsNecessary = 0;
        return;
    }


    answerTelegram->telegramType = khTelType_ANS;
    answerTelegram->senderAddress = telegram.receiverAddress;              //swap sender and receiver's address
    answerTelegram->receiverAddress = telegram.senderAddress;

    if(telegram.telegramType == khTelType_REG_W){
        *answerTelegramIsNecessary = 1;
        answerTelegram->payloadData[1] = telegram.telegramType;
        if(telegram.payloadLength < 1){
            answerTelegram->payloadLength = 2;
            answerTelegram->payloadData[0] = 0xFF;                      //unknown register address
            //next byte is type of telegram we are answering to, is defined above
        }else if((telegram.payloadLength != 2) && (telegram.payloadLength != 3) && (telegram.payloadLength != 5)){
            answerTelegram->payloadLength = 2;
            answerTelegram->payloadData[0] = 0xFD;                      //invalid value (length of value)
            //next byte is type of telegram we are answering to, is defined above
        }else{
            uint32_t newRegisterValue;
            uint8_t registerAddress = telegram.payloadData[0];

            switch(telegram.payloadLength){
            case 2:
                //1 byte datalength
                newRegisterValue = telegram.payloadData[1];
                break;
            case 3:
                //2 byte datalength
                newRegisterValue = telegram.payloadData[1] << 8;
                newRegisterValue += telegram.payloadData[2];
                break;
            case 5:
                //4 byte datalength
                newRegisterValue = telegram.payloadData[1] << 24;
                newRegisterValue += telegram.payloadData[2] << 16;
                newRegisterValue += telegram.payloadData[3] << 8;
                newRegisterValue += telegram.payloadData[4];
                break;
            }

            khRegRet returnCode = khSetDataRegValByAddress(registerAddress, newRegisterValue, 0);
            if(returnCode == khRegRet_RegUnknown){
                answerTelegram->payloadLength = 2;
                answerTelegram->payloadData[0] = 0xFC;              //unknown register address
                //next byte is type of telegram we are answering to, is defined above
            }else if(returnCode == khRegRet_RegReadOnly){
                answerTelegram->payloadLength = 2;
                answerTelegram->payloadData[0] = 0xFE;              //register is read only
            }else{
                uint32_t registerValueReadback;
                uint8_t registerLengthReadback;
                if(khGetDataRegValByAddress(registerAddress, &registerLengthReadback, &registerValueReadback) != khRegRet_OK){
                    answerTelegram->payloadLength = 2;
                    answerTelegram->payloadData[0] = 0xFF;                  //unknown register address
                    //next byte is type of telegram we are answering to, is defined above
                }else{
                    answerTelegram->payloadLength = 2 + registerLengthReadback;
                    answerTelegram->payloadData[0] = 0x00;                  //answer code: OK
                    //next byte is type of telegram we are answering to, is defined above

                    switch(registerLengthReadback){
                    case 4:
                        answerTelegram->payloadData[2] = (registerValueReadback & 0xFF000000) >> 24;
                        answerTelegram->payloadData[3] = (registerValueReadback & 0x00FF0000) >> 16;
                        answerTelegram->payloadData[4] = (registerValueReadback & 0x0000FF00) >> 8;
                        answerTelegram->payloadData[5] = (registerValueReadback & 0x000000FF) >> 0;
                        break;
                    case 2:
                        answerTelegram->payloadData[2] = (registerValueReadback & 0x0000FF00) >> 8;
                        answerTelegram->payloadData[3] = (registerValueReadback & 0x000000FF) >> 0;
                        break;
                    case 1:
                        answerTelegram->payloadData[2] = (registerValueReadback & 0x000000FF) >> 0;
                        break;
                    default:
                        answerTelegram->payloadData[2] = 0;
                        break;
                    }
                }

            }
        }
    }else if(telegram.telegramType == khTelType_REG_R){
        *answerTelegramIsNecessary = 1;
        answerTelegram->payloadData[1] = telegram.telegramType;         //type of telegram we are answering to
        if(telegram.payloadLength < 1){
            answerTelegram->payloadLength = 2;
            answerTelegram->payloadData[0] = 0xFF;                      //unknown register address
            //next byte is type of telegram we are answering to, is defined above
        }else{
            uint32_t registerValue;
            uint8_t registerLength;
            uint8_t registerAddress = telegram.payloadData[0];
            if(khGetDataRegValByAddress(registerAddress, &registerLength, &registerValue) != khRegRet_OK){
                answerTelegram->payloadLength = 2;
                answerTelegram->payloadData[0] = 0xFF;                  //unknown register address
                //next byte is type of telegram we are answering to, is defined above
            }else{
                answerTelegram->payloadLength = 2 + registerLength;
                answerTelegram->payloadData[0] = 0x00;                  //answer code: OK
                //next byte is type of telegram we are answering to, is defined above

                switch(registerLength){
                case 4:
                    answerTelegram->payloadData[2] = (registerValue & 0xFF000000) >> 24;
                    answerTelegram->payloadData[3] = (registerValue & 0x00FF0000) >> 16;
                    answerTelegram->payloadData[4] = (registerValue & 0x0000FF00) >> 8;
                    answerTelegram->payloadData[5] = (registerValue & 0x000000FF) >> 0;
                    break;
                case 2:
                    answerTelegram->payloadData[2] = (registerValue & 0x0000FF00) >> 8;
                    answerTelegram->payloadData[3] = (registerValue & 0x000000FF) >> 0;
                    break;
                case 1:
                    answerTelegram->payloadData[2] = (registerValue & 0x000000FF) >> 0;
                    break;
                default:
                    answerTelegram->payloadData[2] = 0;
                    break;
                }
            }
        }
    }else if(telegram.telegramType == khTelType_REG_B){
        *answerTelegramIsNecessary = 0;
    }else if(telegram.telegramType == khTelType_CNF_W){
        *answerTelegramIsNecessary = 1;
        answerTelegram->payloadData[1] = telegram.telegramType;
        if(telegram.payloadLength < 1){
            answerTelegram->payloadLength = 2;
            answerTelegram->payloadData[0] = 0xFF;                      //unknown register address
            //next byte is type of telegram we are answering to, is defined above
        }else if(telegram.payloadLength != 2){
            answerTelegram->payloadLength = 2;
            answerTelegram->payloadData[0] = 0xFC;                      //invalid value (length of value)
            //next byte is type of telegram we are answering to, is defined above
        }else{
            uint8_t newRegisterValue = telegram.payloadData[1];
            uint8_t registerAddress = telegram.payloadData[0];

            khRegRet returnCode = khSetConfigRegValByAddress(registerAddress, newRegisterValue, 0);
            if(returnCode == khRegRet_RegUnknown){
                answerTelegram->payloadLength = 2;
                answerTelegram->payloadData[0] = 0xFF;              //unknown register address
                //next byte is type of telegram we are answering to, is defined above
            }else if(returnCode == khRegRet_RegReadOnly){
                answerTelegram->payloadLength = 2;
                answerTelegram->payloadData[0] = 0xFE;              //register is read only
            }else{
                uint8_t registerValueReadback;
                if(khGetConfigRegValByAddress(registerAddress, &registerValueReadback) != khRegRet_OK){
                    answerTelegram->payloadLength = 2;
                    answerTelegram->payloadData[0] = 0xFF;                  //unknown register address
                    //next byte is type of telegram we are answering to, is defined above
                }else{
                    answerTelegram->payloadLength = 3;
                    answerTelegram->payloadData[0] = 0x00;                  //answer code: OK
                    //next byte is type of telegram we are answering to, is defined above

                    answerTelegram->payloadData[2] = registerValueReadback;
                }

            }
        }
    }else if(telegram.telegramType == khTelType_CNF_R){
        *answerTelegramIsNecessary = 1;
        answerTelegram->payloadData[1] = telegram.telegramType;         //type of telegram we are answering to
        if(telegram.payloadLength < 1){
            answerTelegram->payloadLength = 2;
            answerTelegram->payloadData[0] = 0xFF;                      //unknown register address
            //next byte is type of telegram we are answering to, is defined above
        }else{
            uint8_t registerValue;
            uint8_t registerAddress = telegram.payloadData[0];
            if(khGetConfigRegValByAddress(registerAddress, &registerValue) != khRegRet_OK){
                answerTelegram->payloadLength = 2;
                answerTelegram->payloadData[0] = 0xFF;                  //unknown register address
                //next byte is type of telegram we are answering to, is defined above
            }else{
                answerTelegram->payloadLength = 3;
                answerTelegram->payloadData[0] = 0x00;                  //answer code: OK
                //next byte (#1) is type of telegram we are answering to, is defined above

                answerTelegram->payloadData[2] = registerValue;
            }
        }
    }else if(telegram.telegramType == khTelType_STS_R){
        *answerTelegramIsNecessary = 1;
        answerTelegram->payloadData[1] = telegram.telegramType;         //type of telegram we are answering to
        if(telegram.payloadLength < 1){
            answerTelegram->payloadLength = 2;
            answerTelegram->payloadData[0] = 0xFF;                      //unknown register address
            //next byte is type of telegram we are answering to, is defined above
        }else{
            uint8_t registerValue;
            uint8_t registerAddress = telegram.payloadData[0];
            if(khGetStatusRegValByAddress(registerAddress, &registerValue) != khRegRet_OK){
                answerTelegram->payloadLength = 2;
                answerTelegram->payloadData[0] = 0xFF;                  //unknown register address
                //next byte is type of telegram we are answering to, is defined above
            }else{
                answerTelegram->payloadLength = 3;
                answerTelegram->payloadData[0] = 0x00;                  //answer code: OK
                //next byte (#1) is type of telegram we are answering to, is defined above

                answerTelegram->payloadData[2] = registerValue;
            }
        }
    }else if(telegram.telegramType == khTelType_ANS){
        *answerTelegramIsNecessary = 0;
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
