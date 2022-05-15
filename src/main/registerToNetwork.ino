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

      // if code is 01, get network confirmation
      // end register status
      if(messageIsForMe(networkMessage) && parsing(networkMessage, '|', 0) == "03")
      {
        confirmation = true;
        isRegistered = true;
        
        networkProperties.id = parsing(networkMessage, '|', 3);
        Serial.print("Node is registered to ");
        Serial.print(networkProperties.id);
        Serial.print(" via ");
        Serial.println(endNetworkCandidate);
      }

      // if code is not 01, wait again
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