/**
 * @file khSerial.c
 * @author Peter Kappelt
 * @date 3.4.2017
 *
 * kHome Serial Media functions
 */

/**
 * Version History:
 * • V1.1 (20.4.2017)
 *  - added this version history
 * • V1.0 (19.4.2017)
 *  - initial revision
 */

#include "khSerial.h"
#include "khTelegram.h"
#include "khRF.h"

#include <Board.h>
#include <ti/drivers/UART.h>

#include <stdlib.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>

static UART_Handle uartHandle;
static UART_Params uartParams;

static uint8_t txBuffer[32];            //the commands of this device won't be longer than 20 bytes

#define KHSERIALRECEIVE_TASK_STACK_SIZE 256
#define KHSERIALRECEIVE_TASK_PRIORITY 1

static Task_Params khSerialReceivedTaskParams;
static Task_Struct khSerialReceivedTask;
static uint8_t khSerialReceivedTaskStack[KHSERIALRECEIVE_TASK_STACK_SIZE];

static Event_Struct eventByteReceived;
static Event_Handle eventByteReceivedHandle;

static void khSerialReceivedTaskFunction(UArg arg0, UArg arg1);

static uint8_t rxBuffer[64];
uint8_t rxBufferIndex = 0;

void uartReadCallback(UART_Handle handle, void *buf, size_t count){
    Event_post(eventByteReceivedHandle, 1);
}

/**
 * @brief init the khSerial functions
 */
void khSerialInit(void){
    UART_init();

    UART_Params_init(&uartParams);

    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.readMode = UART_MODE_CALLBACK;
    uartParams.readCallback = uartReadCallback;
    uartParams.baudRate = khSerial_Baudrate;

    uartHandle = UART_open(Board_UART0, &uartParams);


    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&eventByteReceived, &eventParam);
    eventByteReceivedHandle = Event_handle(&eventByteReceived);

    Task_Params_init(&khSerialReceivedTaskParams);
    khSerialReceivedTaskParams.stackSize = KHSERIALRECEIVE_TASK_STACK_SIZE;
    khSerialReceivedTaskParams.priority = KHSERIALRECEIVE_TASK_PRIORITY;
    khSerialReceivedTaskParams.stack = &khSerialReceivedTaskStack;
    Task_construct(&khSerialReceivedTask, khSerialReceivedTaskFunction, &khSerialReceivedTaskParams, NULL);

    //start receiving one byte
    //once it is received, we will redo that
    UART_read(uartHandle, rxBuffer, 1);
}

static void khSerialReceivedTaskFunction(UArg arg0, UArg arg1){
    while(1){
        uint32_t events = Event_pend(eventByteReceivedHandle, 0, 0xFFFFFFFF, BIOS_WAIT_FOREVER);
        if(events & 1){
            //if the packet is ready and the last digit was a new line
            if(rxBuffer[rxBufferIndex] == '\n'){
                //a valid frame needs at least four bytes:
                //0xAA (start of frame) | one byte data | \r | \n
                if(rxBufferIndex > 2){
                   if((rxBuffer[0] == 0xAA) == (rxBuffer[rxBufferIndex - 1] == '\r')){
                       uint8_t length = rxBufferIndex - 2;

                       khTelegram parsedTelegram;
                       parsedTelegram.payloadData = NULL;

                       khTelStat parseStat = khByteArrayToTelegram(&rxBuffer[1], length, &parsedTelegram);

   #ifdef KHOME_RF_SERIAL_GATEWAY
                       //transmit the telegram over serial if we use the gateway feature
                       khRFTransmitTelegram(parsedTelegram);
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

                           khSerialTransmitTelegram(answerTelegram);
   #ifdef KHOME_RF_SERIAL_GATEWAY
                           //transmit the telegram over serial if we use the gateway feature
                           khRFTransmitTelegram(answerTelegram);
   #endif
                       }else if(parseStat == khTelStat_OK){
                           uint8_t answerTelegramIsNecessary;

                           khTelegram answerTelegram;
                           uint8_t answerTelegramData[32];
                           answerTelegram.payloadData = &answerTelegramData[0];

                           khTelegramHandle(parsedTelegram, &answerTelegram, &answerTelegramIsNecessary);

                           if(answerTelegramIsNecessary){
                               khSerialTransmitTelegram(answerTelegram);
   #ifdef KHOME_RF_SERIAL_GATEWAY
                               //transmit the telegram over serial if we use the gateway feature
                               khRFTransmitTelegram(answerTelegram);
   #endif
                           }
                       }

                       if(parsedTelegram.payloadData != NULL){
                           free(parsedTelegram.payloadData);
                       }
                   }
                }

                rxBufferIndex = 0;          //reset the index
                UART_read(uartHandle, &rxBuffer[0], 1);
                continue;
            }

            UART_read(uartHandle, &rxBuffer[++rxBufferIndex], 1);
            //todo buffer overflow
        }
    }
}

void khSerialTransmitTelegram(khTelegram telegram){
    txBuffer[0] = 0xAA;                             //khSerial start

    //convert the nice telegramm to a stream of bytes for sending
    uint8_t broadcastTelegramBytesLength = khTelegramToByteArray(telegram, &txBuffer[1]);

    txBuffer[broadcastTelegramBytesLength + 1] = '\r';
    txBuffer[broadcastTelegramBytesLength + 2] = '\n';

    UART_write(uartHandle, &txBuffer[0], broadcastTelegramBytesLength + 3);
}
