void copyRoutingTable(String network_id, String routingRecipient)
{
  // Get routing table id from recipient
  bool askRoutingTable = false;

  LoRa.beginPacket();
  LoRa.print("16" + parser + nodeID + parser + routingRecipient + parser + network_id);
  LoRa.endPacket();

  unsigned long previousMillis = 0;
  bool confirmation = false;
  String networkMessage = "";

  while(confirmation == false)
  {
    int packetSize = LoRa.parsePacket();

    if (packetSize)
    {
      while (LoRa.available())
      {
        networkMessage += (char)LoRa.read();
      }

      // if message is for this node and message code is right
      if(messageIsForMe(networkMessage) == true && parsing(networkMessage, '|', 0) == "17")
      {
        confirmation = true;

        if(network.routingID < parsing(networkMessage, '|', 4).toInt())
        {
          askRoutingTable = true;
        }
      }

      else
      {
        networkMessage = "";
        confirmation = false;
      }

    }      
    // End connection if timeout
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 6000) {
      previousMillis = currentMillis;
      confirmation = true;
    }
  }

  if(askRoutingTable == true)
  {
    for(int i=0; i<network.maxMember; i++)
    {
      String message = "12" + parser + nodeID + parser + routingRecipient + parser + network_id + parser + String(i);

      LoRa.beginPacket();
      LoRa.print(message);
      LoRa.endPacket();

      // waiting for answer
      networkMessage = "";
      unsigned long previousMillis = 0;
      bool confirmation = false;

      // ======================Start While==========================
      // Receiver mode until get response or timeout
      while(confirmation == false)
      {
        int packetSize = LoRa.parsePacket();

        if (packetSize)
        {
          while (LoRa.available())
          {
            networkMessage += (char)LoRa.read();
          }

          // if message is for this node and message code is right
          if(messageIsForMe(networkMessage) == true && parsing(networkMessage, '|', 0) == "13")
          {
            confirmation = true;
            network.networkMember[i] = parsing(networkMessage, '|', 4);
            network.memberNeighbour[i] = parsing(networkMessage, '|', 5);
          }

          else
          {
            networkMessage = "";
            confirmation = false;
          }

        }      
        // End connection if timeout
        unsigned long currentMillis = millis();

        if (currentMillis - previousMillis >= 6000) {
          previousMillis = currentMillis;
          confirmation = true;
        }
      }
      // ============== End of While ========================
    }
    
    Serial.print("Routing table copied from ");
    Serial.println(routingRecipient);

    routingTable();
  }

  if(askRoutingTable == false)
  {
    Serial.println("Not copying because id target's routing id is older or target not connected");
  }
}
