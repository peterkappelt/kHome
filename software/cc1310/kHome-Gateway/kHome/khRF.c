/**
 * @file khRF.c
 * @author Peter Kappelt
 * @date 27.3.2017
 *
 * kHome RF Media functions
 */

#include "khRF.h"
#include "khRegister.h"
#include "khTelegram.h"

#include <stdlib.h>

#include "smartRF/smartrf_settings.h"

#include DEVICE_FAMILY_PATH(driverlib/rf_data_entry.h)
#include DEVICE_FAMILY_PATH(driverlib/rf_prop_mailbox.h)

static RF_Object rfObject;
/*static*/ RF_Handle rfHandle;
static RF_Params rfParams;

khRFMode currentRFMode = khRFMode_NotOpened;

#define RADIO_EVENTS_ALL_RELEVANT (RF_EventLastCmdDone | RF_EventCmdError | RF_EventCmdAborted | RF_EventCmdStopped | RF_EventCmdCancelled)


#pragma DATA_ALIGN (rxBuffer, 4);
uint8_t rxBuffer[sizeof(rfc_dataEntryGeneral_t) + 255];

static dataQueue_t rxDataQueue;
static rfc_propRxOutput_t rxStatistics;
static RF_CmdHandle rxCmdHandle;

static uint8_t txBuffer[20];            //the commands of this device won't be longer than 20 bytes
uint8_t txReady = 1;

/**
 * @brief initialize the radio and local config
 */
void khRFInit(void){
    RF_Params_init(&rfParams);

    rfParams.nInactivityTimeout = 4000000/1000;     //1 ms

    rfHandle = RF_open(&rfObject, &RF_prop, (RF_RadioSetup*)&khRF_cmdPropRadioDivSetup, &rfParams);

    if(rfHandle != NULL){
        currentRFMode = khRFMode_Idle;
    }else{
        currentRFMode = khRFMode_NotOpened;
        return;
    }

    RF_runCmd(rfHandle, (RF_Op*)&khRF_cmdFs, RF_PriorityNormal, 0, RADIO_EVENTS_ALL_RELEVANT);

    khRF_cmdPropTx.startTrigger.triggerType = TRIG_NOW;
    khRF_cmdPropTx.startTrigger.pastTrig = 1;
    khRF_cmdPropTx.startTime = 0;

    //go to receive mode (default)
    //khRFReceiveMode();
}

static void txCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e){
    txReady = 1;
    if (e & RF_EventLastCmdDone){
        //successfully transmitted
    }
    else if ( (e & RF_EventCmdAborted) || (e & RF_EventCmdCancelled ) ){
        //tx aborted
    }
    else{
        //general tx error
    }
}

void khRFTransmitTelegram(khTelegram telegram){
    //todo switch to tx from rx mode

    if(currentRFMode == khRFMode_NotOpened){
        return;                                             //the radio is not opened
    }

    //wait for a previous transmit to finish
    while(!txReady);

    //convert the nice telegramm to a stream of bytes for sending
    uint8_t broadcastTelegramBytesLength = khTelegramToByteArray(telegram, &txBuffer[0]);

    //configure them for the radio, send the packet and wait for it to be sent
    khRF_cmdPropTx.pktLen = broadcastTelegramBytesLength;
    khRF_cmdPropTx.pPkt = &txBuffer[0];

    khRF_cmdPropTx.startTrigger.triggerType = TRIG_NOW;
    khRF_cmdPropTx.startTrigger.pastTrig = 1;
    khRF_cmdPropTx.startTime = 0;

    //todo: handle returns
    //todo: check if opened
    RF_CmdHandle cmdHdl = RF_postCmd(rfHandle, (RF_Op*)&khRF_cmdPropTx, RF_PriorityNormal, txCallback, RADIO_EVENTS_ALL_RELEVANT);
    // Wait for Command to complete -> commented out; is done in async callback
    //RF_EventMask result = RF_pendCmd(rfHandle, cmdHdl,  (RF_EventLastCmdDone | RF_EventCmdError));
}

/**
 * @brief Broadcast a data register (REG_B)
 *
 * @param[in] dataRegisterAddress   The address of the data register to broadcast
 */
void khRFBroadcastDataRegister(uint8_t dataRegisterAddress){
    uint8_t registerLength;
    uint32_t registerValue;

    khRegRet returnCodeRegister = khGetDataRegValByAddress(dataRegisterAddress, &registerLength, &registerValue);

    //Todo: check returnCodeRegister

    uint8_t broadcastTelegramPayload[5];
    khTelegram broadcastTelegram;
    broadcastTelegram.telegramType = khTelType_REG_B;
    broadcastTelegram.receiverAddress = 0xFF;               //receiver's address is alway 0xFF at broadcast
    broadcastTelegram.senderAddress = 0x01;                 //temp: handle own address
    broadcastTelegram.payloadLength = 1 + registerLength;   //payload is as follows: |1x register address|1x-4x register data
    broadcastTelegram.payloadData = &broadcastTelegramPayload[0];

    broadcastTelegramPayload[0] = dataRegisterAddress;      //byte 0 of payload is the register's address

    //the rest of the payload depends on on the length
    switch(registerLength){
    case 4:
        broadcastTelegramPayload[1] = (registerValue & 0xFF000000) >> 24;
        broadcastTelegramPayload[2] = (registerValue & 0x00FF0000) >> 16;
        broadcastTelegramPayload[3] = (registerValue & 0x0000FF00) >> 8;
        broadcastTelegramPayload[4] = (registerValue & 0x000000FF) >> 0;
        break;
    case 2:
        broadcastTelegramPayload[1] = (registerValue & 0x0000FF00) >> 8;
        broadcastTelegramPayload[2] = (registerValue & 0x000000FF) >> 0;
        break;
    case 1:
        broadcastTelegramPayload[1] = (registerValue & 0x000000FF) >> 0;
        break;
    default:
        broadcastTelegramPayload[1] = 0;
        break;
    }

    khRFTransmitTelegram(broadcastTelegram);
}

static void rxCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e){
    rfc_dataEntryGeneral_t *pDataEntry;
    pDataEntry = (rfc_dataEntryGeneral_t*) &rxBuffer[0];

    //invalidate handle
    rxCmdHandle = -1;

    if(e & RF_EventLastCmdDone){
        if(khRF_cmdPropRx.status == PROP_DONE_OK){
            if(pDataEntry->status != DATA_ENTRY_FINISHED){
                //general RX Error
            }else if(rxStatistics.nRxOk == 1){
                uint8_t length = *(uint8_t*)(&pDataEntry->data);

                khTelegram parsedTelegram;
                khTelStat parseStat = khByteArrayToTelegram(&pDataEntry->data + 1, length, &parsedTelegram);

                //any other telegram type than defined does not need an answer
                if(parseStat == khTelStat_CRCError){
                    khTelegram answerTelegram;
                    uint8_t answerTelegramData[2];

                    answerTelegram.telegramType = khTelType_ANS;
                    answerTelegram.senderAddress = 0x01;            //todo get own address
                    answerTelegram.receiverAddress = 0xFF;
                    answerTelegram.payloadLength = 2;
                    answerTelegram.payloadData = &answerTelegramData[0];

                    answerTelegramData[0] = 0xFD;                   //CRC problem
                    answerTelegramData[1] = 0xFD;

                    khRFTransmitTelegram(answerTelegram);
                }else if(parseStat == khTelStat_OK){
                    uint8_t answerTelegramIsNecessary;

                    khTelegram answerTelegram;
                    uint8_t answerTelegramData[32];
                    answerTelegram.payloadData = &answerTelegramData[0];

                    khTelegramHandle(parsedTelegram, &answerTelegram, &answerTelegramIsNecessary);

                    if(answerTelegramIsNecessary){
                        khRFTransmitTelegram(answerTelegram);
                    }
                }

                free(parsedTelegram.payloadData);
            }else if(rxStatistics.nRxBufFull == 1){
                //buffer overflow
            }else if(rxStatistics.nRxStopped == 1){
                //aborted -> go to tx?
            }else{
                //general RX error
            }
        }else if(khRF_cmdPropRx.status == PROP_DONE_RXTIMEOUT){
            //receive timeout; shouldn't happen in current config
        }else{
            //todo error
        }

        //restart receiving
        khRFReceiveMode();
    }else if((e == RF_EventCmdAborted) || (e == RF_EventCmdStopped)){
        //todo user aborted -> go to tx mode?
    }
}

void khRFReceiveMode(void){
    if(currentRFMode == khRFMode_NotOpened){
        return;
    }

    if(currentRFMode == khRFMode_TX){
        //Todo abort TX?
    }

    if(currentRFMode == khRFMode_RX){
        if(rxCmdHandle >= 0){
            //we are already in receive mode
            return;
        }
    }

    rfc_dataEntryGeneral_t *pDataEntry;
    pDataEntry = (rfc_dataEntryGeneral_t*)&rxBuffer[0];

    pDataEntry->length = 255;
    pDataEntry->status = 0;

    rxDataQueue.pCurrEntry = (uint8_t*)pDataEntry;
    rxDataQueue.pLastEntry = NULL;

    khRF_cmdPropRx.pQueue = &rxDataQueue;
    khRF_cmdPropRx.pOutput = (uint8_t*)&rxStatistics;

    khRF_cmdPropRx.startTrigger.triggerType = TRIG_NOW;
    khRF_cmdPropRx.startTrigger.pastTrig = 1;
    khRF_cmdPropRx.startTime = 0;
    khRF_cmdPropRx.endTrigger.triggerType = TRIG_NEVER;
    khRF_cmdPropRx.endTime = 0;

    memset(&rxStatistics, 0, sizeof(rfc_propRxOutput_t));

    rxCmdHandle = RF_postCmd(rfHandle, (RF_Op*)&khRF_cmdPropRx, RF_PriorityNormal, rxCallback, RADIO_EVENTS_ALL_RELEVANT);

    currentRFMode = khRFMode_RX;
}
