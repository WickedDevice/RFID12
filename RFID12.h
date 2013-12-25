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

#ifndef _RFID12_LIB_H
#define _RFID12_LIB_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <stdint.h>

typedef enum{
  WAITING = 0,
  PAYLOAD,  
  TRAILER  
} state_t;

#define NUM_CHARS_IN_TAG      10
#define NUM_CHARS_IN_CHECKSUM 2

class RFID12 {
  private:
    char tag[NUM_CHARS_IN_TAG];           // buffer for tag bytes
    char checksum[NUM_CHARS_IN_CHECKSUM]; // buffer for checksum bytes
    uint8_t bytes_read;                   // state transition helper variable
    uint8_t tag_read;                     // for the main process to know it's a new tag
    state_t state;                        // state machine variable
    long previousMillis, timeoutInterval; // timeout measurement variables
    
    uint8_t verifyChecksum(void);
    static uint8_t ascii2Code(char c);
    static uint8_t ascii2Hex(char * c);    
  public:  
	  RFID12(void);
	
	  void inputByte(uint8_t b);
	  boolean available(void);
	  void getTag(uint8_t * buf);
};

#endif
