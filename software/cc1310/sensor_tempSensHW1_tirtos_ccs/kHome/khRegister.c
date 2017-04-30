/**
 * @file khRegister.c
 * @author Peter Kappelt
 * @date 25.3.2017
 * Version of the register template: V1.0
 * Date of the register template: 25.3.2017
 *
 * Date of the register file generation: 2017-04-30 13:41:09.423
 *
 * This file was automatically generated out of the Device-File temperatureSensor.khd
 *
 * Device Information:
 * Author: Peter Kappelt
 * Device ID: 0x1/ 1
 * Version: S1.0, H1.0
 * Comment:
A general purpose temperature- and humidity sensor.<br/>Developed for kHome RF Protocol
 */

/**
 * Version History:
 * • V1.1 (20.4.2017)
 *  - added this version history
 * • V1.0 (19.4.2017)
 *  - initial revision
 */

#include "khRegister.h"

#define khReg_LEN1_DATATYPE uint8_t
#define khReg_LEN2_DATATYPE uint16_t
#define khReg_LEN4_DATATYPE uint32_t

#define true 1
#define false 0

/*
 *
 * Data Registers
 *
 */
 
/*
 * Data Register "temperature"
 * Address 0x1
 * Length: 4 Byte(s)
 * Initial Value: 0
 * Read Only: true
 * Description:
This value contains the measured temperature in tenth degrees<br/>You've to divide this value with ten to get the real value<br/>Furthermore, this value is represented as an 32bit signed integer
 */
khReg khRegDatatemperature_Reg;
khReg_LEN4_DATATYPE khRegDatatemperature_Data;

/*
 * Data Register "humidity"
 * Address 0x2
 * Length: 4 Byte(s)
 * Initial Value: 0
 * Read Only: true
 * Description:
This register contains the humidity value in tenth %RH<br/>You've to divide this value with ten to get the real value<br/>This value is represented as an 32 bit unsigned integer
 */
khReg khRegDatahumidity_Reg;
khReg_LEN4_DATATYPE khRegDatahumidity_Data;

/*
 * Data Register "batteryVoltage"
 * Address 0x4
 * Length: 2 Byte(s)
 * Initial Value: 0
 * Read Only: true
 * Description:
This register contains the voltage of the battery<br/>You've to divide it with 100 to get the real value in volts
 */
khReg khRegDatabatteryVoltage_Reg;
khReg_LEN2_DATATYPE khRegDatabatteryVoltage_Data;

/*
 * Data Register "batteryPercentage"
 * Address 0x3
 * Length: 1 Byte(s)
 * Initial Value: 0
 * Read Only: true
 * Description:
Percentage of battery remaining
 */
khReg khRegDatabatteryPercentage_Reg;
khReg_LEN1_DATATYPE khRegDatabatteryPercentage_Data;





/*
 *
 * Config Registers
 *
 */
 

/*
 * Config Register "deviceAddress"
 * Address 0x0
 * Initial Value: 0
 * Read Only: false
 * Description:
The kHome address, that the device listens to. <br/>This may be changed by the user when initializing a new device
 */
khReg khRegConfdeviceAddress_Reg;
uint8_t khRegConfdeviceAddress_Data;


/*
 * Config Register "autoTransmitInterval"
 * Address 0x10
 * Initial Value: 0
 * Read Only: false
 * Description:
The value of this register declares, how often the data registers 0x01 and 0x02 (temperature and humidity) get transmitted<br/>A value of 0 means, that they will never be broadcasted, only read manually
 */
khReg khRegConfautoTransmitInterval_Reg;
uint8_t khRegConfautoTransmitInterval_Data;


/*
 * Config Register "autoTransmitBattery"
 * Address 0x11
 * Initial Value: 0
 * Read Only: false
 * Description:
The battery voltage/ percentage will be read every autoTransmitInterval * autoTransmitBattery seconds<br/>If it changed, since last time, it'll be transmitted
 */
khReg khRegConfautoTransmitBattery_Reg;
uint8_t khRegConfautoTransmitBattery_Data;





/*
 *
 * Status Registers
 *
 */


/*
 * Data Register "globalStatus"
 * Address 0x0
 * Initial Value: 1
 * Description:
Is an Information about the devices status in general.<br/>If 0, everything is good.<br/>Anything but 0 indicates a fault.<br/>This register needs to be set to 0 after successfull initialisation in software.
 */
khReg khRegStatusglobalStatus_Reg;
uint8_t khRegStatusglobalStatus_Data;


/*
 * Data Register "deviceID"
 * Address 0x1
 * Initial Value: 1
 * Description:
ID of the device.<br/>May not be changed internally
 */
khReg khRegStatusdeviceID_Reg;
uint8_t khRegStatusdeviceID_Data;




/**
 * @brief init the register library
 * Init the register library,
 * prepare the register initial values
 */
void khRegisterInit(void){
    /*
     * Init Data Registers
     */
	 
	
    khRegDatatemperature_Reg.regAddress = 0x1;
    khRegDatatemperature_Reg.regType = khRegType_Data;
    khRegDatatemperature_Reg.regReadOnly = true;
    khRegDatatemperature_Reg.regLength = 4;
    khRegDatatemperature_Data = 0;
    khRegDatatemperature_Reg.dataLen4 = &khRegDatatemperature_Data;

	
    khRegDatahumidity_Reg.regAddress = 0x2;
    khRegDatahumidity_Reg.regType = khRegType_Data;
    khRegDatahumidity_Reg.regReadOnly = true;
    khRegDatahumidity_Reg.regLength = 4;
    khRegDatahumidity_Data = 0;
    khRegDatahumidity_Reg.dataLen4 = &khRegDatahumidity_Data;

	
    khRegDatabatteryVoltage_Reg.regAddress = 0x4;
    khRegDatabatteryVoltage_Reg.regType = khRegType_Data;
    khRegDatabatteryVoltage_Reg.regReadOnly = true;
    khRegDatabatteryVoltage_Reg.regLength = 2;
    khRegDatabatteryVoltage_Data = 0;
    khRegDatabatteryVoltage_Reg.dataLen2 = &khRegDatabatteryVoltage_Data;

	
    khRegDatabatteryPercentage_Reg.regAddress = 0x3;
    khRegDatabatteryPercentage_Reg.regType = khRegType_Data;
    khRegDatabatteryPercentage_Reg.regReadOnly = true;
    khRegDatabatteryPercentage_Reg.regLength = 1;
    khRegDatabatteryPercentage_Data = 0;
    khRegDatabatteryPercentage_Reg.dataLen1 = &khRegDatabatteryPercentage_Data;

	
	
	
	
	/*
     * Init Config Registers
     */
	 
	
    khRegConfdeviceAddress_Reg.regAddress = 0x0;
    khRegConfdeviceAddress_Reg.regType = khRegType_Config;
    khRegConfdeviceAddress_Reg.regReadOnly = false;
    khRegConfdeviceAddress_Reg.regLength = 1;
    khRegConfdeviceAddress_Data = 0;
    khRegConfdeviceAddress_Reg.dataLen1 = &khRegConfdeviceAddress_Data;

	
    khRegConfautoTransmitInterval_Reg.regAddress = 0x10;
    khRegConfautoTransmitInterval_Reg.regType = khRegType_Config;
    khRegConfautoTransmitInterval_Reg.regReadOnly = false;
    khRegConfautoTransmitInterval_Reg.regLength = 1;
    khRegConfautoTransmitInterval_Data = 0;
    khRegConfautoTransmitInterval_Reg.dataLen1 = &khRegConfautoTransmitInterval_Data;

	
    khRegConfautoTransmitBattery_Reg.regAddress = 0x11;
    khRegConfautoTransmitBattery_Reg.regType = khRegType_Config;
    khRegConfautoTransmitBattery_Reg.regReadOnly = false;
    khRegConfautoTransmitBattery_Reg.regLength = 1;
    khRegConfautoTransmitBattery_Data = 0;
    khRegConfautoTransmitBattery_Reg.dataLen1 = &khRegConfautoTransmitBattery_Data;

	
		
	
	
	/*
     * Init Status Registers
     */
	 
	
    khRegStatusglobalStatus_Reg.regAddress = 0x0;
    khRegStatusglobalStatus_Reg.regType = khRegType_Status;
    khRegStatusglobalStatus_Reg.regReadOnly = 1;
    khRegStatusglobalStatus_Reg.regLength = 1;
    khRegStatusglobalStatus_Data = 1;
    khRegStatusglobalStatus_Reg.dataLen1 = &khRegStatusglobalStatus_Data;

	
    khRegStatusdeviceID_Reg.regAddress = 0x1;
    khRegStatusdeviceID_Reg.regType = khRegType_Status;
    khRegStatusdeviceID_Reg.regReadOnly = 1;
    khRegStatusdeviceID_Reg.regLength = 1;
    khRegStatusdeviceID_Data = 1;
    khRegStatusdeviceID_Reg.dataLen1 = &khRegStatusdeviceID_Data;

	
}

/**
 * @brief get a pointer to a register, search for the register by its type and id
 * @param[in] registerType the type of the register to search for, an element of khRegType
 * @param[in] address the address of the register to search for
 * @param[out] registerOut a pointer to a khReg, which points to the found register
 *
 * @return a element of khRegRet, defines, whether register is found
 * @retval khRegRet_OK          everything went well, the pointer got written
 * @retval khRegRet_RegUnknown  the register is unknown, no pointer got written
 */
khRegRet khGetRegByAddress(khRegType registerType, uint8_t address, khReg** registerOut){
    if(registerType == khRegType_Data){
        if(0){

        }else if(address == 0x1){
            *registerOut = &khRegDatatemperature_Reg;
            return khRegRet_OK;
        }else if(address == 0x2){
            *registerOut = &khRegDatahumidity_Reg;
            return khRegRet_OK;
        }else if(address == 0x4){
            *registerOut = &khRegDatabatteryVoltage_Reg;
            return khRegRet_OK;
        }else if(address == 0x3){
            *registerOut = &khRegDatabatteryPercentage_Reg;
            return khRegRet_OK;
        }else{
            return khRegRet_RegUnknown;
        }
    }else if(registerType == khRegType_Config){
        if(0){

        }else if(address == 0x0){
            *registerOut = &khRegConfdeviceAddress_Reg;
            return khRegRet_OK;
        }else if(address == 0x10){
            *registerOut = &khRegConfautoTransmitInterval_Reg;
            return khRegRet_OK;
        }else if(address == 0x11){
            *registerOut = &khRegConfautoTransmitBattery_Reg;
            return khRegRet_OK;
        }else{
            return khRegRet_RegUnknown;
        }
    }else if(registerType == khRegType_Status){
        if(0){

        }else if(address == 0x0){
            *registerOut = &khRegStatusglobalStatus_Reg;
            return khRegRet_OK;
        }else if(address == 0x1){
            *registerOut = &khRegStatusdeviceID_Reg;
            return khRegRet_OK;
        }else{
            return khRegRet_RegUnknown;
        }
    }

    return khRegRet_RegUnknown;
}

/**
 * @brief get the value of a data register, search for the register by its address
 * @param[in] address   the address of the register
 * @param[out] length   a pointer, where the register length gets written to
 * @param[out] value    a pointer, where the value gets written to
 *
 * @return an element of khRegRet, defines, whether register is found
 * @retval khRegRet_OK          everything went well, the pointers got written
 * @retval khRegRet_RegUnknown  the register is unknown, no pointers got written
 */
khRegRet khGetDataRegValByAddress(uint8_t address, uint8_t* length, uint32_t* value){
    khReg* tempRegister;
    khRegRet tempReturn = khGetRegByAddress(khRegType_Data, address, &tempRegister);

    if(tempReturn != khRegRet_OK){
        return tempReturn;
    }else{
        *length = tempRegister->regLength;

        if(tempRegister->regLength == 1){
            *value = *tempRegister->dataLen1;
        }else if(tempRegister->regLength == 2){
            *value = *tempRegister->dataLen2;
        }else if(tempRegister->regLength == 4){
            *value = *tempRegister->dataLen4;
        }

        return khRegRet_OK;
    }
}

/**
 * @brief get the value of a config register, search for the register by its address
 * @param[in] address   the address of the register
 * @param[out] value    a pointer, where the value gets written to
 *
 * @return an element of khRegRet, defines, whether register is found
 * @retval khRegRet_OK          everything went well, the pointers got written
 * @retval khRegRet_RegUnknown  the register is unknown, no pointers got written
 */
khRegRet khGetConfigRegValByAddress(uint8_t address, uint8_t* value){
    khReg* tempRegister;
    khRegRet tempReturn = khGetRegByAddress(khRegType_Config, address, &tempRegister);

    if(tempReturn != khRegRet_OK){
        return tempReturn;
    }else{
        *value = *tempRegister->dataLen1;

        return khRegRet_OK;
    }
}

/**
 * @brief get the value of a status register, search for the register by its address
 * @param[in] address   the address of the register
 * @param[out] value    a pointer, where the value gets written to
 *
 * @return an element of khRegRet, defines, whether register is found
 * @retval khRegRet_OK          everything went well, the pointers got written
 * @retval khRegRet_RegUnknown  the register is unknown, no pointers got written
 */
khRegRet khGetStatusRegValByAddress(uint8_t address, uint8_t* value){
    khReg* tempRegister;
    khRegRet tempReturn = khGetRegByAddress(khRegType_Status, address, &tempRegister);

    if(tempReturn != khRegRet_OK){
        return tempReturn;
    }else{
        *value = *tempRegister->dataLen1;

        return khRegRet_OK;
    }
}

/**
 * @brief set the value of a data register, search for the register by its address
 * @param[in] address           the address of the register
 * @param[in] newValue          the new value of the register. Is an 4-byte value, but gets truncated to the register's length
 * @param[in] ignoreReadOnly    if set to anything but 0, we will write to a register, even if it is read-only
 *
 * @return an element of khRegRet, defines, whether register is found
 * @retval khRegRet_OK          everything went well
 * @retval khRegRet_RegUnknown  the register is unknown
 * @retval khRegRet_RegReadOnly the register is declared as read-only
 */
khRegRet khSetDataRegValByAddress(uint8_t address, uint32_t newValue, uint8_t ignoreReadOnly){
    khReg* tempRegister;
    khRegRet tempReturn = khGetRegByAddress(khRegType_Data, address, &tempRegister);

    if(tempReturn != khRegRet_OK){
        return tempReturn;
    }else{
        if(tempRegister->regReadOnly && !ignoreReadOnly){
            return khRegRet_RegReadOnly;
        }else{
            switch(tempRegister->regLength){
            case 1:
                *tempRegister->dataLen1 = newValue & 0xFF;
                break;
            case 2:
                *tempRegister->dataLen2 = newValue & 0xFFFF;
                break;
            case 4:
                *tempRegister->dataLen4 = newValue & 0xFFFFFFFF;
                break;
            }
            return khRegRet_OK;
        }
    }
}

/**
 * @brief set the value of a config register, search for the register by its address
 * @param[in] address           the address of the register
 * @param[in] newValue          the new value of the register
 * @param[in] ignoreReadOnly    if set to anything but 0, we will write to a register, even if it is read-only
 *
 * @return an element of khRegRet, defines, whether register is found
 * @retval khRegRet_OK          everything went well
 * @retval khRegRet_RegUnknown  the register is unknown
 * @retval khRegRet_RegReadOnly the register is declared as read-only
 */
khRegRet khSetConfigRegValByAddress(uint8_t address, uint8_t newValue, uint8_t ignoreReadOnly){
    khReg* tempRegister;
    khRegRet tempReturn = khGetRegByAddress(khRegType_Config, address, &tempRegister);

    if(tempReturn != khRegRet_OK){
        return tempReturn;
    }else{
        if(tempRegister->regReadOnly && !ignoreReadOnly){
            return khRegRet_RegReadOnly;
        }else{
            *tempRegister->dataLen1 = newValue;
            return khRegRet_OK;
        }
    }
}

/**
 * @brief set the value of a status register, search for the register by its address
 * @param[in] address           the address of the register
 * @param[in] newValue          the new value of the register
 * @param[in] ignoreReadOnly    if set to anything but 0, we will write to a register, even if it is read-only
 *
 * @return an element of khRegRet, defines, whether register is found
 * @retval khRegRet_OK          everything went well
 * @retval khRegRet_RegUnknown  the register is unknown
 * @retval khRegRet_RegReadOnly the register is declared as read-only
 */
khRegRet khSetStatusRegValByAddress(uint8_t address, uint8_t newValue, uint8_t ignoreReadOnly){
    khReg* tempRegister;
    khRegRet tempReturn = khGetRegByAddress(khRegType_Status, address, &tempRegister);

    if(tempReturn != khRegRet_OK){
        return tempReturn;
    }else{
        if(tempRegister->regReadOnly && !ignoreReadOnly){
            return khRegRet_RegReadOnly;
        }else{
            *tempRegister->dataLen1 = newValue;
            return khRegRet_OK;
        }
    }
}
