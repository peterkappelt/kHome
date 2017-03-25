/**
 * @file khRegister.c
 * @author Peter Kappelt
 * @date 25.3.2017
 * Version of the register template: V1.0
 * Date of the register template: 25.3.2017
 *
 * Date of the register file generation: {$GEN_TIME}
 *
 * This file was automatically generated out of the Device-File {$FILE_NAME}
 *
 * Device Information:
 * Author: {$META_AUTHOR}
 * Device ID: 0x{$META_DEVICE_ID_HEX}/ {$META_DEVICE_ID_DEC}
 * Version: {$META_DEVICE_VERSION}
 * Comment:
{$META_COMMENT}
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
 
{$BLOCK_DATAREGISTER_START}/*
 * Data Register "{$NAME}"
 * Address 0x{$ADDRESS_HEX}
 * Length: {$LENGTH_BYTE} Byte(s)
 * Initial Value: {$INITIAL_VALUE}
 * Read Only: {$READ_ONLY}
 * Description:
{$DESCRIPTION}
 */
khReg khRegData{$NAME}_Reg;
khReg_LEN{$LENGTH_BYTE}_DATATYPE khRegData{$NAME}_Data;

{$BLOCK_DATAREGISTER_STOP}



/*
 *
 * Config Registers
 *
 */
 
{$BLOCK_CONFIGREGISTER_START}
/*
 * Config Register "{$NAME}"
 * Address 0x{$ADDRESS_HEX}
 * Initial Value: {$INITIAL_VALUE}
 * Read Only: {$READ_ONLY}
 * Description:
{$DESCRIPTION}
 */
khReg khRegConf{$NAME}_Reg;
uint8_t khRegConf{$NAME}_Data;

{$BLOCK_CONFIGREGISTER_STOP}



/*
 *
 * Status Registers
 *
 */

{$BLOCK_STATUSREGISTER_START}
/*
 * Data Register "{$NAME}"
 * Address 0x{$ADDRESS_HEX}
 * Initial Value: {$INITIAL_VALUE}
 * Description:
{$DESCRIPTION}
 */
khReg khRegStatus{$NAME}_Reg;
uint8_t khRegStatus{$NAME}_Data;

{$BLOCK_STATUSREGISTER_STOP}


/**
 * @brief init the register library
 * Init the register library,
 * prepare the register initial values
 */
void khRegisterInit(void){
    /*
     * Init Data Registers
     */
	 
	{$BLOCK_DATAREGISTER_START}
    khRegData{$NAME}_Reg.regAddress = 0x{$ADDRESS_HEX};
    khRegData{$NAME}_Reg.regType = khRegType_Data;
    khRegData{$NAME}_Reg.regReadOnly = {$READ_ONLY};
    khRegData{$NAME}_Reg.regLength = {$LENGTH_BYTE};
    khRegData{$NAME}_Data = {$INITIAL_VALUE};
    khRegData{$NAME}_Reg.dataLen{$LENGTH_BYTE} = &khRegData{$NAME}_Data;

	{$BLOCK_DATAREGISTER_STOP}
	
	
	
	/*
     * Init Config Registers
     */
	 
	{$BLOCK_CONFIGREGISTER_START}
    khRegConf{$NAME}_Reg.regAddress = 0x{$ADDRESS_HEX};
    khRegConf{$NAME}_Reg.regType = khRegType_Config;
    khRegConf{$NAME}_Reg.regReadOnly = {$READ_ONLY};
    khRegConf{$NAME}_Reg.regLength = 1;
    khRegConf{$NAME}_Data = {$INITIAL_VALUE};
    khRegConf{$NAME}_Reg.dataLen1 = &khRegConf{$NAME}_Data;

	{$BLOCK_CONFIGREGISTER_STOP}
		
	
	
	/*
     * Init Status Registers
     */
	 
	{$BLOCK_STATUSREGISTER_START}
    khRegStatus{$NAME}_Reg.regAddress = 0x{$ADDRESS_HEX};
    khRegStatus{$NAME}_Reg.regType = khRegType_Status;
    khRegStatus{$NAME}_Reg.regReadOnly = 1;
    khRegStatus{$NAME}_Reg.regLength = 1;
    khRegStatus{$NAME}_Data = {$INITIAL_VALUE};
    khRegStatus{$NAME}_Reg.dataLen1 = &khRegStatus{$NAME}_Data;

	{$BLOCK_STATUSREGISTER_STOP}
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

        }{$BLOCK_DATAREGISTER_START}else if(address == 0x{$ADDRESS_HEX}){
            *registerOut = &khRegData{$NAME}_Reg;
            return khRegRet_OK;
        }{$BLOCK_DATAREGISTER_STOP}else{
            return khRegRet_RegUnknown;
        }
    }else if(registerType == khRegType_Config){
        if(0){

        }{$BLOCK_CONFIGREGISTER_START}else if(address == 0x{$ADDRESS_HEX}){
            *registerOut = &khRegConf{$NAME}_Reg;
            return khRegRet_OK;
        }{$BLOCK_CONFIGREGISTER_STOP}else{
            return khRegRet_RegUnknown;
        }
    }else if(registerType == khRegType_Status){
        if(0){

        }{$BLOCK_STATUSREGISTER_START}else if(address == 0x{$ADDRESS_HEX}){
            *registerOut = &khRegStatus{$NAME}_Reg;
            return khRegRet_OK;
        }{$BLOCK_STATUSREGISTER_STOP}else{
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