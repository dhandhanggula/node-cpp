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

  //Save new path
  int pathLength = charMode(getMsgPath, ',');
  String path[] = {};

  if(pathLength == 0){sendPath = getMsgSender;}
  if(pathLength != 0)
  {
    for(int i=0; i <= pathLength; i++)
    {
      path[pathLength - i] = parsing(getMsgPath, ',', i);
    }

    // reverse route path back to sender
    for(int i=0; i<= pathLength; i++)
    {
      if(path[i] != "")
      {
        sendPath += path[i] + ",";
      }
    }
  }


  int sendPathLength = sendPath.length();
  String lastChar = String(sendPath.charAt(sendPathLength - 1));
  if(lastChar == ",")
  {
    sendPath.remove(sendPathLength - 1);
  }

  //Serial.println(sendPath);
  

  // Answer Ping MSG
  if(getMsgCode == code("ping"))
  {
    if(getMsgRecipient == nodeID)
    {
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
  }
}
