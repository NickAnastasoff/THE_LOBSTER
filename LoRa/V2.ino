#include <RadioLib.h>

SX1262 radio = new Module(D36, D40, D44, D39, SPI1);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1262 radio = RadioShield.ModuleA;

enum msgType : int {
    ROUTE,
    DATAREQUEST,
    SPECIFICREQUEST,
    IDLE
}

final String serialNumber = "5687";
//identifying serial number for transmission
volatile bool scanFlag = false;
bool receiving = false;

final char startChar = '<';
final char endChar = '>';
void setFlag(void) {
  // something happened, set the flag
  scanFlag = true;
}

String receivedMsgType;
String receivedMsg;

volatile bool atDestination = false;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  
  // initialize SX1262 with default settings
  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.begin(915.0, 250.0, 7, 5, 0x34, 20, 10, 0, false);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
  receivedMsg = "";
  receivedMsgType = "";
  msgType = IDLE;
}



void loop(){

    switch(msgType){
        case ROUTE:
            Transmit(receivedMsg);
            break;
        case DATAREQUEST:
            // sends a request to get new data
            Serial1.println("gimme new data");//change this 
            break;
        case SPECIFICREQUEST:
            Serial1.println("gimme specific data"); //change this
            // sends a request to get specific lines from the sd card
            break;
        case IDLE:
            break;
    }
  // check if the flag is set
  if(scanFlag) {
    int state = RADIOLIB_ERR_NONE;

    // reset flag
    scanFlag = false;

    // check ongoing reception
    if(receiving) {
      // DIO triggered while reception is ongoing
      // that means we got a packet
      String str;
      state = radio.readData(str);

      if (state == RADIOLIB_ERR_NONE) {
        // packet was successfully received
        Serial.println(F("[SX1262] Received packet!"));
        int lastCharAddress = strlen(str) - 1;
        char lastChar = str[lastCharAddress];

        /*
        *check if the first part of the packet is the start of 
        *the message and if the destination is the current lora
        */

        atDestination = str.substring(0,4) == startChar + serialNumber;
        
        // print data of the packet
        Serial.print(F("[SX1262] Data:\t\t"));
        Serial.println(str);

        if (lastChar == endChar){
            receivedMsg + str;
            if(atDestination){
                msgType = DESTINATION;
            } else {
                msgType = receivedMsgType;
            }
        } else{
            receivedMsg + str;
        }
      } else {
        // some other error occurred
        Serial.print(F("[SX1262] Failed, code "));
        Serial.println(state);
  
      }

      // reception is done now
      receiving = false;
      
    } else {
      // check CAD result
      state = radio.getChannelScanResult();

      if (state == RADIOLIB_LORA_DETECTED) {
        // LoRa packet was detected
        Serial.print(F("[SX1262] Packet detected, starting reception ... "));
        state = radio.startReceive();
        if (state == RADIOLIB_ERR_NONE) {
          Serial.println(F("success!"));
        } else {
          Serial.print(F("failed, code "));
          Serial.println(state);
        }

        // set the flag for ongoing reception
        receiving = true;

      } else if (state == RADIOLIB_CHANNEL_FREE) {
        // channel is free
        Serial.println(F("[SX1262] Channel is free!"));

      } else {
        // some other error occurred
        Serial.print(F("[SX1262] Failed, code "));
        Serial.println(state);

      }

    }

    // if we're not receiving, start scanning again
    if(!receiving) {
      Serial.print(F("[SX1262] Starting scan for LoRa preamble ... "));
      state = radio.startChannelScan();
      if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
      } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
      }
    
    }

  }
    
}

void Transmit(String Message){

radio.transmit(Message);

}