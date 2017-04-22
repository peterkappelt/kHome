/**
 * Copyright 2017, Peter Kappelt
 * Adjusted this file, which is originally from the CC1310-LaunchXL-Board, for the kHome temperatureSensor board, Hardware V1
 */

/**
 * Version History:
 * • V1.1 (20.4.2017)
 *  - added this version history
 * • V1.0 (19.4.2017)
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
/** ============================================================================
 *  @file       tempSens_HWV1.h
 *
 *  @brief      kHome temperatureSensor Hardware V1 Board Specific header file.
 *
 *  The tempSens_HWV1 header file should be included in an application as
 *  follows:
 *  @code
 *  #include "tempSens_HWV1.h"
 *  @endcode
 *
 *  ============================================================================
 */
#ifndef __tempSens_HWV1_BOARD_H__
#define __tempSens_HWV1_BOARD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include <ti/drivers/PIN.h>
#include <ti/devices/cc13x0/driverlib/ioc.h>

/* Externs */
extern const PIN_Config BoardGpioInitTable[];

/* Defines */
#define tempSens_HWV1

/* Mapping of pins to board signals using general board aliases
 *      <board signal alias>                  <pin mapping>   <comments>
 */

/* Analog capable DIOs */
#define tempSens_HWV1_DIO23_ANALOG          IOID_23
#define tempSens_HWV1_DIO24_ANALOG          IOID_24
#define tempSens_HWV1_DIO25_ANALOG          IOID_25
#define tempSens_HWV1_DIO26_ANALOG          IOID_26
#define tempSens_HWV1_DIO27_ANALOG          IOID_27
#define tempSens_HWV1_DIO28_ANALOG          IOID_28
#define tempSens_HWV1_DIO29_ANALOG          IOID_29
#define tempSens_HWV1_DIO30_ANALOG          IOID_30

/* Digital IOs */
#define tempSens_HWV1_DIO0                  IOID_0
#define tempSens_HWV1_DIO1                  IOID_1
#define tempSens_HWV1_DIO12                 IOID_12
#define tempSens_HWV1_DIO15                 IOID_15
#define tempSens_HWV1_DIO16_TDO             IOID_16
#define tempSens_HWV1_DIO17_TDI             IOID_17
#define tempSens_HWV1_DIO21                 IOID_21
#define tempSens_HWV1_DIO22                 IOID_22

/* Discrete Inputs */
//#define tempSens_HWV1_PIN_BTN1              IOID_13
//#define tempSens_HWV1_PIN_BTN2              IOID_14
#define tempSens_HWV1_PIN_PRGBTN            IOID_12

/* GPIO */
//#define tempSens_HWV1_GPIO_LED_ON           1
//#define tempSens_HWV1_GPIO_LED_OFF          0

/* I2C */
#define tempSens_HWV1_I2C0_SCL0             IOID_8
#define tempSens_HWV1_I2C0_SDA0             IOID_9

/* LCD (430BOOST - Sharp96 Rev 1.1) */
#define tempSens_HWV1_LCD_CS                IOID_24  /* SPI chip select */
#define tempSens_HWV1_LCD_EXTCOMIN          IOID_12  /* External COM inversion */
#define tempSens_HWV1_LCD_ENABLE            IOID_22  /* LCD enable */
#define tempSens_HWV1_LCD_POWER             IOID_23  /* LCD power control */
#define tempSens_HWV1_LCD_CS_ON             1
#define tempSens_HWV1_LCD_CS_OFF            0

/* LEDs */
//#define tempSens_HWV1_PIN_LED_ON            1
//#define tempSens_HWV1_PIN_LED_OFF           0
//#define tempSens_HWV1_PIN_RLED              IOID_12
//#define tempSens_HWV1_PIN_GLED              IOID_7
#define tempSens_HWV1_PIN_PRGLED            IOID_12

/* PWM Outputs */
#define tempSens_HWV1_PWMPIN0               tempSens_HWV1_PIN_PRGLED
#define tempSens_HWV1_PWMPIN1               PIN_UNASSIGNED
#define tempSens_HWV1_PWMPIN2               PIN_UNASSIGNED
#define tempSens_HWV1_PWMPIN3               PIN_UNASSIGNED
#define tempSens_HWV1_PWMPIN4               PIN_UNASSIGNED
#define tempSens_HWV1_PWMPIN5               PIN_UNASSIGNED
#define tempSens_HWV1_PWMPIN6               PIN_UNASSIGNED
#define tempSens_HWV1_PWMPIN7               PIN_UNASSIGNED

/* SPI */
#define tempSens_HWV1_SPI_FLASH_CS          IOID_20
#define tempSens_HWV1_FLASH_CS_ON           0
#define tempSens_HWV1_FLASH_CS_OFF          1

/* SPI Board */
#define tempSens_HWV1_SPI0_MISO             PIN_UNASSIGNED
#define tempSens_HWV1_SPI0_MOSI             PIN_UNASSIGNED
#define tempSens_HWV1_SPI0_CLK              PIN_UNASSIGNED
#define tempSens_HWV1_SPI0_CSN              PIN_UNASSIGNED
#define tempSens_HWV1_SPI1_MISO             PIN_UNASSIGNED
#define tempSens_HWV1_SPI1_MOSI             PIN_UNASSIGNED
#define tempSens_HWV1_SPI1_CLK              PIN_UNASSIGNED
#define tempSens_HWV1_SPI1_CSN              PIN_UNASSIGNED

/* UART Board */
#define tempSens_HWV1_UART_RX               IOID_2          /* RXD */
#define tempSens_HWV1_UART_TX               IOID_3          /* TXD */
#define tempSens_HWV1_UART_CTS              IOID_19         /* CTS */
#define tempSens_HWV1_UART_RTS              IOID_18         /* RTS */

/*!
 *  @brief  Initialize the general board specific settings
 *
 *  This function initializes the general board specific settings.
 */
void tempSens_HWV1_initGeneral(void);

/*!
 *  @def    CC1310_LAUNCHXL_NVSBufName
 *  @brief  Enum of NVSBufs
 */
typedef enum tempSens_HWV1_NVSBufName {
    tempSens_HWV1_NVS1F000 = 0,
    tempSens_HWV1_NVSCOUNT
} tempSens_HWV1_NVSBufName;

/*!
 *  @def    tempSens_HWV1_ADCBufName
 *  @brief  Enum of ADCBufs
 */
typedef enum tempSens_HWV1_ADCBufName {
    tempSens_HWV1_ADCBUF0 = 0,

    tempSens_HWV1_ADCBUFCOUNT
} tempSens_HWV1_ADCBufName;

/*!
 *  @def    tempSens_HWV1_ADCBuf0SourceName
 *  @brief  Enum of ADCBuf channels
 */
typedef enum tempSens_HWV1_ADCBuf0ChannelName {
    tempSens_HWV1_ADCBUF0CHANNEL0 = 0,
    tempSens_HWV1_ADCBUF0CHANNEL1,
    tempSens_HWV1_ADCBUF0CHANNEL2,
    tempSens_HWV1_ADCBUF0CHANNEL3,
    tempSens_HWV1_ADCBUF0CHANNEL4,
    tempSens_HWV1_ADCBUF0CHANNEL5,
    tempSens_HWV1_ADCBUF0CHANNEL6,
    tempSens_HWV1_ADCBUF0CHANNEL7,
    tempSens_HWV1_ADCBUF0CHANNELVDDS,
    tempSens_HWV1_ADCBUF0CHANNELDCOUPL,
    tempSens_HWV1_ADCBUF0CHANNELVSS,

    tempSens_HWV1_ADCBUF0CHANNELCOUNT
} tempSens_HWV1_ADCBuf0ChannelName;

/*!
 *  @def    tempSens_HWV1_ADCName
 *  @brief  Enum of ADCs
 */
typedef enum tempSens_HWV1_ADCName {
    tempSens_HWV1_ADC0 = 0,
    tempSens_HWV1_ADC1,
    tempSens_HWV1_ADC2,
    tempSens_HWV1_ADC3,
    tempSens_HWV1_ADC4,
    tempSens_HWV1_ADC5,
    tempSens_HWV1_ADC6,
    tempSens_HWV1_ADC7,
    tempSens_HWV1_ADCDCOUPL,
    tempSens_HWV1_ADCVSS,
    tempSens_HWV1_ADCVDDS,

    tempSens_HWV1_ADCCOUNT
} tempSens_HWV1_ADCName;

/*!
 *  @def    tempSens_HWV1_CryptoName
 *  @brief  Enum of Crypto names
 */
typedef enum tempSens_HWV1_CryptoName {
    tempSens_HWV1_CRYPTO0 = 0,

    tempSens_HWV1_CRYPTOCOUNT
} tempSens_HWV1_CryptoName;

/*!
 *  @def    tempSens_HWV1_GPIOName
 *  @brief  Enum of GPIO names
 */
typedef enum tempSens_HWV1_GPIOName {
    tempSens_HWV1_GPIO_S1 = 0,
    tempSens_HWV1_GPIO_S2,
    tempSens_HWV1_GPIO_LED_GREEN,
    tempSens_HWV1_GPIO_LED_RED,

    tempSens_HWV1_GPIOCOUNT
} tempSens_HWV1_GPIOName;

/*!
 *  @def    tempSens_HWV1_GPTimerName
 *  @brief  Enum of GPTimer parts
 */
typedef enum tempSens_HWV1_GPTimerName {
    tempSens_HWV1_GPTIMER0A = 0,
    tempSens_HWV1_GPTIMER0B,
    tempSens_HWV1_GPTIMER1A,
    tempSens_HWV1_GPTIMER1B,
    tempSens_HWV1_GPTIMER2A,
    tempSens_HWV1_GPTIMER2B,
    tempSens_HWV1_GPTIMER3A,
    tempSens_HWV1_GPTIMER3B,

    tempSens_HWV1_GPTIMERPARTSCOUNT
} tempSens_HWV1_GPTimerName;

/*!
 *  @def    tempSens_HWV1_GPTimers
 *  @brief  Enum of GPTimers
 */
typedef enum tempSens_HWV1_GPTimers {
    tempSens_HWV1_GPTIMER0 = 0,
    tempSens_HWV1_GPTIMER1,
    tempSens_HWV1_GPTIMER2,
    tempSens_HWV1_GPTIMER3,

    tempSens_HWV1_GPTIMERCOUNT
} tempSens_HWV1_GPTimers;

/*!
 *  @def    tempSens_HWV1_I2CName
 *  @brief  Enum of I2C names
 */
typedef enum tempSens_HWV1_I2CName {
    tempSens_HWV1_I2C0 = 0,

    tempSens_HWV1_I2CCOUNT
} tempSens_HWV1_I2CName;

/*!
 *  @def    tempSens_HWV1_PWM
 *  @brief  Enum of PWM outputs
 */
typedef enum tempSens_HWV1_PWMName {
    tempSens_HWV1_PWM0 = 0,
    tempSens_HWV1_PWM1,
    tempSens_HWV1_PWM2,
    tempSens_HWV1_PWM3,
    tempSens_HWV1_PWM4,
    tempSens_HWV1_PWM5,
    tempSens_HWV1_PWM6,
    tempSens_HWV1_PWM7,

    tempSens_HWV1_PWMCOUNT
} tempSens_HWV1_PWMName;

/*!
 *  @def    tempSens_HWV1_SPIName
 *  @brief  Enum of SPI names
 */
typedef enum tempSens_HWV1_SPIName {
    tempSens_HWV1_SPI0 = 0,
    tempSens_HWV1_SPI1,

    tempSens_HWV1_SPICOUNT
} tempSens_HWV1_SPIName;

/*!
 *  @def    tempSens_HWV1_UARTName
 *  @brief  Enum of UARTs
 */
typedef enum tempSens_HWV1_UARTName {
    tempSens_HWV1_UART0 = 0,

    tempSens_HWV1_UARTCOUNT
} tempSens_HWV1_UARTName;

/*!
 *  @def    tempSens_HWV1_UDMAName
 *  @brief  Enum of DMA buffers
 */
typedef enum tempSens_HWV1_UDMAName {
    tempSens_HWV1_UDMA0 = 0,

    tempSens_HWV1_UDMACOUNT
} tempSens_HWV1_UDMAName;

/*!
 *  @def    tempSens_HWV1_WatchdogName
 *  @brief  Enum of Watchdogs
 */
typedef enum tempSens_HWV1_WatchdogName {
    tempSens_HWV1_WATCHDOG0 = 0,

    tempSens_HWV1_WATCHDOGCOUNT
} tempSens_HWV1_WatchdogName;


#ifdef __cplusplus
}
#endif

#endif /* __tempSens_HWV1_BOARD_H__ */
