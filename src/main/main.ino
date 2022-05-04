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

//====================================================================
// Mesh Route ========================================================
//====================================================================

struct networkProperties{
  String id;
  String networkMember[30];
  String memberNeighbour[30];
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

  if(registerStatus == 1)
  {
    // Ask for routing table
  }

  if(registerStatus == 2)
  {
    // Create routing table
    network.networkMember[0] = nodeID;
    printRoutingTable();
  }

}

void loop()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize) {

    // read packet
    while (LoRa.available()) {
      messageReceived += (char)LoRa.read();
    }

    // print RSSI of packet
    Serial.println(messageReceived);
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

  }
}


//====================================================================
// Critical Function =================================================
//====================================================================

void printRoutingTable()
{
  Serial.println("\nRouting Table :");

  for(int i=0; i<30; i++)
  {
    String table = String(i) + 
                    "      " + 
                    String(network.networkMember[i]) + 
                    "     " + 
                    String(network.memberNeighbour[i]);

    Serial.println(table);
  }

}