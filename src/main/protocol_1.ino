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
  switch (parsing(messageReceived, '|', 0).toInt()){
    case 10:
      protocol_1_case_10();
    break;
    
    case 12:
      protocol_1_case_12();
    break;
  }
}

void protocol_1_case_10()
{
  if (parsing(messageReceived, '|', 2) == network.id)
  {
    LoRa.beginPacket();
    messageSent = "11" + parser + nodeID + parser + network.id;
    LoRa.print(messageSent);
    LoRa.endPacket();
  }
}

void protocol_1_case_12()
{
  if (messageIsForMe() == true && isMyNeighbour() == true)
  {
    int neighbourID = parsing(messageReceived, '|', 4).toInt();
    LoRa.beginPacket();
    String recipient = parsing(messageReceived, '|', 1);
    messageSent = "13" + parser + nodeID + parser + recipient  + parser + network.networkMember[neighbourID] + parser + network.memberNeighbour[neighbourID];
    LoRa.print(messageSent);
    LoRa.endPacket();
  }
}