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


#ifdef KHOME_RF_SERIAL_GATEWAY
//transmit the telegram over serial if we use the gateway feature
    #include "khSerial.h"
#endif

#include <stdlib.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>

#include "smartRF/smartrf_settings.h"

//temp:
#include "Board.h"

#include DEVICE_FAMILY_PATH(driverlib/rf_data_entry.h)
#include DEVICE_FAMILY_PATH(driverlib/rf_prop_mailbox.h)

static RF_Object rfObject;
static RF_Handle rfHandle;
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
khRFMode txPreviousRFMode;

#define KHRFQUEUE_TASK_STACK_SIZE 256
#define KHRFQUEUE_TASK_PRIORITY 1

static Task_Params khRfReceivedTaskParams;
static Task_Struct khRfReceivedTask;
static uint8_t khRfReceivedTaskStack[KHRFQUEUE_TASK_STACK_SIZE];

static Event_Struct eventPacketReceived;
static Event_Handle eventPacketReceivedHandle;

static void khRfReceivedTaskFunction(UArg arg0, UArg arg1);

uint8_t hasReceivedUnhandledTelegram = 0;

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

    //init the task to restart receive mode
    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&eventPacketReceived, &eventParam);
    eventPacketReceivedHandle = Event_handle(&eventPacketReceived);

    Task_Params_init(&khRfReceivedTaskParams);
    khRfReceivedTaskParams.stackSize = KHRFQUEUE_TASK_STACK_SIZE;
    khRfReceivedTaskParams.priority = KHRFQUEUE_TASK_PRIORITY;
    khRfReceivedTaskParams.stack = &khRfReceivedTaskStack;
    Task_construct(&khRfReceivedTask, khRfReceivedTaskFunction, &khRfReceivedTaskParams, NULL);
}

static void khRfReceivedTaskFunction(UArg arg0, UArg arg1){
    rfc_dataEntryGeneral_t *pDataEntry;
    pDataEntry = (rfc_dataEntryGeneral_t*) &rxBuffer[0];

    while(1){
        uint32_t events = Event_pend(eventPacketReceivedHandle, 0, 0xFFFFFFFF, BIOS_WAIT_FOREVER);

        //event #1 is the only current implemented
        if(events & 1){
            if(khRF_cmdPropRx.status == PROP_DONE_OK){
                if(pDataEntry->status != DATA_ENTRY_FINISHED){
                    //general RX Error
                }else if(rxStatistics.nRxOk == 1){
                    uint8_t length = *(uint8_t*)(&pDataEntry->data);

                    khTelegram parsedTelegram;
                    parsedTelegram.payloadData = NULL;

                    khTelStat parseStat = khByteArrayToTelegram(&pDataEntry->data + 1, length, &parsedTelegram);

#ifdef KHOME_RF_SERIAL_GATEWAY
                    //transmit the telegram over serial if we use the gateway feature
                    khSerialTransmitTelegram(parsedTelegram);
#endif

                    //any other telegram parse status than CRCError or OK does not need an answer
                    if((parseStat == khTelStat_CRCError) && (parsedTelegram.receiverAddress == khTelegramGetDeviceAddress())){
                        khTelegram answerTelegram;
                        uint8_t answerTelegramData[2];

                        answerTelegram.telegramType = khTelType_ANS;
                        answerTelegram.senderAddress = khTelegramGetDeviceAddress();
                        answerTelegram.receiverAddress = 0xFF;
                        answerTelegram.payloadLength = 2;
                        answerTelegram.payloadData = &answerTelegramData[0];

                        answerTelegramData[0] = 0xFD;                   //CRC problem
                        answerTelegramData[1] = 0xFD;

                        khRFTransmitTelegram(answerTelegram);
#ifdef KHOME_RF_SERIAL_GATEWAY
                    //transmit the telegram over serial if we use the gateway feature
                        khSerialTransmitTelegram(answerTelegram);
#endif
                    }else if(parseStat == khTelStat_OK){
                        uint8_t answerTelegramIsNecessary = 0;

                        khTelegram answerTelegram;
                        uint8_t answerTelegramData[32];
                        answerTelegram.payloadData = &answerTelegramData[0];

                        khTelegramHandle(parsedTelegram, &answerTelegram, &answerTelegramIsNecessary);

                        if(answerTelegramIsNecessary){
                            khRFTransmitTelegram(answerTelegram);
#ifdef KHOME_RF_SERIAL_GATEWAY
                            //transmit the telegram over serial if we use the gateway feature
                            khSerialTransmitTelegram(answerTelegram);
#endif
                        }else{
                            //we need no answer telegram
                            //the restart of the receive mode gets handled by the khRFTransmitTelegram function
                            //since we are not calling it, we need to manually restart receive
                            khRFReceiveMode();
                        }
                    }

                    if(parsedTelegram.payloadData != NULL){
                        free(parsedTelegram.payloadData);
                    }
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
                //general error, do nothing
            }

            hasReceivedUnhandledTelegram = 0;
        }

    }
}

static void txCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e){
    txReady = 1;
    if(txPreviousRFMode == khRFMode_RX){
        //go back to rx if we were in it before
        currentRFMode = khRFMode_RX;
        khRFReceiveMode();
    }
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
    if(currentRFMode == khRFMode_NotOpened){
        return;                                             //the radio is not opened
    }

    txPreviousRFMode = currentRFMode;

    khRFReceiveModeStop();

    currentRFMode = khRFMode_TX;

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

    if(returnCodeRegister != khRegRet_OK){
        return;
    }

    uint8_t broadcastTelegramPayload[5];
    khTelegram broadcastTelegram;
    broadcastTelegram.telegramType = khTelType_REG_B;
    broadcastTelegram.receiverAddress = 0xFF;                           //receiver's address is alway 0xFF at broadcast
    broadcastTelegram.senderAddress = khTelegramGetDeviceAddress();     //get own address
    broadcastTelegram.payloadLength = 1 + registerLength;               //payload is as follows: |1x register address|1x-4x register data
    broadcastTelegram.payloadData = &broadcastTelegramPayload[0];

    broadcastTelegramPayload[0] = dataRegisterAddress;                  //byte 0 of payload is the register's address

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
    //invalidate handle
    rxCmdHandle = -1;

    if(e & RF_EventLastCmdDone){
        //wait for the previous telegram to be handle
        //while(hasReceivedUnhandledTelegram);
        hasReceivedUnhandledTelegram = 1;
        //restart receiving and handle the data, send response packet, post the event
        Event_post(eventPacketReceivedHandle, 1);
    }else if((e == RF_EventCmdAborted) || (e == RF_EventCmdStopped)){
        //user aborted, nothing else is necessary
    }
}

uint8_t khRFTmpIsInReceiveMode(void){
    return (rxCmdHandle >= 0) ? 1:0;
}

void khRFReceiveModeStop(void){
    if(currentRFMode == khRFMode_NotOpened){
        return;
    }

    if(rxCmdHandle >= 0){
        //only abort the command if it is still running
        RF_cancelCmd(rfHandle, rxCmdHandle, 0);
    }
    rxCmdHandle = -1;
    currentRFMode = khRFMode_Idle;
}

void khRFReceiveMode(void){
    if(currentRFMode == khRFMode_NotOpened){
        return;
    }

    //I suppose, we can just start receiving
    //The tx command will be as long in the queue as the data isn't transmitted, this shouldn't be to long
    //After that, the radio should just start receiving
    /*if(currentRFMode == khRFMode_TX){

    }*/

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
