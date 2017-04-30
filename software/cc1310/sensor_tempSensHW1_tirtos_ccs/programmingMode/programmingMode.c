/**
 * @file programmingMode.h
 * @author Peter Kappelt
 * @date 29.4.2017
 */

#include <unistd.h>

#include <ti/drivers/GPIO.h>
#include "../Board.h"
#include "../kHome/khRF.h"

uint8_t programmingModeEnabled = 0;

void programmingModeSetLed(uint8_t state){
    GPIO_write(Board_GPIO_PRGLED, state ? 1:0);
}

void programmingModeButtonPressed(uint_least8_t index){
    programmingModeEnabled = !programmingModeEnabled;

    if(programmingModeEnabled){
        khRFReceiveMode();
    }else{
        khRFReceiveModeStop();
    }

    GPIO_write(Board_GPIO_PRGLED, programmingModeEnabled ? 1:0);
}

void programmingModeInit(void){
    GPIO_init();
    GPIO_setCallback(Board_GPIO_PRGBTN, programmingModeButtonPressed);
    GPIO_write(Board_GPIO_PRGLED, 0);
}
