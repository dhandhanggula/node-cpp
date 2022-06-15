//====================================================================
//====================================================================
// Fuction to send ping message to targeted node
//
// return 0 if can't connect
// return 1 if connected
//====================================================================
//====================================================================

bool ping(route destinationRoute)
{
  // change this code later
  String msgCode = code("ping");
  DateTime now = rtc.now();
  String msgID = String(now.unixtime(), HEX); 

  String destination = destinationRoute.destination;
  String path = destinationRoute.routePath;
  Serial.print("Ping to ");
  Serial.print(destination);
  Serial.print(". ");

  // Send ping msg
  LoRa.beginPacket();
  String sentMsg = msgCode + parser + msgID + parser + nodeID + parser + destination + parser + path + parser + "99";
  LoRa.print(sentMsg);
  LoRa.endPacket();

  // Wait for the answer
  bool waitAnswer = true;
  
  String receivedMsg = "";
  pingMillis = millis();
  prevMillis = millis();

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
        
    // End connection if timeout
    unsigned long currentMillis = millis();

    if(currentMillis - prevMillis > 6000) 
    {
      prevMillis = currentMillis;
      Serial.println("Request Time Out");
      waitAnswer = false;
      return false;
    }

    // authentication
    if(isForMe(receivedMsg) == true && isFromSender(receivedMsg, destination) == true && isCodeRight(receivedMsg, code("ansPing")) == true)
    {
      waitAnswer = false;
      Serial.print("Connected with time ");
      Serial.print(millis() - pingMillis);
      Serial.println("ms");
      return true;
    }
  }
  // ============== End of While ========================
}

