void copyRoutingTable(String network, String routingRecipient)
{
  for(int i=0; i<networkProperties.maxMember; i++)
  {
    String message = "12" + parser + nodeID + parser + routingRecipient + parser + 
                      network + parser + i;

    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket();

    // waiting for answer
    networkMessage = "";
    unsigned long previousMillis = 0
    bool confirmation = false;

    // ======================Start While==========================
    while(confirmation == false)
    {
      int packetSize = LoRa.parsePacket();

      if (packetSize)
      {
        while (LoRa.available())
        {
          networkMessage += (char)LoRa.read();
        }

        if(messageIsForMe(networkMessage) && parsing(networkMessage, '|', 0) == "13")
        {
          confirmation = true;
          networkProperties.networkMember[i] = parsing(networkMessage, '|', 4);
          networkProperties.memberNeighbour[i] = parsing(networkMessage, '|', 5);
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

}