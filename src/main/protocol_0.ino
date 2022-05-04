void protocol_0()
{
  if (parsing(messageReceived, '|', 0) == "00")
  {
    LoRa.beginPacket();
    messageSent = "01" + parser + network.id;
    LoRa.print(messageSent);
    LoRa.endPacket();
  }

  if (parsing(messageReceived, '|', 0) == "02" && parsing(messageReceived, '|', 2) == network.id)
  {
    bool notRegisteredStatus = false;
    bool freeSpace = true;
    int freeID;

    // if space is availabe, add to network
    if(network.networkMember[29] == "")
    {
      // check if id is not registered yet
      for(int i=0; i<network.maxMember; i++)
      {
        if(network.networkMember[i] == parsing(messageReceived, '|', 1))
        {
          freeSpace = false;
          i=99;
        }
      }

      // check if there is space
      for(int i=0; i<network.maxMember; i++)
      {
        if(network.networkMember[i] == "")
        {
          notRegisteredStatus = true;
          freeID = i;
          i=99;
        }
      }

      if(notRegisteredStatus = true && freeSpace == true)
      {
        network.networkMember[freeID] = parsing(messageReceived, '|', 1);

        // send confirmation
        LoRa.beginPacket();
        messageSent = "03" + parser + parsing(messageReceived, '|', 1) + parser + network.id;
        LoRa.print(messageSent);
        LoRa.endPacket();

        // print new table
        printRoutingTable();
      }

      if(notRegisteredStatus == false)
      {
        // send confirmation only without changing routing table
        LoRa.beginPacket();
        messageSent = "03" + parser + parsing(messageReceived, '|', 1) + parser + network.id;
        LoRa.print(messageSent);
        LoRa.endPacket();
      }

    }
  }

}