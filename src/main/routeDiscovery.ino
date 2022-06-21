
int routeDiscovery(String destinationRoute)
{
  // SEND RREQ
  String msgCode = code("rreq");
  DateTime now = rtc.now();
  String msgID = String(now.unixtime(), HEX); 

  String path = "0";
  Serial.print("Send RREQ to ");
  Serial.print(destinationRoute);
  Serial.println(".");
  parser = "|";

  // Send ping msg
  LoRa.beginPacket();
  String sentMsg = msgCode + parser + msgID + parser + nodeID + parser + destinationRoute + parser + path + parser + "99";
  //Serial.println(sentMsg);
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

      Serial.println(receivedMsg);
      tempPingMillis = millis();
    }
        
    // End connection if timeout
    unsigne\d long currentMillis = millis();

    if(currentMillis - prevMillis > 6000) 
    {
      prevMillis = currentMillis;
      Serial.println("Error. Code 408");
      Serial.println("Request Time Out");
      waitAnswer = false;
      return 404;
    }

    // authentication
    if(isForMe(receivedMsg) == true && isFromSender(receivedMsg, destinationRoute) == true && isCodeRight(receivedMsg, code("rrep")) == true)
    {
      waitAnswer = false;
      Serial.print("Get link to ");
      Serial.println(destinationRoute);

      for(int i=0; i<20; i++)
      {
        if(destinationBook[i] == destinationRoute)
        {
          routeBook[i] = parsing(receivedMsg, '|', 5);
          return 201;
        }
      }

      // create in bookmark
      for(int i = 0; i<20; i++)
      {
        // save as new route path
        if(destinationBook[i] == "")
        {
          destinationBook[i] = destinationRoute;
          routeBook[i] = parsing(receivedMsg, '|', 5);
          Serial.println(routeBook[i]);
          return 201;
        }
      }
    }
  }
  // ============== End of While ========================
}