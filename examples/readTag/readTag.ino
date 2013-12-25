#include "RFID12.h"

RFID12 rfid;
uint8_t tag[5];

void setup(void){
  Serial.begin(9600); 
}

void loop(void){
  if(Serial.available()){
    rfid.inputByte(Serial.read());
  }
  
  if(rfid.available()){
    rfid.getTag(tag);
    printTag();
  }
}

void printTag(void){
  Serial.print(F("TAG: "));
  for(uint8_t ii = 0; ii < 5; ii++){
    if(tag[ii] < 0x10) Serial.print(F("0"));
    Serial.print(tag[ii], HEX);
    if(ii != 4) Serial.print(F(":"));
  }
}
