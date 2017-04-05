/**
 * @file khSerial.c
 * @author Peter Kappelt
 * @date 3.4.2017
 *
 * kHome Serial Media functions
 */

#include "khSerial.h"
#include "khTelegram.h"

#include <Board.h>
#include <ti/drivers/UART.h>

static UART_Handle uartHandle;
static UART_Params uartParams;

static uint8_t txBuffer[32];            //the commands of this device won't be longer than 20 bytes

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
    uartParams.baudRate = khSerial_Baudrate;

    uartHandle = UART_open(Board_UART0, &uartParams);
}

void khSerialTransmitTelegram(khTelegram telegram){
    txBuffer[0] = 0xAA;                             //khSerial start

    //convert the nice telegramm to a stream of bytes for sending
    uint8_t broadcastTelegramBytesLength = khTelegramToByteArray(telegram, &txBuffer[1]);

    txBuffer[broadcastTelegramBytesLength + 1] = '\r';
    txBuffer[broadcastTelegramBytesLength + 2] = '\n';

    UART_write(uartHandle, &txBuffer[0], broadcastTelegramBytesLength + 3);
}
