/**
 * @author Peter Kappelt
 *
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/rf/RF.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>

/* Board Header file */
#include "Board.h"

#include "kHome/khRegister.h"
#include "kHome/khTelegram.h"
#include "kHome/khRF.h"

#include DEVICE_FAMILY_PATH(driverlib/rf_data_entry.h)
#include DEVICE_FAMILY_PATH(driverlib/rf_prop_mailbox.h)

#include DEVICE_FAMILY_PATH(driverlib/aon_batmon.h)

#include "kHome/smartRF/smartrf_settings.h"
#include "kHome/khRF.h"

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    /* 1 second delay */
    uint32_t time = 1;

    /* Call driver init functions */
    GPIO_init();
    // I2C_init();
    // SDSPI_init();
    // SPI_init();
    // UART_init();
    // Watchdog_init();

    /* Turn on user LED */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);

    khRegisterInit();
    khRFInit();

    khTelegram test;
    uint8_t testData[3] = {0x01, 0x02, 0x03};

    uint8_t testOutput[261];

    test.telegramType = khTelType_REG_W;
    test.receiverAddress = 0x12;
    test.senderAddress = 0xaa;
    test.payloadLength = 3;
    test.payloadData = &testData[0];

    khTelegramToByteArray(test, &testOutput[0]);

    uint8_t testTelegram[9] = {1,1,5,18,3,1,2,3,0};
    khTelegram newTel;

    khByteArrayToTelegram(&testTelegram[0], 9, &newTel);

    AONBatMonEnable();
    while(!AONBatMonNewBatteryMeasureReady());
    uint32_t testTemp = AONBatMonBatteryVoltageGet();

    //khRFBroadcastDataRegister(0x02);

    khRFReceiveMode();

    while (1) {
        sleep(time);
        GPIO_toggle(Board_GPIO_LED0);
        //khRFBroadcastDataRegister(0x02);

        //printf("Hello World!\r\n");
    }
}
