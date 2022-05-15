void networkSearch()
{
  Serial.println("========================================");
  Serial.println("Searching Nearest Network ==============");

  //======================= "00" Code ==========================
  LoRa.beginPacket();
  LoRa.print("00");
  LoRa.endPacket();

  Serial.println("Waiting for nearest network confirmation");
  //===================== End "00" Code ========================



  // ======================Start While==========================
  String networkMessage = "";
  unsigned long previousMillis = 0;
  bool networkConfirmation = false;

  // Receiver mode until get response or timeout
  while(networkConfirmation == false)
  {
    int packetSize = LoRa.parsePacket();

    // if data received
    if (packetSize)
    {
      while (LoRa.available())
      {
        networkMessage += (char)LoRa.read();
      }

      // if message is for this node and message code is right
      if(messageIsForMe(networkMessage) == true && parsing(networkMessage, '|', 0) == "01")
      {
        networkConfirmation = true;

        // if network is not full
        if((parsing(networkMessage, '|', 4).toInt()) < network.maxMember)
        {
          getNetwork = true;

          networkCandidate = parsing(networkMessage, '|', 3);
          endNetworkCandidate = parsing(networkMessage, '|', 1);
        }
      }

      else
      {
        networkMessage = "";
        networkConfirmation = false;
      }

    }

    // End connection if timeout
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 6000) {
      previousMillis = currentMillis;
      networkConfirmation = true;
      getNetwork = false;
    }
  }
  // ============== End of While ========================

}
