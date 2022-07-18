//====================================================================
//====================================================================
// Function to send Message in Mesh Network
//
// via sendMessage(struct destination, String payload)
//====================================================================
//====================================================================

int meshMsg(String msgDestination, String msgRoute, String msgPayload)
{
  // if payload is larger, return error code 413
  if(payload.length() > 150)
  {
    Serial.println(F("Payload is larger. Please reduce your message length"));
    return 413;
  }

  // Send message
  String msgCode = code("msg");
  DateTime now = rtc.now();
  String msgID = String(now.unixtime(), HEX); 

  LoRa.beginPacket();
  String sentMsg = msgCode + parser + msgID + parser + nodeID + parser + msgDestination + parser + msgRoute + parser + msgPayload;
  LoRa.print(sentMsg);
  LoRa.endPacket(true);

  Serial.print(F("Sent message to "));
  Serial.print(msgDestination);
  Serial.println(F(". "));
  Serial.print(F("Message : "));
  Serial.println(sentMsg);
}