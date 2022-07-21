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
String messageSent = "";
String payload = "";                    // payload only

unsigned long prevMillis = 0;           // to save latest millis
unsigned long pingMillis = 0;           // to save latest ping millis

String lastMsgID = "";
unsigned long msgMillis = 0;

//====================================================================
// Other(s) ==========================================================
//====================================================================

void setup()
{
  // Reading the credentials saved in EEPROM
  for(int i=0; i<6; i++)
  {
    nodeID += (char)EEPROM.read(i);
  }

  Serial.begin(115200);

  // Activate LoRa
  Serial.println(F("========================================"));
  Serial.print(F("Node with ID "));
  Serial.print(nodeID);
  Serial.println(F(" is active"));
  Serial.println(F("========================================"));

  if (!LoRa.begin(networkFreq)) {
    Serial.println(F("Starting LoRa failed!"));
    while (1);
  }

  Serial.print(F("Frequency : "));
  Serial.println(networkFreq);

  LoRa.setTxPower(txPower);
  Serial.print(F("Transmitter power (dB) : "));
  Serial.println(txPower);

  LoRa.setSpreadingFactor(spreadingFactor);
  Serial.print(F("Spreading Factor : "));
  Serial.println(spreadingFactor);

  LoRa.setSignalBandwidth(signalBandwidth);
  Serial.print(F("Signal Bandwidth : "));
  Serial.println(signalBandwidth);

  LoRa.setSyncWord(networkSync);
  Serial.print(F("Network Sync Word : "));
  Serial.println(networkSync, HEX);

  Serial.println(F("Node is activated successfully ========="));
  Serial.println(F("========================================"));
  Serial.println("\n");

  rtc.begin();
  rtc.adjust(DateTime(2022, 1, 1, 0, 0, 0));
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
    Serial.print(messageReceived);
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }

}
