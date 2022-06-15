//====================================================================
//====================================================================
// Function to send Message in Mesh Network
//
// via sendMessage(struct destination, String payload)
//====================================================================
//====================================================================

int sendMessage(route destinationRoute, String payload)
{
  // if payload is larger, return error code 413
  if(payload.length() > 150){return 413;}

  // if no route path, do RREQ
  if(destinationRoute.routePath == "")
  {
    // Do RRQ (route request)
    int RREQ = routeDiscovery(destinationRoute);

    // if route request error, return 404 code
    if(RREQ == 404){return 404;}
  }

  if(destinationRoute.routePath != "")
  {
    // Send message as usuall
    String msgCode = code("msg");
    DateTime now = rtc.now();
    String msgID = String(now.unixtime(), HEX); 

    String destination = destinationRoute.destination;
    String path = destinationRoute.routePath;
    Serial.print("Send message to ");
    Serial.println(destination);
    Serial.print("Payload : ");
    Serial.println(payload);

    // Send ping msg
    LoRa.beginPacket();
    String sentMsg = msgCode + parser + msgID + parser + nodeID + parser + destination + parser + path + parser + payload;
    LoRa.print(sentMsg);
    LoRa.endPacket();

    // return send success
    return 200;
  }
}