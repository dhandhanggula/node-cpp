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

}

void registerToNetwork()
{
  Serial.println("========================================");
  Serial.println("Searching Nearest Network ==============");

  // Broadcast and wait for confirmation
  bool networkConfirmation;

  LoRa.beginPacket();
  LoRa.print("00");
  LoRa.print(parser);
  LoRa.print(nodeID);
  LoRa.endPacket();

  bool sendRegisterMessage = true;
  String networkMessage = "";

  Serial.println("Waiting for nearest network confirmation");

  unsigned long previousMillis = 0;

  while(sendRegisterMessage == true)
  {
    int packetSize = LoRa.parsePacket();
    
    if (packetSize)
    {

      while (LoRa.available())
      {
        networkMessage += (char)LoRa.read();

        if(!LoRa.available())
        {
          if(parsing(networkMessage, '|', 0) == "01")
          {
            networkConfirmation = true;
            sendRegisterMessage = false;
          }

          if(parsing(networkMessage, '|', 0) != "01")
          {
            sendRegisterMessage = true;
            networkMessage = "";
          }
        }
      }
    }

    // End connection if timeout
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 10000) {
      previousMillis = currentMillis;
      sendRegisterMessage = false;
      networkConfirmation = false;
    }
  }

  if(networkConfirmation == true)
  {
    // Register to nearest network
    Serial.println("Network detected =======================");
    Serial.println("========================================");

    // parsing data receive
    network.id = parsing(networkMessage, '|', 1);

    // send registering request to network
    LoRa.beginPacket();
    String askToRegisterMessage = "02" + parser + nodeID + parser + network.id;
    LoRa.print(askToRegisterMessage);
    LoRa.endPacket();

    // Register to network
    networkMessage = "";
    bool sendRegisterMessage = true;
    Serial.println("Registering to network" + network.id);

    while(sendRegisterMessage == true)
    {
      int packetSize = LoRa.parsePacket();
      
      if (packetSize)
      {

        while (LoRa.available())
        {
          networkMessage += (char)LoRa.read();

          if(!LoRa.available())
          {
            if(parsing(networkMessage, '|', 0) == "03" && 
                parsing(networkMessage, '|', 1) == nodeID && 
                parsing(networkMessage, '|', 2) == network.id && 
                parsing(networkMessage, '|', 3) == "1")
            {
              sendRegisterMessage = false;
              registerStatus = 1;

              Serial.println("\n========================================");
              Serial.println("Registered to Network " + network.id);
              Serial.println("========================================");
            }

            else
            {
              sendRegisterMessage = true;
              networkMessage = "";
            }
          }
        }
        
        // End connection if timeout
        unsigned long currentMillis = millis();

        if (currentMillis - previousMillis >= 10000) {
          previousMillis = currentMillis;
          sendRegisterMessage = false;
          networkConfirmation = false;
        }
      }
    }
  }

  if(networkConfirmation == false)
  {
    Serial.println("Network not detected ===================");
    Serial.println("========================================");

    delay(1000);

    // Create network
    randomSeed(analogRead(0));
    long randNumber = random(100000, 999999);
    network.id = String(randNumber);
    Serial.println("\n========================================");
    Serial.println("Create Network with ID " + network.id);
    Serial.println("========================================");

    registerStatus = 2;
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

String parsing(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  } 
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}