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

/*
 *  ====================== tempSens_HWV1.c ===================================
 *  This file is responsible for setting up the board specific items for the
 *  tempSens_HWV1 board.
 */

#include <tempSens_HWV1.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>

#include <ti/devices/cc13x0/driverlib/ioc.h>
#include <ti/devices/cc13x0/driverlib/udma.h>
#include <ti/devices/cc13x0/inc/hw_ints.h>
#include <ti/devices/cc13x0/inc/hw_memmap.h>

#include <ti/drivers/ADCBuf.h>
#include <ti/drivers/adcbuf/ADCBufCC26XX.h>

/*
 *  ========================== NVS begin =========================================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(NVS_config, ".const:NVS_config")
#pragma DATA_SECTION(nvsCC26xxHWAttrs, ".const:nvsCC26xxHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/NVS.h>
#include <ti/drivers/nvs/NVSCC26XX.h>


/* NVS objects */
NVSCC26XX_Object nvsCC26xxObjects[tempSens_HWV1_NVSCOUNT];
/*
 * Block to be used if making a copy of the flash page is needed.
 * Could use another block of flash, but there is not alot of flash on
 * these devices...
 */
char myCopyBlock[4096];
const NVSCC26XX_HWAttrs nvsCC26xxHWAttrs[tempSens_HWV1_NVSCOUNT] = {
{
        .block = (void *)(0x10000 - 4096), // Flash sector to use is top 4096 of flash on a 64k part
        .blockSize = 4096,
        .copyBlock = myCopyBlock,
        .isRam = true
    }
};

const uint8_t NVS_count = 1;

const NVS_Config NVS_config[] = {
    {
        .fxnTablePtr = &NVSCC26XX_fxnTable,
        .object      = &nvsCC26xxObjects[0],
        .hwAttrs     = &nvsCC26xxHWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ========================== NVS end =========================================
 */

ADCBufCC26XX_Object adcBufCC26xxObjects[tempSens_HWV1_ADCBUFCOUNT];

/*
 *  This table converts a virtual adc channel into a dio and internal analogue
 *  input signal. This table is necessary for the functioning of the adcBuf
 *  driver. Comment out unused entries to save flash. Dio and internal signal
 *  pairs are hardwired. Do not remap them in the table. You may reorder entire
 *  entries. The mapping of dio and internal signals is package dependent.
 */
const ADCBufCC26XX_AdcChannelLutEntry ADCBufCC26XX_adcChannelLut[tempSens_HWV1_ADCBUF0CHANNELCOUNT] = {
    {tempSens_HWV1_DIO23_ANALOG, ADC_COMPB_IN_AUXIO7},
    {tempSens_HWV1_DIO24_ANALOG, ADC_COMPB_IN_AUXIO6},
    {tempSens_HWV1_DIO25_ANALOG, ADC_COMPB_IN_AUXIO5},
    {tempSens_HWV1_DIO26_ANALOG, ADC_COMPB_IN_AUXIO4},
    {tempSens_HWV1_DIO27_ANALOG, ADC_COMPB_IN_AUXIO3},
    {tempSens_HWV1_DIO28_ANALOG, ADC_COMPB_IN_AUXIO2},
    {tempSens_HWV1_DIO29_ANALOG, ADC_COMPB_IN_AUXIO1},
    {tempSens_HWV1_DIO30_ANALOG, ADC_COMPB_IN_AUXIO0},
    {PIN_UNASSIGNED, ADC_COMPB_IN_VDDS},
    {PIN_UNASSIGNED, ADC_COMPB_IN_DCOUPL},
    {PIN_UNASSIGNED, ADC_COMPB_IN_VSS},
};

const ADCBufCC26XX_HWAttrs adcBufCC26xxHWAttrs[tempSens_HWV1_ADCBUFCOUNT] = {
    {
        .intPriority       = ~0,
        .swiPriority       = 0,
        .adcChannelLut     = ADCBufCC26XX_adcChannelLut,
        .gpTimerUnit       = tempSens_HWV1_GPTIMER0A,
        .gptDMAChannelMask = 1 << UDMA_CHAN_TIMER0_A,
    }
};

const ADCBuf_Config ADCBuf_config[tempSens_HWV1_ADCBUFCOUNT] = {
    {
        &ADCBufCC26XX_fxnTable,
        &adcBufCC26xxObjects[tempSens_HWV1_ADCBUF0],
        &adcBufCC26xxHWAttrs[tempSens_HWV1_ADCBUF0]
    },
};

const uint_least8_t ADCBuf_count = tempSens_HWV1_ADCBUFCOUNT;

/*
 *  =============================== ADC ===============================
 */
#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCCC26XX.h>

ADCCC26XX_Object adcCC26xxObjects[tempSens_HWV1_ADCCOUNT];


const ADCCC26XX_HWAttrs adcCC26xxHWAttrs[tempSens_HWV1_ADCCOUNT] = {
    {
        .adcDIO              = tempSens_HWV1_DIO23_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO7,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = tempSens_HWV1_DIO24_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO6,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = tempSens_HWV1_DIO25_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO5,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = tempSens_HWV1_DIO26_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO4,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = tempSens_HWV1_DIO27_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO3,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = tempSens_HWV1_DIO28_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO2,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = tempSens_HWV1_DIO29_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO1,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = tempSens_HWV1_DIO30_ANALOG,
        .adcCompBInput       = ADC_COMPB_IN_AUXIO0,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_10P9_MS,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_DCOUPL,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_VSS,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    },
    {
        .adcDIO              = PIN_UNASSIGNED,
        .adcCompBInput       = ADC_COMPB_IN_VDDS,
        .refSource           = ADCCC26XX_FIXED_REFERENCE,
        .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
        .inputScalingEnabled = true,
        .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
        .returnAdjustedVal   = 0
    }
};

const ADC_Config ADC_config[tempSens_HWV1_ADCCOUNT] = {
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[tempSens_HWV1_ADC0], &adcCC26xxHWAttrs[tempSens_HWV1_ADC0]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[tempSens_HWV1_ADC1], &adcCC26xxHWAttrs[tempSens_HWV1_ADC1]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[tempSens_HWV1_ADC2], &adcCC26xxHWAttrs[tempSens_HWV1_ADC2]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[tempSens_HWV1_ADC3], &adcCC26xxHWAttrs[tempSens_HWV1_ADC3]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[tempSens_HWV1_ADC4], &adcCC26xxHWAttrs[tempSens_HWV1_ADC4]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[tempSens_HWV1_ADC5], &adcCC26xxHWAttrs[tempSens_HWV1_ADC5]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[tempSens_HWV1_ADC6], &adcCC26xxHWAttrs[tempSens_HWV1_ADC6]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[tempSens_HWV1_ADC7], &adcCC26xxHWAttrs[tempSens_HWV1_ADC7]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[tempSens_HWV1_ADCDCOUPL], &adcCC26xxHWAttrs[tempSens_HWV1_ADCDCOUPL]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[tempSens_HWV1_ADCVSS], &adcCC26xxHWAttrs[tempSens_HWV1_ADCVSS]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[tempSens_HWV1_ADCVDDS], &adcCC26xxHWAttrs[tempSens_HWV1_ADCVDDS]},
};

const uint_least8_t ADC_count = tempSens_HWV1_ADCCOUNT;

/*
 *  =============================== Crypto ===============================
 */
#include <ti/drivers/crypto/CryptoCC26XX.h>

CryptoCC26XX_Object cryptoCC26XXObjects[tempSens_HWV1_CRYPTOCOUNT];

const CryptoCC26XX_HWAttrs cryptoCC26XXHWAttrs[tempSens_HWV1_CRYPTOCOUNT] = {
    {
        .baseAddr       = CRYPTO_BASE,
        .powerMngrId    = PowerCC26XX_PERIPH_CRYPTO,
        .intNum         = INT_CRYPTO_RESULT_AVAIL_IRQ,
        .intPriority    = ~0,
    }
};

const CryptoCC26XX_Config CryptoCC26XX_config[tempSens_HWV1_CRYPTOCOUNT] = {
    {
         .object  = &cryptoCC26XXObjects[tempSens_HWV1_CRYPTO0],
         .hwAttrs = &cryptoCC26XXHWAttrs[tempSens_HWV1_CRYPTO0]
    }
};

/*
 *  =============================== Display ===============================
 */
#include <ti/display/Display.h>
#include <ti/display/DisplayUart.h>
#include <ti/display/DisplaySharp.h>

#ifndef BOARD_DISPLAY_UART_STRBUF_SIZE
#define BOARD_DISPLAY_UART_STRBUF_SIZE    128
#endif

#ifndef BOARD_DISPLAY_SHARP_SIZE
#define BOARD_DISPLAY_SHARP_SIZE    96
#endif

DisplayUart_Object     displayUartObject;
DisplaySharp_Object    displaySharpObject;

static char uartStringBuf[BOARD_DISPLAY_UART_STRBUF_SIZE];
static uint_least8_t sharpDisplayBuf[BOARD_DISPLAY_SHARP_SIZE * BOARD_DISPLAY_SHARP_SIZE / 8];

const DisplayUart_HWAttrs displayUartHWAttrs = {
    .uartIdx      = tempSens_HWV1_UART0,
    .baudRate     = 115200,
    .mutexTimeout = (unsigned int)(-1),
    .strBuf       = uartStringBuf,
    .strBufLen    = BOARD_DISPLAY_UART_STRBUF_SIZE,
};

const DisplaySharp_HWAttrs displaySharpHWattrs = {
    .spiIndex    = tempSens_HWV1_SPI0,
    .csPin       = tempSens_HWV1_LCD_CS,
    .extcominPin = tempSens_HWV1_LCD_EXTCOMIN,
    .powerPin    = tempSens_HWV1_LCD_POWER,
    .enablePin   = tempSens_HWV1_LCD_ENABLE,
    .pixelWidth  = BOARD_DISPLAY_SHARP_SIZE,
    .pixelHeight = BOARD_DISPLAY_SHARP_SIZE,
    .displayBuf  = sharpDisplayBuf,
};

#ifndef BOARD_DISPLAY_USE_UART
#define BOARD_DISPLAY_USE_UART 1
#endif
#ifndef BOARD_DISPLAY_USE_UART_ANSI
#define BOARD_DISPLAY_USE_UART_ANSI 0
#endif
#ifndef BOARD_DISPLAY_USE_LCD
#define BOARD_DISPLAY_USE_LCD 0
#endif

/*
 * This #if/#else is needed to workaround a problem with the
 * IAR compiler. The IAR compiler doesn't like the empty array
 * initialization. (IAR Error[Pe1345])
 */
#if (BOARD_DISPLAY_USE_UART || BOARD_DISPLAY_USE_LCD)

const Display_Config Display_config[] = {
#if (BOARD_DISPLAY_USE_UART)
    {
#  if (BOARD_DISPLAY_USE_UART_ANSI)
        .fxnTablePtr = &DisplayUartAnsi_fxnTable,
#  else /* Default to minimal UART with no cursor placement */
        .fxnTablePtr = &DisplayUartMin_fxnTable,
#  endif
        .object      = &displayUartObject,
        .hwAttrs     = &displayUartHWAttrs,
    },
#endif
#if (BOARD_DISPLAY_USE_LCD)
    {
        .fxnTablePtr = &DisplaySharp_fxnTable,
        .object      = &displaySharpObject,
        .hwAttrs     = &displaySharpHWattrs
    },
#endif
};

const uint_least8_t Display_count = sizeof(Display_config) / sizeof(Display_Config);

#else

const Display_Config *Display_config = NULL;
const uint_least8_t Display_count = 0;

#endif /* (BOARD_DISPLAY_USE_UART || BOARD_DISPLAY_USE_LCD) */

/*
 *  =============================== GPIO ===============================
 */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

/*
 * Array of Pin configurations
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in tempSens_HWV1.h
 * NOTE: Pins not used for interrupts should be placed at the end of the
 *       array. Callback entries can be omitted from callbacks array to
 *       reduce memory usage.
 */
GPIO_PinConfig gpioPinConfigs[] = {
    /* Input pins */
    GPIOCC26XX_DIO_11 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING,  /* Programming button */

    /* Output pins */
    GPIOCC26XX_DIO_12 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_HIGH,  /* Programming mode indicator LED */
};

/*
 * Array of callback function pointers
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in CC1310_LAUNCH.h
 * NOTE: Pins not used for interrupts can be omitted from callbacks array to
 *       reduce memory usage (if placed at end of gpioPinConfigs array).
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    NULL,  /* Programming button */
};

const GPIOCC26XX_Config GPIOCC26XX_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = tempSens_HWV1_GPIOCOUNT,
    .numberOfCallbacks  = sizeof(gpioCallbackFunctions)/sizeof(GPIO_CallbackFxn),
    .intPriority = (~0)
};

/*
 *  =============================== GPTimer ===============================
 *  Remove unused entries to reduce flash usage both in Board.c and Board.h
 */
#include <ti/drivers/timer/GPTimerCC26XX.h>

GPTimerCC26XX_Object gptimerCC26XXObjects[tempSens_HWV1_GPTIMERCOUNT];

const GPTimerCC26XX_HWAttrs gptimerCC26xxHWAttrs[tempSens_HWV1_GPTIMERPARTSCOUNT] = {
    { .baseAddr = GPT0_BASE, .intNum = INT_GPT0A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT0, .pinMux = GPT_PIN_0A, },
    { .baseAddr = GPT0_BASE, .intNum = INT_GPT0B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT0, .pinMux = GPT_PIN_0B, },
    { .baseAddr = GPT1_BASE, .intNum = INT_GPT1A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT1, .pinMux = GPT_PIN_1A, },
    { .baseAddr = GPT1_BASE, .intNum = INT_GPT1B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT1, .pinMux = GPT_PIN_1B, },
    { .baseAddr = GPT2_BASE, .intNum = INT_GPT2A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT2, .pinMux = GPT_PIN_2A, },
    { .baseAddr = GPT2_BASE, .intNum = INT_GPT2B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT2, .pinMux = GPT_PIN_2B, },
    { .baseAddr = GPT3_BASE, .intNum = INT_GPT3A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT3, .pinMux = GPT_PIN_3A, },
    { .baseAddr = GPT3_BASE, .intNum = INT_GPT3B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT3, .pinMux = GPT_PIN_3B, },
};

const GPTimerCC26XX_Config GPTimerCC26XX_config[tempSens_HWV1_GPTIMERPARTSCOUNT] = {
    { &gptimerCC26XXObjects[tempSens_HWV1_GPTIMER0], &gptimerCC26xxHWAttrs[tempSens_HWV1_GPTIMER0A], GPT_A },
    { &gptimerCC26XXObjects[tempSens_HWV1_GPTIMER0], &gptimerCC26xxHWAttrs[tempSens_HWV1_GPTIMER0B], GPT_B },
    { &gptimerCC26XXObjects[tempSens_HWV1_GPTIMER1], &gptimerCC26xxHWAttrs[tempSens_HWV1_GPTIMER1A], GPT_A },
    { &gptimerCC26XXObjects[tempSens_HWV1_GPTIMER1], &gptimerCC26xxHWAttrs[tempSens_HWV1_GPTIMER1B], GPT_B },
    { &gptimerCC26XXObjects[tempSens_HWV1_GPTIMER2], &gptimerCC26xxHWAttrs[tempSens_HWV1_GPTIMER2A], GPT_A },
    { &gptimerCC26XXObjects[tempSens_HWV1_GPTIMER2], &gptimerCC26xxHWAttrs[tempSens_HWV1_GPTIMER2B], GPT_B },
    { &gptimerCC26XXObjects[tempSens_HWV1_GPTIMER3], &gptimerCC26xxHWAttrs[tempSens_HWV1_GPTIMER3A], GPT_A },
    { &gptimerCC26XXObjects[tempSens_HWV1_GPTIMER3], &gptimerCC26xxHWAttrs[tempSens_HWV1_GPTIMER3B], GPT_B },
};

/*
 *  =============================== I2C ===============================
*/
#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CCC26XX.h>

I2CCC26XX_Object i2cCC26xxObjects[tempSens_HWV1_I2CCOUNT];

const I2CCC26XX_HWAttrsV1 i2cCC26xxHWAttrs[tempSens_HWV1_I2CCOUNT] = {
    {
        .baseAddr    = I2C0_BASE,
        .powerMngrId = PowerCC26XX_PERIPH_I2C0,
        .intNum      = INT_I2C_IRQ,
        .intPriority = ~0,
        .swiPriority = 0,
        .sdaPin      = tempSens_HWV1_I2C0_SDA0,
        .sclPin      = tempSens_HWV1_I2C0_SCL0,
    }
};

const I2C_Config I2C_config[tempSens_HWV1_I2CCOUNT] = {
    {
        .fxnTablePtr = &I2CCC26XX_fxnTable,
        .object      = &i2cCC26xxObjects[tempSens_HWV1_I2C0],
        .hwAttrs     = &i2cCC26xxHWAttrs[tempSens_HWV1_I2C0]
    }
};

const uint_least8_t I2C_count = tempSens_HWV1_I2CCOUNT;

/*
 *  =============================== PIN ===============================
 */
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

const PIN_Config BoardGpioInitTable[] = {

    tempSens_HWV1_PIN_PRGLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,       /* LED initially off          */
    /*tempSens_HWV1_PIN_GLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,*/       /* LED initially off          */
    tempSens_HWV1_PIN_PRGBTN | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_BOTHEDGES | PIN_HYSTERESIS,          /* Button is active low       */
    /*tempSens_HWV1_PIN_BTN2 | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_BOTHEDGES | PIN_HYSTERESIS,*/        /* Button is active low       */
    /*tempSens_HWV1_SPI_FLASH_CS | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MIN, */ /* External flash chip select */
    tempSens_HWV1_UART_RX | PIN_INPUT_EN | PIN_PULLDOWN,                                              /* UART RX via debugger back channel */
    tempSens_HWV1_UART_TX | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL,                        /* UART TX via debugger back channel */
    tempSens_HWV1_SPI0_MOSI | PIN_INPUT_EN | PIN_PULLDOWN,                                            /* SPI master out - slave in */
    tempSens_HWV1_SPI0_MISO | PIN_INPUT_EN | PIN_PULLDOWN,                                            /* SPI master in - slave out */
    tempSens_HWV1_SPI0_CLK | PIN_INPUT_EN | PIN_PULLDOWN,                                             /* SPI clock */

    PIN_TERMINATE
};

const PINCC26XX_HWAttrs PINCC26XX_hwAttrs = {
    .intPriority = ~0,
    .swiPriority = 0
};

/*
 *  =============================== Power ===============================
 */
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>

const PowerCC26XX_Config PowerCC26XX_config = {
    .policyInitFxn      = NULL,
    .policyFxn          = &PowerCC26XX_standbyPolicy,
    .calibrateFxn       = &PowerCC26XX_calibrate,
    .enablePolicy       = TRUE,
    .calibrateRCOSC_LF  = TRUE,
    .calibrateRCOSC_HF  = TRUE,
};

/*
 *  =============================== PWM ===============================
 *  Remove unused entries to reduce flash usage both in Board.c and Board.h
 */
#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMTimerCC26XX.h>

PWMTimerCC26XX_Object pwmtimerCC26xxObjects[tempSens_HWV1_PWMCOUNT];

const PWMTimerCC26XX_HwAttrs pwmtimerCC26xxHWAttrs[tempSens_HWV1_PWMCOUNT] = {
    { .pwmPin = tempSens_HWV1_PWMPIN0, .gpTimerUnit = tempSens_HWV1_GPTIMER0A },
    { .pwmPin = tempSens_HWV1_PWMPIN1, .gpTimerUnit = tempSens_HWV1_GPTIMER0B },
    { .pwmPin = tempSens_HWV1_PWMPIN2, .gpTimerUnit = tempSens_HWV1_GPTIMER1A },
    { .pwmPin = tempSens_HWV1_PWMPIN3, .gpTimerUnit = tempSens_HWV1_GPTIMER1B },
    { .pwmPin = tempSens_HWV1_PWMPIN4, .gpTimerUnit = tempSens_HWV1_GPTIMER2A },
    { .pwmPin = tempSens_HWV1_PWMPIN5, .gpTimerUnit = tempSens_HWV1_GPTIMER2B },
    { .pwmPin = tempSens_HWV1_PWMPIN6, .gpTimerUnit = tempSens_HWV1_GPTIMER3A },
    { .pwmPin = tempSens_HWV1_PWMPIN7, .gpTimerUnit = tempSens_HWV1_GPTIMER3B },
};

const PWM_Config PWM_config[tempSens_HWV1_PWMCOUNT] = {
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[tempSens_HWV1_PWM0], &pwmtimerCC26xxHWAttrs[tempSens_HWV1_PWM0] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[tempSens_HWV1_PWM1], &pwmtimerCC26xxHWAttrs[tempSens_HWV1_PWM1] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[tempSens_HWV1_PWM2], &pwmtimerCC26xxHWAttrs[tempSens_HWV1_PWM2] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[tempSens_HWV1_PWM3], &pwmtimerCC26xxHWAttrs[tempSens_HWV1_PWM3] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[tempSens_HWV1_PWM4], &pwmtimerCC26xxHWAttrs[tempSens_HWV1_PWM4] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[tempSens_HWV1_PWM5], &pwmtimerCC26xxHWAttrs[tempSens_HWV1_PWM5] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[tempSens_HWV1_PWM6], &pwmtimerCC26xxHWAttrs[tempSens_HWV1_PWM6] },
    { &PWMTimerCC26XX_fxnTable, &pwmtimerCC26xxObjects[tempSens_HWV1_PWM7], &pwmtimerCC26xxHWAttrs[tempSens_HWV1_PWM7] },
};

const uint_least8_t PWM_count = tempSens_HWV1_PWMCOUNT;

/*
 *  =============================== RF Driver ===============================
 */
#include <ti/drivers/rf/RF.h>

const RFCC26XX_HWAttrs RFCC26XX_hwAttrs = {
    .hwiCpe0Priority = ~0,
    .hwiHwPriority   = ~0,
    .swiCpe0Priority =  0,
    .swiHwPriority   =  0,
};

/*
 *  =============================== SPI DMA ===============================
 */
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPICC26XXDMA.h>

SPICC26XXDMA_Object spiCC26XXDMAObjects[tempSens_HWV1_SPICOUNT];

const SPICC26XXDMA_HWAttrsV1 spiCC26XXDMAHWAttrs[tempSens_HWV1_SPICOUNT] = {
    {
        .baseAddr           = SSI0_BASE,
        .intNum             = INT_SSI0_COMB,
        .intPriority        = ~0,
        .swiPriority        = 0,
        .powerMngrId        = PowerCC26XX_PERIPH_SSI0,
        .defaultTxBufValue  = 0,
        .rxChannelBitMask   = 1<<UDMA_CHAN_SSI0_RX,
        .txChannelBitMask   = 1<<UDMA_CHAN_SSI0_TX,
        .mosiPin            = tempSens_HWV1_SPI0_MOSI,
        .misoPin            = tempSens_HWV1_SPI0_MISO,
        .clkPin             = tempSens_HWV1_SPI0_CLK,
        .csnPin             = tempSens_HWV1_SPI0_CSN
    },
    {
        .baseAddr           = SSI1_BASE,
        .intNum             = INT_SSI1_COMB,
        .intPriority        = ~0,
        .swiPriority        = 0,
        .powerMngrId        = PowerCC26XX_PERIPH_SSI1,
        .defaultTxBufValue  = 0,
        .rxChannelBitMask   = 1<<UDMA_CHAN_SSI1_RX,
        .txChannelBitMask   = 1<<UDMA_CHAN_SSI1_TX,
        .mosiPin            = tempSens_HWV1_SPI1_MOSI,
        .misoPin            = tempSens_HWV1_SPI1_MISO,
        .clkPin             = tempSens_HWV1_SPI1_CLK,
        .csnPin             = tempSens_HWV1_SPI1_CSN
    }
};

const SPI_Config SPI_config[tempSens_HWV1_SPICOUNT] = {
    {
         .fxnTablePtr = &SPICC26XXDMA_fxnTable,
         .object      = &spiCC26XXDMAObjects[tempSens_HWV1_SPI0],
         .hwAttrs     = &spiCC26XXDMAHWAttrs[tempSens_HWV1_SPI0]
    },
    {
         .fxnTablePtr = &SPICC26XXDMA_fxnTable,
         .object      = &spiCC26XXDMAObjects[tempSens_HWV1_SPI1],
         .hwAttrs     = &spiCC26XXDMAHWAttrs[tempSens_HWV1_SPI1]
    },
};

const uint_least8_t SPI_count = tempSens_HWV1_SPICOUNT;

/*
 *  =============================== UART ===============================
 */
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>

UARTCC26XX_Object uartCC26XXObjects[tempSens_HWV1_UARTCOUNT];

const UARTCC26XX_HWAttrsV2 uartCC26XXHWAttrs[tempSens_HWV1_UARTCOUNT] = {
    {
        .baseAddr       = UART0_BASE,
        .powerMngrId    = PowerCC26XX_PERIPH_UART0,
        .intNum         = INT_UART0_COMB,
        .intPriority    = ~0,
        .swiPriority    = 0,
        .txPin          = tempSens_HWV1_UART_TX,
        .rxPin          = tempSens_HWV1_UART_RX,
        .ctsPin         = PIN_UNASSIGNED,
        .rtsPin         = PIN_UNASSIGNED
    }
};

const UART_Config UART_config[tempSens_HWV1_UARTCOUNT] = {
    {
        .fxnTablePtr = &UARTCC26XX_fxnTable,
        .object      = &uartCC26XXObjects[tempSens_HWV1_UART0],
        .hwAttrs     = &uartCC26XXHWAttrs[tempSens_HWV1_UART0]
    },
};

const uint_least8_t UART_count = tempSens_HWV1_UARTCOUNT;

/*
 *  =============================== UDMA ===============================
 */
#include <ti/drivers/dma/UDMACC26XX.h>

UDMACC26XX_Object udmaObjects[tempSens_HWV1_UDMACOUNT];

const UDMACC26XX_HWAttrs udmaHWAttrs[tempSens_HWV1_UDMACOUNT] = {
    {
        .baseAddr    = UDMA0_BASE,
        .powerMngrId = PowerCC26XX_PERIPH_UDMA,
        .intNum      = INT_DMA_ERR,
        .intPriority = ~0
    }
};

const UDMACC26XX_Config UDMACC26XX_config[tempSens_HWV1_UDMACOUNT] = {
    {
         .object  = &udmaObjects[tempSens_HWV1_UDMA0],
         .hwAttrs = &udmaHWAttrs[tempSens_HWV1_UDMA0]
    },
};

/*
 *  =============================== Watchdog ===============================
 */
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogCC26XX.h>

WatchdogCC26XX_Object watchdogCC26XXObjects[tempSens_HWV1_WATCHDOGCOUNT];

const WatchdogCC26XX_HWAttrs watchdogCC26XXHWAttrs[tempSens_HWV1_WATCHDOGCOUNT] = {
    {
        .baseAddr    = WDT_BASE,
        .reloadValue = 1000 /* Reload value in milliseconds */
    },
};

const Watchdog_Config Watchdog_config[tempSens_HWV1_WATCHDOGCOUNT] = {
    {
        .fxnTablePtr = &WatchdogCC26XX_fxnTable,
        .object      = &watchdogCC26XXObjects[tempSens_HWV1_WATCHDOG0],
        .hwAttrs     = &watchdogCC26XXHWAttrs[tempSens_HWV1_WATCHDOG0]
    },
};

const uint_least8_t Watchdog_count = tempSens_HWV1_WATCHDOGCOUNT;

/*
 *  ======== tempSens_HWV1_initGeneral ========
 */
void tempSens_HWV1_initGeneral(void)
{
    Power_init();

    if (PIN_init(BoardGpioInitTable) != PIN_SUCCESS) {
        System_abort("Error with PIN_init\n");
    }
}
