void registerToNetwork()
{

  // Register to nearest network
  Serial.println("Registering . . . .");

  // send register request to network
  LoRa.beginPacket();
  String recipient = endNetworkCandidate;
  String askToRegisterMessage = "02" + parser + nodeID + parser + recipient + parser + networkCandidate;
  LoRa.print(askToRegisterMessage);
  LoRa.endPacket();

  // waiting for answer
  String networkMessage = "";
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
      if(messageIsForMe(networkMessage) == true && parsing(networkMessage, '|', 0) == "03")
      {
        confirmation = true;
        isRegistered = true;
        
        // save network ID
        network.id = parsing(networkMessage, '|', 3);

        Serial.print("Node is registered to ");
        Serial.print(network.id);
        Serial.print(" via ");
        Serial.println(endNetworkCandidate);
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
      isRegistered = false;
    }
  }
  // ============== End of While ========================
}
