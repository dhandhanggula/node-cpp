void registerToNetwork()
{
  Serial.println("========================================");
  Serial.println("Searching Nearest Network ==============");

  //======================= "00" Code ==========================
  // Broadcast and wait for confirmation
  bool networkConfirmation;

  LoRa.beginPacket();
  LoRa.print("00");
  LoRa.endPacket();
  bool sendRegisterMessage = true;

  Serial.println("Waiting for nearest network confirmation");
  //===================== End "00" Code ========================

  String networkMessage = "";
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
          // if code is 01, get network confirmation
          // end register status
          if(parsing(networkMessage, '|', 0) == "01")
          {
            networkConfirmation = true;
            sendRegisterMessage = false;
          }

          // if code is not 01, wait again
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

  // Process network confirmation
  if(networkConfirmation == true)
  {
    // Register to nearest network
    Serial.println("Network detected =======================");
    Serial.println("========================================");

    // parsing data receive
    if (parsing(networkMessage, '|', 1)) {
      network.id = parsing(networkMessage, '|', 1);
    }

    // send register request to network
    LoRa.beginPacket();
    String askToRegisterMessage = "02" + parser + nodeID + parser + network.id;
    LoRa.print(askToRegisterMessage);
    LoRa.endPacket();

    // Register to network
    networkMessage = "";
    bool sendRegisterMessage = true;
    Serial.println("Registering to network " + network.id);

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
                parsing(networkMessage, '|', 2) == network.id)
            {
              sendRegisterMessage = false;
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

  if(networkConfirmation == true)
  {
    Serial.println("Network detected =======================");
    Serial.println("Device is registered to Network " + network.id);
    Serial.println("========================================");

    delay(1000);
    registerStatus = 1;
  }

}