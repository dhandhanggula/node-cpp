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
#include <EEPROM.h>

//====================================================================
// LoRa Node Configuration ===========================================
//====================================================================

#define txPower 17
#define LNAgain 0                     // 0 - 6
String nodeID = "";         // 

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

//====================================================================
// Mesh Route ========================================================
//====================================================================

struct networkProperties{
  String id;
  String networkMember[30];
  String memberNeighbour[30];
  int maxMember = 30;
};

networkProperties network;

//====================================================================
// Other(s) ==========================================================
//====================================================================

String parser = "|";

int registerStatus = 0;

String messageReceived = "";
String messageSent = "";



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

  // Network search
  String networkCandidate = "";
  String endNetworkCandidate = "";
  bool getNetwork = false;
  bool isRegistered = false;

  networkSearch();

  if(getNetwork == true)
  {
    Serial.print("\nGet network candidate with ID ");
    Serial.println(networkCandidate);
    Serial.print("End node ID ");
    Serial.println(endNetworkCandidate);

    registerToNetwork();

    if(isRegistered == true)
    {

      // copy routing table
      Serial.print("\nCopy Routing Table from ");
      Serial.println(endNetworkCandidate);
      
      copyRoutingTable(networkProperties.id, endNetworkCandidate);
    }

    if(isRegistered == false)
    {
      Serial.print("\nError : Can't join network ");
      Serial.println(networkCandidate);
      
      delay(1000);

      // create network
      getNetwork = false;
    }
  }

  if(getNetwork == false)
  {
    Serial.println("Network not detected");
    Serial.println("Preparing to create new network\n");

    delay(1000);
    createNetwork();
    routingTable();
  }

}

void loop()
{
  int packetSize = LoRa.parsePacket();
  messageReceived = "";

  if (packetSize) {

    // read packet
    while (LoRa.available()) {
      messageReceived += (char)LoRa.read();
    }

    // print RSSI of packet
    Serial.print(messageReceived);
    Serial.print(" with RSSI ");
    Serial.println(LoRa.packetRssi());

  }

  if (messageReceived != "")
  {
    protocol_0();
    protocol_1();
    protocol_2();
  }

}


//====================================================================
// Critical Function =================================================
//====================================================================

// to authenticate
bool messageIsForMe(String message)
{
  if(parsing(message, '|', 2) == nodeID)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool isMyNeighbour()
{
  bool myNeighbour = false;

  for(int i=0; i<network.maxMember; i++)
  {
    if(parsing(messageReceived, '|', 3) == network.id)
    {
      myNeighbour = true;
    }
  }

  return myNeighbour;
}