//====================================================================
//====================================================================
// Function to send Message in Mesh Network
//
// via sendMessage(struct destination, String payload)
//====================================================================
//====================================================================

/*
int sendMessage(String destinationRoute, String payload)
{
  // if payload is larger, return error code 413
  if(payload.length() > 150){return 413;}

  int routeBookNumber;

  for(int i=0; i<30; i++)
  {
    if(String(routeBook[i].destination) == destinationRoute)
    {
      // if route path not null
      if(routeBook[i].routePath != "")
      {
        // Send message as usuall
        String msgCode = code("msg");
        DateTime now = rtc.now();
        String msgID = String(now.unixtime(), HEX); 

        String destination = routeBook[i].destination;
        String path = routeBook[i].routePath;
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
  }

  //do RREQ
  Serial.println("Destination not found. Doing RREQ . . .");
  //int RREQ = routeDiscovery(destinationRoute);
  //if(RREQ == 404){return 404;}

  // Send msg next time
  Serial.println("Message is canceled. Send message later");

  // if no route path, do RREQ
  if(routeBook[routeBookNumber].routePath == "")
  {
    // Do RRQ (route request)
    //int RREQ = routeDiscovery(destinationRoute);

    // if route request error, return 404 code
    //if(RREQ == 404){return 404;}
  }

}
*/