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


byte sbox(byte stateHex)
{
  if(stateHex == 0x00){return 0x63;}
  if(stateHex == 0x01){return 0x7c;}
  if(stateHex == 0x02){return 0x77;}
  if(stateHex == 0x03){return 0x7b;}
  if(stateHex == 0x04){return 0xf2;}
  if(stateHex == 0x05){return 0x6b;}
  if(stateHex == 0x06){return 0x6f;}
  if(stateHex == 0x07){return 0xc5;}
  if(stateHex == 0x08){return 0x30;}
  if(stateHex == 0x09){return 0x01;}
  if(stateHex == 0x0a){return 0x67;}
  if(stateHex == 0x0b){return 0x2b;}
  if(stateHex == 0x0c){return 0xfe;}
  if(stateHex == 0x0d){return 0xd7;}
  if(stateHex == 0x0e){return 0xab;}
  if(stateHex == 0x0f){return 0x76;}

  if(stateHex == 0x10){return 0xca;}
  if(stateHex == 0x11){return 0x82;}
  if(stateHex == 0x12){return 0xc9;}
  if(stateHex == 0x13){return 0x7d;}
  if(stateHex == 0x14){return 0xfa;}
  if(stateHex == 0x15){return 0x59;}
  if(stateHex == 0x16){return 0x47;}
  if(stateHex == 0x17){return 0xf0;}
  if(stateHex == 0x18){return 0xad;}
  if(stateHex == 0x19){return 0xd4;}
  if(stateHex == 0x1a){return 0xa2;}
  if(stateHex == 0x1b){return 0xaf;}
  if(stateHex == 0x1c){return 0x9c;}
  if(stateHex == 0x1d){return 0xa4;}
  if(stateHex == 0x1e){return 0x72;}
  if(stateHex == 0x1f){return 0xc0;}

  if(stateHex == 0x20){return 0xb7;}
  if(stateHex == 0x21){return 0xfd;}
  if(stateHex == 0x22){return 0x93;}
  if(stateHex == 0x23){return 0x26;}
  if(stateHex == 0x24){return 0x36;}
  if(stateHex == 0x25){return 0x3f;}
  if(stateHex == 0x26){return 0xf7;}
  if(stateHex == 0x27){return 0xcc;}
  if(stateHex == 0x28){return 0x34;}
  if(stateHex == 0x29){return 0xa5;}
  if(stateHex == 0x2a){return 0xe5;}
  if(stateHex == 0x2b){return 0xf1;}
  if(stateHex == 0x2c){return 0x71;}
  if(stateHex == 0x2d){return 0xd8;}
  if(stateHex == 0x2e){return 0x31;}
  if(stateHex == 0x2f){return 0x15;}

  if(stateHex == 0x30){return 0x04;}
  if(stateHex == 0x31){return 0xc7;}
  if(stateHex == 0x32){return 0x23;}
  if(stateHex == 0x33){return 0xc3;}
  if(stateHex == 0x34){return 0x18;}
  if(stateHex == 0x35){return 0x96;}
  if(stateHex == 0x36){return 0x05;}
  if(stateHex == 0x37){return 0x9a;}
  if(stateHex == 0x38){return 0x07;}
  if(stateHex == 0x39){return 0x12;}
  if(stateHex == 0x3a){return 0x80;}
  if(stateHex == 0x3b){return 0xe2;}
  if(stateHex == 0x3c){return 0xeb;}
  if(stateHex == 0x3d){return 0x27;}
  if(stateHex == 0x3e){return 0xb2;}
  if(stateHex == 0x3f){return 0x75;}

  if(stateHex == 0x40){return 0x09;}
  if(stateHex == 0x41){return 0x83;}
  if(stateHex == 0x42){return 0x2c;}
  if(stateHex == 0x43){return 0x1a;}
  if(stateHex == 0x44){return 0x1b;}
  if(stateHex == 0x45){return 0x6e;}
  if(stateHex == 0x46){return 0x5a;}
  if(stateHex == 0x47){return 0xa0;}
  if(stateHex == 0x48){return 0x52;}
  if(stateHex == 0x49){return 0x3b;}
  if(stateHex == 0x4a){return 0xd6;}
  if(stateHex == 0x4b){return 0xb3;}
  if(stateHex == 0x4c){return 0x29;}
  if(stateHex == 0x4d){return 0xe3;}
  if(stateHex == 0x4e){return 0x2f;}
  if(stateHex == 0x4f){return 0x84;}

  if(stateHex == 0x50){return 0x53;}
  if(stateHex == 0x51){return 0xd1;}
  if(stateHex == 0x52){return 0x00;}
  if(stateHex == 0x53){return 0xed;}
  if(stateHex == 0x54){return 0x20;}
  if(stateHex == 0x55){return 0xfc;}
  if(stateHex == 0x56){return 0xb1;}
  if(stateHex == 0x57){return 0x5b;}
  if(stateHex == 0x58){return 0x6a;}
  if(stateHex == 0x59){return 0xcb;}
  if(stateHex == 0x5a){return 0xbe;}
  if(stateHex == 0x5b){return 0x39;}
  if(stateHex == 0x5c){return 0x4a;}
  if(stateHex == 0x5d){return 0x4c;}
  if(stateHex == 0x5e){return 0x58;}
  if(stateHex == 0x5f){return 0xcf;}

  if(stateHex == 0x60){return 0xd0;}
  if(stateHex == 0x61){return 0xef;}
  if(stateHex == 0x62){return 0xaa;}
  if(stateHex == 0x63){return 0xfb;}
  if(stateHex == 0x64){return 0x43;}
  if(stateHex == 0x65){return 0x4d;}
  if(stateHex == 0x66){return 0x33;}
  if(stateHex == 0x67){return 0x85;}
  if(stateHex == 0x68){return 0x45;}
  if(stateHex == 0x69){return 0xf9;}
  if(stateHex == 0x6a){return 0x02;}
  if(stateHex == 0x6b){return 0x7f;}
  if(stateHex == 0x6c){return 0x50;}
  if(stateHex == 0x6d){return 0x3c;}
  if(stateHex == 0x6e){return 0x9f;}
  if(stateHex == 0x6f){return 0xa8;}

  if(stateHex == 0x70){return 0x51;}
  if(stateHex == 0x71){return 0xa3;}
  if(stateHex == 0x72){return 0x40;}
  if(stateHex == 0x73){return 0x8f;}
  if(stateHex == 0x74){return 0x92;}
  if(stateHex == 0x75){return 0x9d;}
  if(stateHex == 0x76){return 0x38;}
  if(stateHex == 0x77){return 0xf5;}
  if(stateHex == 0x78){return 0xbc;}
  if(stateHex == 0x79){return 0xb6;}
  if(stateHex == 0x7a){return 0xda;}
  if(stateHex == 0x7b){return 0x21;}
  if(stateHex == 0x7c){return 0x10;}
  if(stateHex == 0x7d){return 0xff;}
  if(stateHex == 0x7e){return 0xf3;}
  if(stateHex == 0x7f){return 0xd2;}

  if(stateHex == 0x80){return 0xcd;}
  if(stateHex == 0x81){return 0x0c;}
  if(stateHex == 0x82){return 0x13;}
  if(stateHex == 0x83){return 0xec;}
  if(stateHex == 0x84){return 0x5f;}
  if(stateHex == 0x85){return 0x97;}
  if(stateHex == 0x86){return 0x44;}
  if(stateHex == 0x87){return 0x17;}
  if(stateHex == 0x88){return 0xc4;}
  if(stateHex == 0x89){return 0xa7;}
  if(stateHex == 0x8a){return 0x7e;}
  if(stateHex == 0x8b){return 0x3d;}
  if(stateHex == 0x8c){return 0x64;}
  if(stateHex == 0x8d){return 0x5d;}
  if(stateHex == 0x8e){return 0x19;}
  if(stateHex == 0x8f){return 0x73;}

  if(stateHex == 0x90){return 0x60;}
  if(stateHex == 0x91){return 0x81;}
  if(stateHex == 0x92){return 0x4f;}
  if(stateHex == 0x93){return 0xdc;}
  if(stateHex == 0x94){return 0x22;}
  if(stateHex == 0x95){return 0x2a;}
  if(stateHex == 0x96){return 0x90;}
  if(stateHex == 0x97){return 0x88;}
  if(stateHex == 0x98){return 0x46;}
  if(stateHex == 0x99){return 0xee;}
  if(stateHex == 0x9a){return 0xb8;}
  if(stateHex == 0x9b){return 0x14;}
  if(stateHex == 0x9c){return 0xde;}
  if(stateHex == 0x9d){return 0x5e;}
  if(stateHex == 0x9e){return 0x0b;}
  if(stateHex == 0x9f){return 0xdb;}

  if(stateHex == 0xa0){return 0xe0;}
  if(stateHex == 0xa1){return 0x32;}
  if(stateHex == 0xa2){return 0x3a;}
  if(stateHex == 0xa3){return 0x0a;}
  if(stateHex == 0xa4){return 0x49;}
  if(stateHex == 0xa5){return 0x06;}
  if(stateHex == 0xa6){return 0x24;}
  if(stateHex == 0xa7){return 0x5c;}
  if(stateHex == 0xa8){return 0xc2;}
  if(stateHex == 0xa9){return 0xd3;}
  if(stateHex == 0xaa){return 0xac;}
  if(stateHex == 0xab){return 0x62;}
  if(stateHex == 0xac){return 0x91;}
  if(stateHex == 0xad){return 0x95;}
  if(stateHex == 0xae){return 0xe4;}
  if(stateHex == 0xaf){return 0x79;}

  if(stateHex == 0xb0){return 0xe7;}
  if(stateHex == 0xb1){return 0xc8;}
  if(stateHex == 0xb2){return 0x37;}
  if(stateHex == 0xb3){return 0x6d;}
  if(stateHex == 0xb4){return 0x8d;}
  if(stateHex == 0xb5){return 0xd5;}
  if(stateHex == 0xb6){return 0x4e;}
  if(stateHex == 0xb7){return 0xa9;}
  if(stateHex == 0xb8){return 0x6c;}
  if(stateHex == 0xb9){return 0x56;}
  if(stateHex == 0xba){return 0xf4;}
  if(stateHex == 0xbb){return 0xea;}
  if(stateHex == 0xbc){return 0x65;}
  if(stateHex == 0xbd){return 0x7a;}
  if(stateHex == 0xbe){return 0xae;}
  if(stateHex == 0xbf){return 0x08;}

  if(stateHex == 0xc0){return 0xba;}
  if(stateHex == 0xc1){return 0x78;}
  if(stateHex == 0xc2){return 0x25;}
  if(stateHex == 0xc3){return 0x2e;}
  if(stateHex == 0xc4){return 0x1c;}
  if(stateHex == 0xc5){return 0xa6;}
  if(stateHex == 0xc6){return 0xb4;}
  if(stateHex == 0xc7){return 0xc6;}
  if(stateHex == 0xc8){return 0xe8;}
  if(stateHex == 0xc9){return 0xdd;}
  if(stateHex == 0xca){return 0x74;}
  if(stateHex == 0xcb){return 0x1f;}
  if(stateHex == 0xcc){return 0x4b;}
  if(stateHex == 0xcd){return 0xbd;}
  if(stateHex == 0xce){return 0x8b;}
  if(stateHex == 0xcf){return 0x8a;}

  if(stateHex == 0xd0){return 0x70;}
  if(stateHex == 0xd1){return 0x3e;}
  if(stateHex == 0xd2){return 0xb5;}
  if(stateHex == 0xd3){return 0x66;}
  if(stateHex == 0xd4){return 0x48;}
  if(stateHex == 0xd5){return 0x03;}
  if(stateHex == 0xd6){return 0xf6;}
  if(stateHex == 0xd7){return 0x0e;}
  if(stateHex == 0xd8){return 0x61;}
  if(stateHex == 0xd9){return 0x35;}
  if(stateHex == 0xda){return 0x57;}
  if(stateHex == 0xdb){return 0xb9;}
  if(stateHex == 0xdc){return 0x86;}
  if(stateHex == 0xdd){return 0xc1;}
  if(stateHex == 0xde){return 0x1d;}
  if(stateHex == 0xdf){return 0x9e;}

  if(stateHex == 0xe0){return 0xe1;}
  if(stateHex == 0xe1){return 0xf8;}
  if(stateHex == 0xe2){return 0x98;}
  if(stateHex == 0xe3){return 0x11;}
  if(stateHex == 0xe4){return 0x69;}
  if(stateHex == 0xe5){return 0xd9;}
  if(stateHex == 0xe6){return 0x8e;}
  if(stateHex == 0xe7){return 0x94;}
  if(stateHex == 0xe8){return 0x9b;}
  if(stateHex == 0xe9){return 0x1e;}
  if(stateHex == 0xea){return 0x87;}
  if(stateHex == 0xeb){return 0xe9;}
  if(stateHex == 0xec){return 0xce;}
  if(stateHex == 0xed){return 0x55;}
  if(stateHex == 0xee){return 0x28;}
  if(stateHex == 0xef){return 0xdf;}

  if(stateHex == 0xf0){return 0x8c;}
  if(stateHex == 0xf1){return 0xa1;}
  if(stateHex == 0xf2){return 0x89;}
  if(stateHex == 0xf3){return 0x0d;}
  if(stateHex == 0xf4){return 0xbf;}
  if(stateHex == 0xf5){return 0xe6;}
  if(stateHex == 0xf6){return 0x42;}
  if(stateHex == 0xf7){return 0x68;}
  if(stateHex == 0xf8){return 0x41;}
  if(stateHex == 0xf9){return 0x99;}
  if(stateHex == 0xfa){return 0x2d;}
  if(stateHex == 0xfb){return 0x0f;}
  if(stateHex == 0xfc){return 0xb0;}
  if(stateHex == 0xfd){return 0x54;}
  if(stateHex == 0xfe){return 0xbb;}
  if(stateHex == 0xff){return 0x16;}
}
