/**
 * @file khRegister.c
 * @author Peter Kappelt
 */

/**
 * Version History:
 * • V1.1 (20.4.2017)
 *  - added this version history
 * • V1.0 (19.4.2017)
 *  - initial revision
 */

#ifndef KHOME_KHREGISTER_H_
#define KHOME_KHREGISTER_H_

#include <stdint.h>

typedef enum{
    khRegType_Data = 0,
    khRegType_Config,
    khRegType_Status,
}khRegType;

typedef enum{
    khRegRet_OK = 0,
    //khRegRet_RegInvalidLen,
    //khRegRet_RegInvalidValue,
    khRegRet_RegReadOnly,
    khRegRet_RegUnknown
}khRegRet;

typedef struct{
    khRegType regType;
    uint8_t regAddress;
    uint8_t regReadOnly;
    uint8_t regLength;
    union{
        uint8_t* dataLen1;
        uint16_t* dataLen2;
        uint32_t* dataLen4;
    };
}khReg;

void khRegisterInit(void);
khRegRet khGetRegByAddress(khRegType registerType, uint8_t address, khReg** registerOut);
khRegRet khGetDataRegValByAddress(uint8_t address, uint8_t* length, uint32_t* value);
khRegRet khGetConfigRegValByAddress(uint8_t address, uint8_t* value);
khRegRet khGetStatusRegValByAddress(uint8_t address, uint8_t* value);
khRegRet khSetDataRegValByAddress(uint8_t address, uint32_t newValue, uint8_t ignoreReadOnly);
khRegRet khSetConfigRegValByAddress(uint8_t address, uint8_t newValue, uint8_t ignoreReadOnly);
khRegRet khSetStatusRegValByAddress(uint8_t address, uint8_t newValue, uint8_t ignoreReadOnly);

inline uint8_t khGetDeviceID(void){return 0x{$META_DEVICE_ID_HEX};}

#endif /* KHOME_KHREGISTER_H_ */
