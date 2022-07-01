
String routeDiscovery(String destinationID)
{
  // SEND RREQ
  String msgCode = code("rreq");
  DateTime now = rtc.now();
  String msgID = String(now.unixtime(), HEX); 

  String path = "0";
  Serial.print("Send RREQ to ");
  Serial.print(destinationID);
  Serial.println(".");
  parser = "|";

  // Send rreq msg
  LoRa.beginPacket();
  String sentMsg = msgCode + parser + msgID + parser + nodeID + parser + destinationID + parser + path + parser + "99";
  Serial.println(sentMsg);
  LoRa.print(sentMsg);
  LoRa.endPacket();

  // Wait for the answer
  bool waitAnswer = true;
  
  String receivedMsg = "";
  pingMillis = millis();
  prevMillis = millis();
  unsigned long tempPingMillis;

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

      Serial.print("Received : ");
      Serial.println(receivedMsg);
      tempPingMillis = millis();
    }
        
    // End connection if timeout
    unsigned long currentMillis = millis();

    if(currentMillis - prevMillis > 6000) 
    {
      prevMillis = currentMillis;
      Serial.println("Error. Code 408");
      Serial.println("Request Time Out");
      waitAnswer = false;
      return "404";
    }

    // authentication
    if(isForMe(receivedMsg) == true && isFromSender(receivedMsg, destinationID) == true && isCodeRight(receivedMsg, code("rrep")) == true)
    {
      waitAnswer = false;
      
      String getPath = parsing(receivedMsg, '|', 5);
      Serial.print("Get link to ");
      Serial.print(destinationID);
      Serial.print(" via ");
      Serial.println(getPath);

      lastDestination = destinationID;
      lastRoute = getPath;

      return "201";
    }
  }
  // ============== End of While ========================
}