//====================================================================
//====================================================================
// Dhandhanggula Node
// Written in C++ Arduino
// Mesh Routing Protocol : Dynamic Source Routing
//
// MIT License
// For more information : https://github.com/dhandhanggula/node-cpp
//====================================================================


//====================================================================
// Library(s) ========================================================
//====================================================================

#include <SPI.h>
#include <LoRa.h>
#include <EEPROM.h>

#include <Wire.h>
#include "RTClib.h"

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
#define spreadingFactor 8             // 6 - 12
#define signalBandwidth 125E3         // 7.8E3, 10.4E3, 15.6E3, 
                                      // 20.8E3, 31.25E3, 41.7E3, 
                                      // 62.5E3, 125E3, 250E3, 500E3
#define codingRateDen 5               // 5 - 8 => default 5 (4/5)

//====================================================================
// Mesh Route ========================================================
//====================================================================

struct route{
  String destination;
  String routePath;
};

//====================================================================
// R T C =============================================================
//====================================================================

RTC_DS1307 rtc;

//====================================================================
// Message Config ====================================================
// Message Structure
// MsgCode | MsgID | Sender | Destination | Route Path | Payload
//====================================================================
int registerStatus = 0;

String parser = "|";
String messageReceived = "";
String messageSent = "";
String payload = "";                    // payload only

unsigned long prevMillis = 0;
unsigned long pingMillis = 0;


String lastMsgID = "";
unsigned long msgMillis = 0;
int lastMsgCode = 404;

String pingID = "";

route toTested[10];

//====================================================================
// Other(s) ==========================================================
//====================================================================

void setup()
{
  // Refer to setup.ino file
  setupNode();
toTested[0].destination = "YrCGaw";
toTested[0].routePath = "YrCGaw";
}

void loop()
{
  int packetSize = LoRa.parsePacket();
  messageReceived = "";

  // Read received packet
  if (packetSize) {;
    while (LoRa.available()) {
      messageReceived += (char)LoRa.read();
    }

    Serial.println(messageReceived);
  }

  // Answer if needed
  if (messageReceived != "")
  {
    answer(messageReceived);
  }

  // Test to ping 
  //ping(toTested);

  // read serial command
  String serialMsg = "";
  if(Serial.available())
  {
    delay(100);
    while(Serial.available() > 0)
    {
      serialMsg += (char)Serial.read();
    }
    Serial.flush();
  }

  if(serialMsg != "")
  {
    uartcom(serialMsg);
  }

}
