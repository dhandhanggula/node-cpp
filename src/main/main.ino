//====================================================================
//====================================================================
// Dhandhanggula Node
// Written in C++ Arduino
//
// MIT License
// For more information : https://github.com/dhandhanggula/node-cpp
//====================================================================


//====================================================================
// Library(s) ========================================================
//====================================================================

#include <SPI.h>
#include <LoRa.h>

//====================================================================
// LoRa Node Configuration ===========================================
//====================================================================

#define txPower 17
#define LNAgain 0                     // 0 - 6
String nodeID = "62698410GC";         // 

//====================================================================
// Network Configuration =============================================
//====================================================================

#define networkFreq 915E6             // 433E6, 868E6, 915E6
#define networkSync 0x12              // 0x00 - 0xFF
#define spreadingFactor 8             // 6 - 12
#define signalBandwidth 125E3         // 7.8E3, 10.4E3, 15.6E3, 
                                      // 20.8E3, 31.25E3, 41.7E3, 
                                      // 62.5E3, 125E3, 250E3, 500E3
#define codingRateDen 5               // 5 - 8 => default 5 (4/5)
String networkID = "";

//====================================================================
// Mesh Route ========================================================
//====================================================================

void setup()
{
  Serial.begin(9600);

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

  // Register to Network
  registerToNetwork();
}

void loop()
{

}

void registerToNetwork()
{
  Serial.println("========================================");
  Serial.println("Searching Nearest Network ==============");

  // Broadcast and wait for confirmation
  bool networkConfirmation;

  LoRa.beginPacket();
  Lora.print("reg");
  LoRa.endPacket();

  bool sendRegisterMessage = true;

  while(sendRegisterMessage = true)
  {
    int packetSize = LoRa.parsePacket();
    String networkMessage = "";
    
    if (packetSize)
    {
      Serial.println("Read Network Confirmation");

      while (LoRa.available())
      {
        networkMessage += (char)LoRa.read();

        if(!LoRa.available())
        {
          sendRegisterMessage = false;
        }
      }
    }
  }

  if(networkConfirmation == true)
  {
    // Register to nearest network
    Serial.println("Network detected =======================");
    networkID = "";

  }

  if(networkConfirmation == false)
  {
    // Register to nearest network
    Serial.println("Network not detected ===================");
  }

}