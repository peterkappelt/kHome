/*
  Receiver for the temporary software of the kHome sensors
*/

#include "EasyLink.h"
#include "easylink/EasyLink.h"

EasyLink_RxPacket rxPacket;
EasyLink link;

uint8_t allowedPacketAddress = 'K';

void setup() {
  Serial.begin(115200);
  // begin defaults to EasyLink_Phy_50kbps2gfsk
  link.begin();

  rxPacket.rxTimeout = 0;
  // Turn the receiver on immediately
  rxPacket.absTime = EasyLink_ms_To_RadioTime(0);
  rxPacket.dstAddr[0] = allowedPacketAddress;

  EasyLink_enableRxAddrFilter(&allowedPacketAddress, 1, 1);
}

uint16_t value;
void loop() {
  EasyLink_Status status = link.receive(&rxPacket);
  
  if (status == EasyLink_Status_Success) {
   /* memcpy(&value, &rxPacket.payload, sizeof(uint16_t));
    Serial.print("Packet received with lenght ");
    Serial.print(rxPacket.len);
    Serial.print(" and value ");
    Serial.println(value);
*/
    Serial.print(rxPacket.rssi);
    Serial.print("|");
    Serial.write(rxPacket.payload, rxPacket.len);
    Serial.println();
  }
}
