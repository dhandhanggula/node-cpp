//====================================================================
//====================================================================
// Fuction to send ping message to targeted node
//
// return 0 if can't connect
// return 1 if connected
//====================================================================
//====================================================================

bool ping(String destinationPing, String routePathPing)
{
  String msgCode = code("ping");
  DateTime now = rtc.now();
  String msgID = String(now.unixtime(), HEX); 

  if(lastMsgID != msgID)
  {
    Serial.print(F("Ping to "));
    Serial.print(destinationPing);
    Serial.print(F(". "));

    // Send ping msg
    LoRa.beginPacket();
    String sentMsg = msgCode + parser + msgID + parser + nodeID + parser + destinationPing + parser + routePathPing + parser + "99";
    LoRa.print(sentMsg);
    LoRa.endPacket();

    lastMsgID = msgID;
    //Serial.println(sentMsg);

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

        //Serial.println(receivedMsg);
        tempPingMillis = millis();
      }
          
      // End connection if timeout
      unsigned long currentMillis = millis();

      if(currentMillis - prevMillis > 6000) 
      {
        prevMillis = currentMillis;
        Serial.println(F("Request Time Out"));
        waitAnswer = false;
        //return false;
      }

      // authentication
      if(isForMe(receivedMsg) == true && isFromSender(receivedMsg, destinationPing) == true && isCodeRight(receivedMsg, code("ansPing")) == true)
      {
        waitAnswer = false;
        Serial.print(F("Connected with time "));
        Serial.print(tempPingMillis - pingMillis);
        Serial.println(F("ms"));
        //return true;
      }

    }
    // ============== End of While ========================
  }
}

