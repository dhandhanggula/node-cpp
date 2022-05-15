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

  while(networkConfirmation == false)
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
      if(messageIsForMe(networkMessage) && parsing(networkMessage, '|', 0) == "01")
      {
        networkConfirmation = true;
        if(parsing(networkMessage, '|', 4).toInt() < networkProperties.maxMember)
        {
          getNetwork = true;

          networkCandidate = parsing(networkMessage, '|', 3);
          endNetworkCandidate = parsing(networkMessage, '|', 1);
        }
      }

      // if code is not 01, wait again
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