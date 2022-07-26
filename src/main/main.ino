//====================================================================
//====================================================================
// Dhandhanggula Node
// Written in C++ Arduino
// Mesh Routing Protocol : Dynamic Source Routing
// Firmware version : 1.0
//
// MIT License
// For more information : https://github.com/dhandhanggula/node-cpp
//
// Be Carefull. 
// HIGHLY RECOMMENDED TO NOT CHANGE THE VOID SETUP AND VOID LOOP
//====================================================================


//====================================================================
// Library(s) ========================================================
//====================================================================

#include <SPI.h>
#include <LoRa.h>
#include <EEPROM.h>

#include <Wire.h>
#include "RTClib.h"

#include <MemoryUsage.h>
#include "base64.hpp"

//====================================================================
// LoRa Node Configuration ===========================================
// Read : 
// https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md ==
//====================================================================

#define txPower 17
#define LNAgain 0                     // 0 - 6
String nodeID = "";                   // to save nodID from EEPROM 

//====================================================================
// Network Configuration =============================================
// Read : 
// https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md ==
//====================================================================

#define networkFreq 915E6             // 433E6, 868E6, 915E6
#define networkSync 0x12              // 0x00 - 0xFF
#define spreadingFactor 7             // 6 - 12
#define signalBandwidth 125E3         // 7.8E3, 10.4E3, 15.6E3, 
                                      // 20.8E3, 31.25E3, 41.7E3, 
                                      // 62.5E3, 125E3, 250E3, 500E3
#define codingRateDen 5               // 5 - 8 => default 5 (4/5)

//====================================================================
// Mesh Route ========================================================
//====================================================================

String lastDestination = "";
String lastRoute = "";

//====================================================================
// R T C =============================================================
//====================================================================

RTC_DS1307 rtc;

//====================================================================
// Message Config ====================================================
// Message Structure
// MsgCode | MsgID | Sender | Destination | Route Path | Payload
//====================================================================

String parser = "|";
String messageReceived = "";
String payload = "";                    // payload only

unsigned long prevMillis = 0;           // to save latest millis
unsigned long pingMillis = 0;           // to save latest ping millis

String lastMsgID = "";
unsigned long msgMillis = 0;

String msgIDHistory[8] = {"", "", "", "", "", "", "", ""};

//====================================================================
// Encryption ========================================================
//====================================================================

// https://www.binaryhexconverter.com/ascii-text-to-hex-converter 
// for converting your String to hex (used here : testtesttesttest)

byte internalState[16] = {};          // to save state in process

const byte s[256] PROGMEM  = 
{
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

//====================================================================
// Other(s) ==========================================================
//====================================================================

void setup()
{
  // Reading the credentials saved in EEPROM
  for(int i=0; i<6; i++){
    nodeID += (char)EEPROM.read(i);
  }

  Serial.begin(115200);

  // Activate LoRa
  Serial.println(F("========================="));
  Serial.print(F("Node with ID "));
  Serial.print(nodeID);
  Serial.println(F(" is active"));
  Serial.println(F("========================="));

  LoRa.begin(networkFreq);
  //Serial.print(F("FREQ : "));
  Serial.println(networkFreq);

  LoRa.setTxPower(txPower);
  //Serial.print(F("TX (dB) : "));
  Serial.println(txPower);

  LoRa.setSpreadingFactor(spreadingFactor);
  //Serial.print(F("SF : "));
  Serial.println(spreadingFactor);

  LoRa.setSignalBandwidth(signalBandwidth);
  //Serial.print(F("Band : "));
  Serial.println(signalBandwidth);

  LoRa.setSyncWord(networkSync);
  //Serial.print(F("Sync : "));
  Serial.println(networkSync, HEX);

  //Serial.println(F(""));

  rtc.begin();
  rtc.adjust(DateTime(2022, 1, 1, 0, 0, 0));

  pinMode(A3, INPUT);
  randomSeed(analogRead(A3));

}

void loop()
{
  messageReceived = "";

  // read serial command
  String serialMsg = "";
  if(Serial.available())
  {
    delay(10);
    while(Serial.available() > 0)
    {
      serialMsg += (char)Serial.read();
    }
    Serial.flush();

    // pass to uartcommand
    Serial.print(serialMsg);
    uartcom(serialMsg);
  }

  onReceive(LoRa.parsePacket());

  // TESTING DISTANCE
  //uartcom("msg|YrZJNA|YrZJNA|test_distance");
  //delay(3000);
}


void updateMsgHistory(String newID)
{
  for(int i=0; i<8; i++){
    msgIDHistory[7 - i] = msgIDHistory[7 - (i+1)];
  }
  msgIDHistory[0] = newID;
}


void onReceive(int packetSize)
{
  if (packetSize == 0) return;          // if there's no packet, return
  while (LoRa.available()) {
    messageReceived += (char)LoRa.read();
  }
  answer(messageReceived);

  //Serial.println(messageReceived);
}

