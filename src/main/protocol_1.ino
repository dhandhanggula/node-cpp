//====================================================================
//====================================================================
// Protocol 1 is a protocol for routing and path finding
// 10 - Finding nearest neighbour in network
// 11 - Answer of "10" 
// 12 - Ask for routing table to the neighbour node
// 13 - Answer of "12" and give routing table
//
// Status:
// Build : DEVELOPMENT
// Tested : -
//====================================================================

void protocol_1()
{
  // if get 10, send it ID again
  if (parsing(messageReceived, '|', 0) == "10" && parsing(messageReceived, '|', 2) == network.id)
  {
    LoRa.beginPacket();
    messageSent = "11" + parser + nodeID + parser + network.id;
    LoRa.print(messageSent);
    LoRa.endPacket();
  }

  // if get 12, send routing table
  if (parsing(messageReceived, '|', 0) == "12" && parsing(messageReceived, '|', 2) == network.id)
  {
    int neighbourID = parsing(messageReceived, '|', 3).toInt();
    LoRa.beginPacket();
    messageSent = "13" + parser + network.id + parser + network.networkMember[neighbourID] + parser + network.memberNeighbour[neighbourID];
    LoRa.print(messageSent);
    LoRa.endPacket();
  }
}