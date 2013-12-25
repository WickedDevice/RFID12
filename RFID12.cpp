/***********************************************************************
 * RFID12 Arduino Library
 *
 * Created by Victor Aprea
 * victor.aprea@wickeddevice.com
 * http://wickeddevice.com  
 *
 * This library is intended for use with the 
 * Innovations ID-12LA RFID Reader
 *
 * License: Creative Commons Attribution-ShareAlike 4.0 International
 *
 ***********************************************************************/
 
#include "RFID12.h"

RFID12::RFID12(void){
  uint8_t ii;
  for(ii = 0; ii < NUM_CHARS_IN_TAG; ii++){
    tag[ii] = ' ';
  }
  
  for(ii = 0; ii < NUM_CHARS_IN_CHECKSUM; ii++){
    checksum[ii] = ' ';
  }
  
  bytes_read       = 0;
  tag_read         = 0;      
  state            = WAITING;         
  previousMillis   = 0;
  timeoutInterval  = 1000;
}

void RFID12::inputByte(uint8_t c){

  unsigned long currentMillis = 0;
  switch(state){
    case WAITING:
      if(c == 0x02){
        currentMillis = millis();          
        state = PAYLOAD;  
        previousMillis = currentMillis;
        bytes_read = 0;     
      }  
      break;
    case PAYLOAD:
      if((c < 'A' || c > 'F') && (c < '0' || c > '9')){        
         state = WAITING;
      }
      else{      
        //Serial.print((char) c);
        
        tag[bytes_read++] = c;
        if(bytes_read == NUM_CHARS_IN_TAG){
          state = TRAILER;         
          bytes_read = 0;
        }    
      }
      break;
    case TRAILER:
      if(bytes_read < NUM_CHARS_IN_CHECKSUM){
        checksum[bytes_read] = c;   
        //Serial.print((char) c);
      }
      
      bytes_read++;
      
      if(bytes_read == 5){
        // message complete     
        if(verifyChecksum()){
          //Serial.println();
          //Serial.println("VERIFIED CHECKSUM");
          tag_read = 1;          // this tells the main loop there's something to look at in tag
        }
        
        state = WAITING;  
        
      }
      break;    
  }  
}

boolean RFID12::available(void){
  unsigned ret = 0;
  unsigned long currentMillis = millis();   

  if(tag_read == 1){
    ret = 1;
  }
  else if((state != WAITING) && (currentMillis - previousMillis > timeoutInterval)){
     //Serial.println();
     //Serial.println("TIMEOUT");
     state = WAITING;          
  }  
  
  return ret;
}

void RFID12::getTag(uint8_t * buf){
  for(uint8_t ii = 0, jj = 0; ii < NUM_CHARS_IN_TAG; ii+=2, jj++){
    buf[jj] = ascii2Hex(tag + ii);
  } 
  
  tag_read = 0;
}

uint8_t RFID12::verifyChecksum(void){
  
  // form the calculated checksum
  uint8_t ii = 0;
  uint8_t computed_checksum = 0;
  uint8_t numeric_checksum  = 0;
  
  for(ii = 0; ii < NUM_CHARS_IN_TAG; ii+=2){
    computed_checksum ^= ascii2Hex(tag + ii);
  }
  
  numeric_checksum = ascii2Hex(checksum);
  
  return (numeric_checksum == computed_checksum) ? 1 : 0;
}

uint8_t RFID12::ascii2Code(char c){
  uint8_t ret = 0;
    
  if(c >= 'A' && c <= 'Z'){
    ret = c - 'A'+ 10;
  }
  else if(c >= '0' && c <= '9'){
    ret = c - '0';
  }
  else{
    ret = 0;
  }
  
  return ret;
}

uint8_t RFID12::ascii2Hex(char * c){
  uint8_t value = ascii2Code(c[0]) << 4;   
  value |= ascii2Code(c[1]);
  return value;
}
