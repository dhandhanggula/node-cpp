
String routeDiscovery(String destinationID)
{
  // SEND RREQ
  String msgCode = code("rreq");
  DateTime now = rtc.now();
  String msgID = String(now.unixtime(), HEX); 

  String path = "0";
  Serial.print(F("Send RREQ to "));
  Serial.print(destinationID);
  Serial.println(F("."));
  parser = "|";

  // Send rreq msg
  LoRa.beginPacket();
  String sentMsg = msgCode + parser + msgID + parser + nodeID + parser + destinationID + parser + path + parser + "99";
  Serial.println(sentMsg);
  LoRa.print(sentMsg);
  LoRa.endPacket(true);

  updateMsgHistory(msgID);

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
    receivedMsg = "";

    // Get packet
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
      while (LoRa.available())
      {
        receivedMsg += (char)LoRa.read();
      }

      tempPingMillis = millis();
      //Serial.print(F("Received : "));
      //Serial.println(receivedMsg);
      
      if(isForMe(receivedMsg) == true && isCodeRight(receivedMsg, code("rrep")) == true)
      {
        waitAnswer = false;
        
        String getPath = parsing(receivedMsg, '|', 5);
        Serial.print(F("Get link to "));
        Serial.print(destinationID);
        Serial.print(F(" via "));
        Serial.println(getPath);

        lastDestination = destinationID;
        lastRoute = getPath;

        return "201";
      }
    }
        
    // End connection if timeout
    unsigned long currentMillis = millis();

    if(currentMillis - prevMillis > 6000) 
    {
      prevMillis = currentMillis;
      Serial.println(F("Error. Code 408"));
      Serial.println(F("Request Time Out"));
      waitAnswer = false;
      return "404";
    }

    
    // authentication
    

  }
  // ============== End of While ========================
}