/**
 * @file mqtt.h
 * @author Peter Kappelt
 */

#include <stdint.h>

void mqttOpen(void);
void mqttClose(void);
void mqttPublishDataRegister(uint8_t deviceAddress, uint8_t registerAddress, uint32_t value);