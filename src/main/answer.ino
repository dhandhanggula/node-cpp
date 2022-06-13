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
  String msgSender = parsing(message, '|', 2);
  String msgRecipient = parsing(message, '|', 3);
  String msgPath = parsing(message, '|', 4);
  String msgPayload = parsing(message, '|', 5);

  String sendPath;

  //Save new path
  int pathLength = charMode(msgPath, ',');
  String path[] = {};

  if(pathLength == 0)
  {
    sendPath = msgSender;
  }

  if(pathLength != 0)
  {
    for(int i=0; i <= pathLength; i++)
    {
      path[i] = parsing(msgPath, ',', i);
    }

    for(int i=0; i<= pathLength; i++)
    {
      if(path[i] != "")
      {
        sendPath += path[i] + ",";
      }
    }
  }


  int sendPathLength = sendPath.length() - 1;
  String lastChar = String(sendPath.charAt(sendPathLength));

  if(lastChar == ",")
  {
    sendPath.remove(sendPathLength);
  }

  Serial.println(sendPath);
  
  if(getMsgCode == code("ping"))
  {
    if(msgRecipient == nodeID)
    {
      // send answer
      String msgCode = code("ansPing");
      String msgID = getMsgID;
      String destination = msgSender;
      String path = sendPath;
      String sentMsg = msgCode + parser + msgID + parser + nodeID + parser + destination + parser + path + parser + "99";

      LoRa.beginPacket();
      LoRa.print(sentMsg);
      LoRa.endPacket();
    }
  }
}
