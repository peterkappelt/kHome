/**
 * Copyright 2017, Peter Kappelt
 * Adjusted this file, which is originally from the CC1310-LaunchXL-Board, for the kHome temperatureSensor board, Hardware V1
 */

/**
 * Version History:
 * � V1.1 (20.4.2017)
 *  - added this version history
 * � V1.0 (19.4.2017)
 *  - initial revision
 */

/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/drivers/ADC.h>
#include <ti/drivers/ADCBuf.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Watchdog.h>

#include "tempSens_HWV1.h"

#define Board_initGeneral()     tempSens_HWV1_initGeneral()

/* These #defines allow us to reuse TI-RTOS across other device families */

#define Board_ADC0              tempSens_HWV1_ADC0
#define Board_ADC1              tempSens_HWV1_ADC1

#define Board_ADCBUF0           tempSens_HWV1_ADCBUF0
#define Board_ADCBUF0CHANNEL0   tempSens_HWV1_ADCBUF0CHANNEL0
#define Board_ADCBUF0CHANNEL1   tempSens_HWV1_ADCBUF0CHANNEL1

#define Board_CRYPTO0           tempSens_HWV1_CRYPTO0

#define Board_DIO0              tempSens_HWV1_DIO0
#define Board_DIO1              tempSens_HWV1_DIO1
#define Board_DIO12             tempSens_HWV1_DIO12
#define Board_DIO15             tempSens_HWV1_DIO15
#define Board_DIO16_TDO         tempSens_HWV1_DIO16_TDO
#define Board_DIO17_TDI         tempSens_HWV1_DIO17_TDI
#define Board_DIO21             tempSens_HWV1_DIO21
#define Board_DIO22             tempSens_HWV1_DIO22

//#define Board_GPIO_BUTTON0      tempSens_HWV1_GPIO_S1
//#define Board_GPIO_BUTTON1      tempSens_HWV1_GPIO_S2
//#define Board_GPIO_BTN1         tempSens_HWV1_GPIO_S1
//#define Board_GPIO_BTN2         tempSens_HWV1_GPIO_S2
//#define Board_GPIO_LED0         tempSens_HWV1_GPIO_LED_RED
//#define Board_GPIO_LED1         tempSens_HWV1_GPIO_LED_GREEN
//#define Board_GPIO_RLED         tempSens_HWV1_GPIO_LED_RED
//#define Board_GPIO_GLED         tempSens_HWV1_GPIO_LED_GREEN
//#define Board_GPIO_LED_ON       tempSens_HWV1_GPIO_LED_ON
//#define Board_GPIO_LED_OFF      tempSens_HWV1_GPIO_LED_OFF
#define Board_GPIO_PRGLED       tempSens_HWV1_GPIO_PRGLED
#define Board_GPIO_PRGBTN       tempSens_HWV1_GPIO_PRGBTN

#define Board_GPTIMER0A         tempSens_HWV1_GPTIMER0A
#define Board_GPTIMER0B         tempSens_HWV1_GPTIMER0B
#define Board_GPTIMER1A         tempSens_HWV1_GPTIMER1A
#define Board_GPTIMER1B         tempSens_HWV1_GPTIMER1B
#define Board_GPTIMER2A         tempSens_HWV1_GPTIMER2A
#define Board_GPTIMER2B         tempSens_HWV1_GPTIMER2B
#define Board_GPTIMER3A         tempSens_HWV1_GPTIMER3A
#define Board_GPTIMER3B         tempSens_HWV1_GPTIMER3B

#define Board_I2C0              tempSens_HWV1_I2C0
#define Board_I2C_TMP           tempSens_HWV1_I2C0

//#define Board_PIN_BUTTON0       tempSens_HWV1_PIN_BTN1
//#define Board_PIN_BUTTON1       tempSens_HWV1_PIN_BTN2
//#define Board_PIN_BTN1          tempSens_HWV1_PIN_BTN1
//#define Board_PIN_BTN2          tempSens_HWV1_PIN_BTN2
//#define Board_PIN_LED0          tempSens_HWV1_PIN_RLED
//#define Board_PIN_LED1          tempSens_HWV1_PIN_GLED
//#define Board_PIN_LED2          tempSens_HWV1_PIN_RLED
//#define Board_PIN_RLED          tempSens_HWV1_PIN_RLED
//#define Board_PIN_GLED          tempSens_HWV1_PIN_GLED
#define Board_PIN_PRGBTN        tempSens_HWV1_PIN_PRGBTN
#define Board_PIN_PRGLED        tempSens_HWV1_PIN_PRGLED

#define Board_PWM0              tempSens_HWV1_PWM0
#define Board_PWM1              tempSens_HWV1_PWM1
#define Board_PWM2              tempSens_HWV1_PWM2
#define Board_PWM3              tempSens_HWV1_PWM3
#define Board_PWM4              tempSens_HWV1_PWM4
#define Board_PWM5              tempSens_HWV1_PWM5
#define Board_PWM6              tempSens_HWV1_PWM6
#define Board_PWM7              tempSens_HWV1_PWM7

#define Board_SPI0              tempSens_HWV1_SPI0
#define Board_SPI1              tempSens_HWV1_SPI1
#define Board_SPI_FLASH_CS      tempSens_HWV1_SPI_FLASH_CS
#define Board_FLASH_CS_ON       0
#define Board_FLASH_CS_OFF      1

#define Board_UART0             tempSens_HWV1_UART0

#define Board_WATCHDOG0         tempSens_HWV1_WATCHDOG0

/* Board specific I2C addresses */
#define Board_TMP_ADDR          (0x40)
#define Board_SENSORS_BP_TMP_ADDR Board_TMP_ADDR

/*
 * These macros are provided for backwards compatibility.
 * Please use the <Driver>_init functions directly rather
 * than Board_init<Driver>.
 */
#define Board_initADC()         ADC_init()
#define Board_initADCBuf()      ADCBuf_init()
#define Board_initGPIO()        GPIO_init()
#define Board_initPWM()         PWM_init()
#define Board_initSPI()         SPI_init()
#define Board_initUART()        UART_init()
#define Board_initWatchdog()    Watchdog_init()

/*
 * These macros are provided for backwards compatibility.
 * Please use the 'Board_PIN_xxx' macros to differentiate
 * them from the 'Board_GPIO_xxx' macros.
 */
#define Board_BUTTON0           Board_PIN_BUTTON0
#define Board_BUTTON1           Board_PIN_BUTTON1
//#define Board_BTN1              Board_PIN_BTN1
//#define Board_BTN2              Board_PIN_BTN2
//#define Board_LED0              Board_PIN_LED0
//#define Board_LED1              Board_PIN_LED1
//#define Board_LED2              Board_PIN_LED2
//#define Board_RLED              Board_PIN_RLED
//#define Board_GLED              Board_PIN_GLED
//#define Board_LED_ON            Board_GPIO_LED_ON
//#define Board_LED_OFF           Board_GPIO_LED_OFF
#define Board_ADCBUFCHANNEL0    Board_ADCBUF0CHANNEL0
#define Board_ADCBUFCHANNEL1    Board_ADCBUF0CHANNEL1

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H */