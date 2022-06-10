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
  String relayNode[10];
};

route routeToDestination;

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

//====================================================================
// Other(s) ==========================================================
//====================================================================

void setup()
{
  // Reading the credentials saved in EEPROM
  readNodeCredentials();

  Serial.begin(9600);

  // Activate LoRa
  Serial.println("========================================");
  Serial.print("Node with ID ");
  Serial.print(nodeID);
  Serial.println(" is active");
  Serial.println("========================================");

  if (!LoRa.begin(networkFreq)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.print("Frequency : ");
  Serial.println(networkFreq);

  LoRa.setTxPower(txPower);
  Serial.print("Transmitter power (dB) : ");
  Serial.println(txPower);

  LoRa.setSpreadingFactor(spreadingFactor);
  Serial.print("Spreading Factor : ");
  Serial.println(spreadingFactor);

  LoRa.setSignalBandwidth(signalBandwidth);
  Serial.print("Signal Bandwidth : ");
  Serial.println(signalBandwidth);

  LoRa.setSyncWord(networkSync);
  Serial.print("Network Sync Word : ");
  Serial.println(networkSync, HEX);

  Serial.println("Node is activated successfully =========");
  Serial.println("========================================");
  Serial.println(" ");
}

void loop()
{
  int packetSize = LoRa.parsePacket();
  messageReceived = "";

  // Read received packet
  if (packetSize) {
    while (LoRa.available()) {
      messageReceived += (char)LoRa.read();
    }
  }

  // Answer if needed
  if (messageReceived != "")
  {
    //answer protocol here
  }

}
