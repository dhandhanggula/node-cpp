void ping(route destinationRoute)
{
  String destination = destinationRoute.destination;
  Serial.print("Ping to ");
  Serial.println(destination);

  // Send ping msg
  LoRa.beginPacket();
  String pingMsg[] = {"00", "zz", nodeID, destination, "routePath", "99"};
  LoRa.print(createMsg(pingMsg));
  LoRa.endPacket();

  // Wait for the answer
  long int timeout = 6000;        // set timeout to 6 second
  unsigned long prevMillis = 0;
  bool waitAnswer = true;
  String receivedMsg = "";

  // ======================Start While==========================
  // Receiver mode until get response or timeout
  while(waitAnswer == true)
  {
    // Get packet
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
      while (LoRa.available())
      {
        receivedMsg += (char)LoRa.read();
      }
    }

    // authentication
    if(isForMe(receivedMsg) == true && isFromSender(receivedMsg, destination) == true && isCodeRight(receivedMsg, "00") == true)
    {
      waitAnswer = false;
      Serial.println("Connected to destination");
    }
        
    // End connection if timeout
    unsigned long currentMillis = millis();

    if (currentMillis - prevMillis >= timeout) {
      prevMillis = currentMillis;
      Serial.println("Can't connect to destination");
      waitAnswer = false;
    }
  }
  // ============== End of While ========================

}

