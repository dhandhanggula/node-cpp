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
  String getMsgCode = parsing(message, '|', 0);
  String getMsgID = parsing(message, '|', 1);
  String getMsgSender = parsing(message, '|', 2);
  String getMsgRecipient = parsing(message, '|', 3);
  String getMsgPath = parsing(message, '|', 4);
  String getMsgPayload = parsing(message, '|', 5);

  String sendPath;
  String path[] = {};

  //====================================================================
  // Code OO ==> PING ==================================================
  //====================================================================

  if(getMsgCode == code("ping"))
  {
    //Save new path
    int pathLength = charMode(getMsgPath, ',');

    if(pathLength == 0){sendPath = getMsgSender;}
    if(pathLength != 0)
    {
      // save route path to array
      for(int i=0; i <= pathLength; i++)
      {
        path[i] = parsing(getMsgPath, ',', i);
      }
    }


    if(getMsgRecipient == nodeID)
    {
      // create reverse route path back to sender
      for(int i=0; i<= pathLength; i++)
      {
        if(path[pathLength - i] != "")
        {
          sendPath += path[pathLength - i] + ",";
        }
      }

      int sendPathLength = sendPath.length();
      String lastChar = String(sendPath.charAt(sendPathLength - 1));
      if(lastChar == ",")
      {
        sendPath.remove(sendPathLength - 1);
      }
      //Serial.println(sendPath);

      // send answer
      String msgCode = code("ansPing");
      String msgID = getMsgID;
      String destination = getMsgSender;
      String path = sendPath;
      String sentMsg = msgCode + parser + msgID + parser + nodeID + parser + destination + parser + path + parser + "99";

      LoRa.beginPacket();
      LoRa.print(sentMsg);
      LoRa.endPacket();
    }

    else
    {
      // check in list path this node is exist or not
      for(int i=0; i<=pathLength; i++)
      {
        if(path[i] == nodeID);
        {
          // relay ping ==> send message as same as received message
          String sentMsg = getMsgCode + parser + getMsgID + parser + getMsgSender + parser + getMsgRecipient + parser + getMsgPath + parser + getMsgPayload;
        }
      }
    }
  }

  //====================================================================
  // Code xx ==> xx ==================================================
  //====================================================================

}
