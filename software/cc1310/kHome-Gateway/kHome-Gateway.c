/**
 * @author Peter Kappelt
 *
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>

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
#include "kHome/khSerial.h"

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
    khTelegramInit();
    khRFInit();
    khSerialInit();

    AONBatMonEnable();
    while(!AONBatMonNewBatteryMeasureReady());
    uint32_t testTemp = AONBatMonBatteryVoltageGet();

    //khRFBroadcastDataRegister(0x02);

    khRFReceiveMode();

    while (1) {
        sleep(time);
        //GPIO_toggle(Board_GPIO_LED0);

        if(!GPIO_read(Board_GPIO_BTN1)){
            khRFBroadcastDataRegister(0x02);
        }

        //khRFBroadcastDataRegister(0x02);

        //printf("Hello World!\r\n");
    }
}
