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
 *  ======== pwmled.c ========
 */
/* For usleep() */
#include <unistd.h>
#include <stddef.h>
#include <math.h>

/* Example/Board Header files */
#include "Board.h"

/* Driver Header files */
#include <ti/drivers/I2C.h>
#include <ti/drivers/NVS.h>
#include <ti/devices/cc13x0/driverlib/flash.h>
#include <ti/devices/cc13x0/driverlib/aon_batmon.h>

#include "kHome/khRegister.h"
#include "kHome/khRF.h"
#include "kHome/khTelegram.h"

#include "programmingMode/programmingMode.h"

/*
 * I2C:
 * Read from SI7020 sensor
 */
I2C_Handle i2cHandle;
I2C_Params i2cParams;

uint8_t i2cRxBuf[2];      // Receive buffer
uint8_t i2cTxCmd;

float rh = 0.0, temp = 0.0;
uint32_t humidityLastTransmitted = 0, temperatureLastTransmitted = 0;
uint16_t batteryLastTransmitted = 0;

/*
 * NVS: write global configs
 */
NVS_Handle nvsHandle;

/**
 * struct for variables, that shall be stored permanently
 */

NVS_Handle nvsHandle;

typedef struct{
    uint8_t magicKey;
    uint8_t deviceAddress;
    uint8_t autoTransmitInterval;
    uint8_t autoTransmitBattery;
}configNV_t;

configNV_t configNV = {
   .magicKey = 0xAA,        //this is used to check, whether the NV-memory contains a valid struct with data
   .deviceAddress = 0,
   .autoTransmitInterval = 0,
   .autoTransmitBattery = 0,
};

inline uint8_t configNVSize(){
    //make sure, the size is a multiple of 4
    return (sizeof(configNV_t) + 4) & 0xFFFFFFFFC;
}

uint8_t configDeviceAddressOld = 0;
uint8_t configAutoTransmitIntervalOld = 0;
uint8_t configAutoTransmitBatteryOld = 0;

/**
 * @brief set the global temperature and RH value by reading it from the SI7020
 * @retcode 1 everything went well
 * @retcode 0 there was an error, like a NACK or timeout
 */
uint8_t si7020updateTemperatureAndRh(void){
    I2C_Transaction i2cTrans;

    //Step 1: read RH

    //write the command 0xF5 to the SI7020, for initiating the conversion
    i2cTrans.writeCount   = 1;
    i2cTrans.writeBuf     = &i2cTxCmd;
    i2cTrans.readCount    = 0;
    i2cTrans.readBuf      = i2cRxBuf;
    i2cTrans.slaveAddress = 0x40;
    i2cTxCmd = 0xE5;
    uint8_t status = I2C_transfer(i2cHandle, &i2cTrans);
    if(!status){
        return 0;
    }

    //start to read the data from the device. The device uses clock stretching as long, as the conversion is in prograss
    i2cTrans.writeCount   = 0;
    i2cTrans.writeBuf     = &i2cTxCmd;
    i2cTrans.readCount    = 2;
    i2cTrans.readBuf      = i2cRxBuf;
    i2cTrans.slaveAddress = 0x40;

    //usleep(20000);

    status = I2C_transfer(i2cHandle, &i2cTrans);
    if(!status){
        return 0;
    }

    //now we can calculate the relative humidity, like shown in the SI7020 datasheet page 22
    rh = (i2cRxBuf[0] << 8) | i2cRxBuf[1];
    rh = (rh * 125) / 65536 - 6;

    //Step 2: read temperature
    //write the command 0xE0 to the sensor
    //this is a special command: it won't issue a new reading, because a temperature reading was already taking at the humidity conversion
    //so we do not need to wait for a conversion
    i2cTrans.writeCount   = 1;
    i2cTrans.writeBuf     = &i2cTxCmd;
    i2cTrans.readCount    = 0;
    i2cTrans.readBuf      = i2cRxBuf;
    i2cTrans.slaveAddress = 0x40;
    i2cTxCmd = 0xE0;
    status = I2C_transfer(i2cHandle, &i2cTrans);
    if(!status){
        return 0;
    }

    //read the two bytes, waiting is not necessary
    i2cTrans.writeCount   = 0;
    i2cTrans.writeBuf     = &i2cTxCmd;
    i2cTrans.readCount    = 2;
    i2cTrans.readBuf      = i2cRxBuf;
    i2cTrans.slaveAddress = 0x40;

    status = I2C_transfer(i2cHandle, &i2cTrans);

    if(!status){
        return 0;
    }

    //calculate the temperature value, like shown in the datasheet page 23
    temp = (i2cRxBuf[0] << 8) | i2cRxBuf[1];
    temp = (temp * 175.72) / 65536 - 46.85;

    return 1;
}

void configNVRead(void){
    configNV_t configNVRead;

    int status = NVS_read(nvsHandle, 0, &configNVRead, configNVSize());
    if (status != NVS_SOK) {
        //NVS_read failed
        programmingModeSetLed(1);
        while(1);
    }

    if(configNVRead.magicKey == 0xAA){
        //the magic key is valid -> copy the information
        configNV.deviceAddress = configNVRead.deviceAddress;
        configNV.autoTransmitInterval = configNVRead.autoTransmitInterval;
        configNV.autoTransmitBattery = configNVRead.autoTransmitBattery;

        configDeviceAddressOld = configNVRead.deviceAddress;
        configAutoTransmitIntervalOld = configNVRead.autoTransmitInterval;
        configAutoTransmitBatteryOld = configNVRead.autoTransmitBattery;
    }
}

void configNVWrite(void){
    int status = NVS_write(nvsHandle, 0, &configNV, configNVSize(), NVS_WRITE_ERASE | NVS_WRITE_VALIDATE);
    if (status != NVS_SOK) {
        //NVS_write failed
        programmingModeSetLed(1);
        while(1);
    }

    configDeviceAddressOld = configNV.deviceAddress;
    configAutoTransmitIntervalOld = configNV.autoTransmitInterval;
    configAutoTransmitBatteryOld = configNV.autoTransmitBattery;
}

/*
 *  ======== mainThread ========
 *  Task handles the sensor function
 */
void *mainThread(void *arg0)
{
    /*
     * Init higher level kHome libraries
     * Caution: the sequence of the kHome init is important
     * before bending the device register to our non-volatile solution, we must call the register init function
     * there, the pointer to the register value gets set to the actual data, not our solution.
     */
    khRegisterInit();

    //get the register pointers for the config registers that contain device address and auto transmit interval
    //we want to overwrite the data with the config from the non-volatile storage
    khReg* deviceAddressRegister;
    khReg* autoTransmitIntervalRegister;
    khReg* autoTransmitBatteryRegister;

    if(khGetRegByAddress(khRegType_Config, 0x0, &deviceAddressRegister) != khRegRet_OK){
        programmingModeSetLed(1);
        while(1);
    }
    if(khGetRegByAddress(khRegType_Config, 0x10, &autoTransmitIntervalRegister) != khRegRet_OK){
        programmingModeSetLed(1);
        while(1);
    }
    if(khGetRegByAddress(khRegType_Config, 0x11, &autoTransmitBatteryRegister) != khRegRet_OK){
        programmingModeSetLed(1);
        while(1);
    }

    //overwrite the data pointers
    deviceAddressRegister->dataLen1 = &(configNV.deviceAddress);
    autoTransmitIntervalRegister->dataLen1 = &(configNV.autoTransmitInterval);
    autoTransmitBatteryRegister->dataLen1 = &(configNV.autoTransmitBattery);

    /*
     * Init higher level kHome libraries
     * Caution: the sequence of the kHome init is important
     * We must call telegramInit after we've set the new pointer for the device address
     */
    khTelegramInit();
    khRFInit();

    //get the register pointers for the data registers, that contain humidity and temperature
    khReg* temperatureRegister;
    khReg* humidityRegister;
    khReg* batteryVoltageRegister;
    khReg* batteryPercentageRegister;
    if(khGetRegByAddress(khRegType_Data, 0x01, &temperatureRegister) != khRegRet_OK){
        programmingModeSetLed(1);
        while(1);
    }
    if(khGetRegByAddress(khRegType_Data, 0x02, &humidityRegister) != khRegRet_OK){
        programmingModeSetLed(1);
        while(1);
    }
    if(khGetRegByAddress(khRegType_Data, 0x03, &batteryPercentageRegister) != khRegRet_OK){
        programmingModeSetLed(1);
        while(1);
    }
    if(khGetRegByAddress(khRegType_Data, 0x04, &batteryVoltageRegister) != khRegRet_OK){
        programmingModeSetLed(1);
        while(1);
    }

    /*
     * I2C-init
     */
    I2C_init();
    // Configure I2C parameters.
    I2C_Params_init(&i2cParams);
    // Open I2C
    i2cHandle = I2C_open(tempSens_HWV1_I2C0, &i2cParams);

    /*
     * NVS init
     */
    /* Confirming the sector size on this device is 4096 */
    if (FlashSectorSizeGet() != 4096) {
        //Flash sector size is not 4096
        programmingModeSetLed(1);
        while(1);
    }

    NVS_init();
    nvsHandle = NVS_open(tempSens_HWV1_NVS1F000, NULL);

    //read the last config
    configNVRead();

    /**
     * Init the hardware for the programming mode toggle button
     */
    programmingModeInit();

    /*
     * start receiving commands
     */
    /*
     * Edit: do not start receive by default.
     * We only receive during programming mode, which is coded seperately
     */
    //khRFReceiveMode();

    /**
     * increment it every loop iteration
     * once it is equal to config register 0x11 value, we read the battery voltage and transmit it
     */
    uint8_t batteryTransmitIterations = 0;

    while (1) {
        if(*autoTransmitBatteryRegister->dataLen1 != 0){
            batteryTransmitIterations++;
            if(batteryTransmitIterations >= *autoTransmitBatteryRegister->dataLen1){
                AONBatMonEnable();
                while(!AONBatMonNewBatteryMeasureReady());

                uint16_t batteryVoltageRaw = AONBatMonBatteryVoltageGet() & 0xFFFF;
                float batteryVoltage = (float)batteryVoltageRaw * exp2f(-8);
                *batteryVoltageRegister->dataLen2 = (uint16_t)(batteryVoltage * 100.0);

                AONBatMonDisable();

                /**
                 * Calculate voltage in percentage,
                 * where 3V and above are 100%
                 * 2V and below are 0%
                 * and proportional values inbetween
                 */
                if(*batteryVoltageRegister->dataLen2 >= 300){
                    *batteryPercentageRegister->dataLen1 = 100;
                }else if(*batteryVoltageRegister->dataLen2 <= 200){
                    *batteryPercentageRegister->dataLen1 = 0;
                }else{
                    *batteryPercentageRegister->dataLen1 = *batteryVoltageRegister->dataLen2 - 200;
                }

                if(batteryLastTransmitted != *batteryPercentageRegister->dataLen2){
                    khRFBroadcastDataRegister(0x03);
                    usleep(10000);
                    batteryLastTransmitted = *batteryPercentageRegister->dataLen2;
                }
            }
        }


        if(si7020updateTemperatureAndRh()){
            *temperatureRegister->dataLen4 = (int32_t)(temp * 10);
            *humidityRegister->dataLen4 = (uint32_t)(rh * 10);
        }
        if(configNV.autoTransmitInterval != 0){
            if(temperatureLastTransmitted != *temperatureRegister->dataLen4){
                temperatureLastTransmitted = *temperatureRegister->dataLen4;
                khRFBroadcastDataRegister(0x01);
            }
            usleep(10000);
            if(humidityLastTransmitted != *humidityRegister->dataLen4){
                humidityLastTransmitted = *humidityRegister->dataLen4;
                khRFBroadcastDataRegister(0x02);
            }
            sleep(configNV.autoTransmitInterval);
        }else{
            sleep(10);
        }

        //if the config was changed, write it to the non-volatile memory
        if((configDeviceAddressOld != configNV.deviceAddress) || (configAutoTransmitIntervalOld != configNV.autoTransmitInterval) || (configAutoTransmitBatteryOld) != configNV.autoTransmitBattery){
            configNVWrite();
        }
    }
}
