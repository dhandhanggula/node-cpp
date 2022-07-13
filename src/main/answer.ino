//====================================================================
//====================================================================
// Fuction to answer message
//
// List :
// - Ping 
//====================================================================
//====================================================================

void answer(String message)
{
  // check if message is null, return to main loop
  if(message == ""){return; }

  // check if message header is broken, return to main loop
  if(precheckmsg(message) == false){return; }

  // if header is OK, do next
  String getMsgCode = parsing(message, '|', 0);
  String getMsgID = parsing(message, '|', 1);
  String getMsgSender = parsing(message, '|', 2);
  String getMsgRecipient = parsing(message, '|', 3);
  String getMsgPath = parsing(message, '|', 4);
  String getMsgPayload = parsing(message, '|', 5);

  String sendPath = "";
  String path[] = {""};

  // Get routing path
  uint8_t pathLength = charCount(getMsgPath, ',');

  if(pathLength == 0)
  {
    path[0] = getMsgSender;
    sendPath = getMsgSender;
  }

  if(pathLength != 0)
  {
    // save route path to array
    for(int i=0; i < pathLength; i++)
    {
      path[i] = parsing(getMsgPath, ',', i);
    }

    // create reverse route path back to sender
    for(int i=0; i < pathLength; i++)
    {
      if(path[pathLength - (i + 1)] != "")
      {
        sendPath += path[pathLength - i] + ",";
      }
    }

    // Add destination to last reverse route path
    sendPath = sendPath + getMsgSender;
    
    int sendPathLength = sendPath.length();
    String lastChar = String(sendPath.charAt(sendPathLength - 1));
    if(lastChar == ",")
    {
      sendPath.remove(sendPathLength - 1);
    }
  }

  int randomDelay = random(100, 500);



  // Check getMsgRecipient
  if(isForMe(message) == false && isInMsgIDHistory(getMsgID) == false)
  {
    // if rreq
    if(getMsgCode == code("rreq"))
    {
      if(getMsgPath != "0")
      {
        getMsgPath = getMsgPath + "," + nodeID;
      }
      if(getMsgPath == "0")
      {
        getMsgPath = nodeID;
      }

      // to prevent incoming message at the same time
      delay(randomDelay);

      // relay msg
      String sentMsg = getMsgCode + parser + getMsgID + parser + getMsgSender + parser + getMsgRecipient + parser + getMsgPath + parser + getMsgPayload;

      LoRa.beginPacket();
      LoRa.print(sentMsg);
      LoRa.endPacket(true);

      Serial.print(F("Relaying RREQ Message "));
      Serial.println(getMsgID);
      Serial.print(F("From : "));
      Serial.println(message);
      Serial.print(F("With : "));
      Serial.println(sentMsg);

      updateMsgHistory(getMsgID);

      return;
    }

    // if rerr
    if(getMsgCode == code("rerr"))
    {
      //do here
      delay(randomDelay);

      //relay msg
      String sentMsg = getMsgCode + parser + getMsgID + parser + getMsgSender + parser + getMsgRecipient + parser + getMsgPath + parser + getMsgPayload;
      
      LoRa.beginPacket();
      LoRa.print(sentMsg);
      LoRa.endPacket(true);

      Serial.print(F("Relaying RERR Message "));
      Serial.println(getMsgID);
      Serial.print(F("From : "));
      Serial.println(message);
      Serial.print(F("With : "));
      Serial.println(sentMsg);

      updateMsgHistory(getMsgID);

      return;
    }

    // if other
    for(int i=0; i<=pathLength; i++)
    {
      if(path[i] == nodeID)
      {
        delay(randomDelay);
        
        // Relay message if this node is in routing path
        String sentMsg = getMsgCode + parser + getMsgID + parser + getMsgSender + parser + getMsgRecipient + parser + getMsgPath + parser + getMsgPayload;
        
        LoRa.beginPacket();
        LoRa.print(sentMsg);
        LoRa.endPacket(true);

        Serial.print(F("Relaying General Message "));
        Serial.println(getMsgID);
        Serial.print(F("From : "));
        Serial.println(message);
        Serial.print(F("With : "));
        Serial.println(sentMsg);
        
        updateMsgHistory(getMsgID);

        return;
      }
    }
  }

  if(isForMe(message) == true && isInMsgIDHistory(getMsgID) == false)
  {
    
    //================================================================
    // Code OO ==> PING ==============================================
    //================================================================

    if(getMsgCode == code("ping"))
    {
      // send answer

      String msgCode = code("ansPing");
      String msgID = getMsgID;
      String destination = getMsgSender;
      String path = sendPath;
      String sentMsg = msgCode + parser + msgID + parser + nodeID + parser + destination + parser + path + parser + "99";

      LoRa.beginPacket();
      LoRa.print(sentMsg);
      LoRa.endPacket(true);

      Serial.print(F("Answer Ping Message "));
      Serial.println(getMsgID);
      Serial.print(F("From : "));
      Serial.println(message);
      Serial.print(F("With : "));
      Serial.println(sentMsg);

      return;
    }

    //====================================================================
    // Code 20 ==> RREQ ==================================================
    //====================================================================
    if(getMsgCode == code("rreq"))
    {
      // Send RREP
      String msgCode = code("rrep");
      String msgID = getMsgID;
      String destination = getMsgSender;
      String path = sendPath;
      String payload;
      if(getMsgPath != "0")
      {
        payload = getMsgPath + "," + nodeID;
      }
      if(getMsgPath == "0")
      {
        payload = nodeID;
      }
      String sentMsg = msgCode + parser + msgID + parser + nodeID + parser + destination + parser + path + parser + payload;

      LoRa.beginPacket();
      LoRa.print(sentMsg);
      LoRa.endPacket(true);

      Serial.print(F("Answer RREQ Message with RREP "));
      Serial.println(getMsgID);
      Serial.print(F("From : "));
      Serial.println(message);
      Serial.print(F("With : "));
      Serial.println(sentMsg);

      updateMsgHistory(getMsgID);

      return;
    }

    //====================================================================
    // Code 10 ==> General Message =======================================
    //====================================================================
    if(getMsgCode == code("msg"))
    {
      Serial.print(F("GET Message from "));
      Serial.println(getMsgSender);
      Serial.print(F("Message : "));
      Serial.println(getMsgPayload);

      updateMsgHistory(getMsgID);

      return;
    }

  }
}

