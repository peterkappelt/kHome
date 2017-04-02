/**
 * @file khRegister.c
 * @author Peter Kappelt
 * @date 25.3.2017
 * Version of the register template: V1.0
 * Date of the register template: 25.3.2017
 *
 * Date of the register file generation: 2017-03-25 18:53:57.499
 *
 * This file was automatically generated out of the Device-File test.khd
 *
 * Device Information:
 * Author: Peter Kappelt
 * Device ID: 0xAB/ 171
 * Version: V1.0
 * Comment:
This is an empty device. Just for testing!<br/>An a second line of description.
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
 * Data Register "dataRegOne"
 * Address 0x1
 * Length: 2 Byte(s)
 * Initial Value: 15
 * Read Only: true
 * Description:
Read only,<br/>initial 15,<br/>dataRegOne,<br/>Length 2,<br/>Address 1
 */
khReg khRegDatadataRegOne_Reg;
khReg_LEN2_DATATYPE khRegDatadataRegOne_Data;

/*
 * Data Register "dataRegTwo"
 * Address 0x2
 * Length: 4 Byte(s)
 * Initial Value: 2
 * Read Only: false
 * Description:
initial 2,<br/>dataRegTwo,<br/>Length 4,<br/>Address 2
 */
khReg khRegDatadataRegTwo_Reg;
khReg_LEN4_DATATYPE khRegDatadataRegTwo_Data;





/*
 *
 * Config Registers
 *
 */
 

/*
 * Config Register "configOne"
 * Address 0x5
 * Initial Value: 2
 * Read Only: false
 * Description:
configOne,<br/>address 5,<br/>value 2
 */
khReg khRegConfconfigOne_Reg;
uint8_t khRegConfconfigOne_Data;


/*
 * Config Register "configTwo"
 * Address 0x6
 * Initial Value: 7
 * Read Only: true
 * Description:
configTwo:<br/>address 6<br/>value 7<br/>read only
 */
khReg khRegConfconfigTwo_Reg;
uint8_t khRegConfconfigTwo_Data;





/*
 *
 * Status Registers
 *
 */


/*
 * Data Register "statusOne"
 * Address 0x8
 * Initial Value: 3
 * Description:
statusOne:<br/>Value 3<br/>Address 8
 */
khReg khRegStatusstatusOne_Reg;
uint8_t khRegStatusstatusOne_Data;


/*
 * Data Register "statusTwo"
 * Address 0x9
 * Initial Value: 200
 * Description:
statusTwo:<br/>address 9<br/>value 200
 */
khReg khRegStatusstatusTwo_Reg;
uint8_t khRegStatusstatusTwo_Data;




/**
 * @brief init the register library
 * Init the register library,
 * prepare the register initial values
 */
void khRegisterInit(void){
    /*
     * Init Data Registers
     */
	 
	
    khRegDatadataRegOne_Reg.regAddress = 0x1;
    khRegDatadataRegOne_Reg.regType = khRegType_Data;
    khRegDatadataRegOne_Reg.regReadOnly = true;
    khRegDatadataRegOne_Reg.regLength = 2;
    khRegDatadataRegOne_Data = 15;
    khRegDatadataRegOne_Reg.dataLen2 = &khRegDatadataRegOne_Data;

	
    khRegDatadataRegTwo_Reg.regAddress = 0x2;
    khRegDatadataRegTwo_Reg.regType = khRegType_Data;
    khRegDatadataRegTwo_Reg.regReadOnly = false;
    khRegDatadataRegTwo_Reg.regLength = 4;
    khRegDatadataRegTwo_Data = 2;
    khRegDatadataRegTwo_Reg.dataLen4 = &khRegDatadataRegTwo_Data;

	
	
	
	
	/*
     * Init Config Registers
     */
	 
	
    khRegConfconfigOne_Reg.regAddress = 0x5;
    khRegConfconfigOne_Reg.regType = khRegType_Config;
    khRegConfconfigOne_Reg.regReadOnly = false;
    khRegConfconfigOne_Reg.regLength = 1;
    khRegConfconfigOne_Data = 2;
    khRegConfconfigOne_Reg.dataLen1 = &khRegConfconfigOne_Data;

	
    khRegConfconfigTwo_Reg.regAddress = 0x6;
    khRegConfconfigTwo_Reg.regType = khRegType_Config;
    khRegConfconfigTwo_Reg.regReadOnly = true;
    khRegConfconfigTwo_Reg.regLength = 1;
    khRegConfconfigTwo_Data = 7;
    khRegConfconfigTwo_Reg.dataLen1 = &khRegConfconfigTwo_Data;

	
		
	
	
	/*
     * Init Status Registers
     */
	 
	
    khRegStatusstatusOne_Reg.regAddress = 0x8;
    khRegStatusstatusOne_Reg.regType = khRegType_Status;
    khRegStatusstatusOne_Reg.regReadOnly = 1;
    khRegStatusstatusOne_Reg.regLength = 1;
    khRegStatusstatusOne_Data = 3;
    khRegStatusstatusOne_Reg.dataLen1 = &khRegStatusstatusOne_Data;

	
    khRegStatusstatusTwo_Reg.regAddress = 0x9;
    khRegStatusstatusTwo_Reg.regType = khRegType_Status;
    khRegStatusstatusTwo_Reg.regReadOnly = 1;
    khRegStatusstatusTwo_Reg.regLength = 1;
    khRegStatusstatusTwo_Data = 200;
    khRegStatusstatusTwo_Reg.dataLen1 = &khRegStatusstatusTwo_Data;

	
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
            *registerOut = &khRegDatadataRegOne_Reg;
            return khRegRet_OK;
        }else if(address == 0x2){
            *registerOut = &khRegDatadataRegTwo_Reg;
            return khRegRet_OK;
        }else{
            return khRegRet_RegUnknown;
        }
    }else if(registerType == khRegType_Config){
        if(0){

        }else if(address == 0x5){
            *registerOut = &khRegConfconfigOne_Reg;
            return khRegRet_OK;
        }else if(address == 0x6){
            *registerOut = &khRegConfconfigTwo_Reg;
            return khRegRet_OK;
        }else{
            return khRegRet_RegUnknown;
        }
    }else if(registerType == khRegType_Status){
        if(0){

        }else if(address == 0x8){
            *registerOut = &khRegStatusstatusOne_Reg;
            return khRegRet_OK;
        }else if(address == 0x9){
            *registerOut = &khRegStatusstatusTwo_Reg;
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
