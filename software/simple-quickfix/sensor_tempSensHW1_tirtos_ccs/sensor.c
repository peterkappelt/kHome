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
#include <stdio.h>

/* Example/Board Header files */
#include "Board.h"

/* Driver Header files */
#include <ti/drivers/I2C.h>
#include <ti/drivers/NVS.h>
#include <ti/devices/cc13x0/driverlib/flash.h>
#include <ti/devices/cc13x0/driverlib/aon_batmon.h>

#include "easylink/EasyLink.h"

#include "config.h"

/*
 * I2C:
 * Read from SI7020 sensor
 */
I2C_Handle i2cHandle;
I2C_Params i2cParams;

uint8_t i2cRxBuf[2];      // Receive buffer
uint8_t i2cTxCmd;

float battery = 0, batteryLast = 0;
uint8_t batteryPct = 0;
float rh = 0, rhLast = 0;
float temp = 0, tempLast = 0;

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

void txTaskFunction(UArg arg0, UArg arg1){
    //Init for GPIO and I2C
    GPIO_init();
    /*
     * I2C-init
     */
    I2C_init();
    // Configure I2C parameters.
    I2C_Params_init(&i2cParams);
    // Open I2C
    i2cHandle = I2C_open(tempSens_HWV1_I2C0, &i2cParams);


    EasyLink_init(EasyLink_Phy_50kbps2gfsk);

    EasyLink_setRfPwr(10);

    //number of loops without data being sent
    uint16_t idleLoopNumber = 0;

    //number of loops without battery being measured
    uint16_t idleBatteryLoops = UINT16_MAX;

    //data packet to send
    EasyLink_TxPacket txPacket = { {75}, 0, 0, {0} };

    while (1) {
        if(idleBatteryLoops >= SENSOR_BATTMEASURE){
            //measure battery voltage
            AONBatMonEnable();
            while(!AONBatMonNewBatteryMeasureReady());
            uint16_t batteryVoltageRaw = AONBatMonBatteryVoltageGet() & 0xFFFF;
            battery = (float)batteryVoltageRaw * exp2f(-8);
            AONBatMonDisable();

            /**
             * Calculate voltage in percentage,
             * where 3V and above are 100%
             * 2V and below are 0%
             * and proportional values inbetween
             */
            if(battery >= 3.0f){
                batteryPct = 100;
            }else if(battery <= 2.0f){
                batteryPct = 0;
            }else{
                batteryPct = (battery * 100.0f) - 200.0f;
            }
            idleBatteryLoops = 0;
        }else{
            idleBatteryLoops++;
        }

        if(!si7020updateTemperatureAndRh()){
            GPIO_write(Board_GPIO_PRGLED, 1);
            continue;
        }else{
            GPIO_write(Board_GPIO_PRGLED, 0);
        }

        if((battery != batteryLast) || (rh != rhLast) || (temp != tempLast) ||      // if a value changed
                (idleLoopNumber >= SENSOR_MAXIDLE && SENSOR_MAXIDLE != 0)       ){  // the sensor was idle for a certain period)

            txPacket.len = sprintf((char*)txPacket.payload, "{\"hwtype\":\"khtempv1\",\"id\":\"%s\",\"batt\":\"%.1f\",\"battpct\":\"%d\",\"temp\":\"%.1f\",\"rh\":\"%.1f\"}",
                                       SENSOR_NAME,
                                       battery,
                                       batteryPct,
                                       temp,
                                       rh);


            EasyLink_transmit(&txPacket);

            idleLoopNumber = 0;

            batteryLast = battery;
            rhLast = rh;
            tempLast = temp;
        }else{
            idleLoopNumber++;
        }

        sleep(SENSOR_TRANSMITPERIOD);
    }
}
