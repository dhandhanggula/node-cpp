// ping to specific target id

bool ping(String target_id)
{
  Serial.print("Ping to ");
  Serial.println(target_id);

  // send ping to target
  LoRa.beginPacket();
  String pingMessage = "14" + parser + nodeID + parser + target_id + parser + network.id;
  LoRa.print(pingMessage);
  LoRa.endPacket();

  // waiting for answer
  String networkMessage = "";
  unsigned long previousMillis = 0;
  bool confirmation = false;
  bool connected = false;

  // ======================Start While==========================
  // Receiver mode until get response or timeout
  while(confirmation == false)
  {
    int packetSize = LoRa.parsePacket();

    if (packetSize)
    {
      while (LoRa.available())
      {
        networkMessage += (char)LoRa.read();
      }

      // if message is for this node and message code is right
      if(messageIsForMe(networkMessage) == true && parsing(networkMessage, '|', 0) == "15")
      {
        confirmation = true;
        connected = true;
      }

      else
      {
        networkMessage = "";
        confirmation = false;
      }

    }      
    // End connection if timeout
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 6000) {
      previousMillis = currentMillis;
      confirmation = true;
      connected = false;
    }
  }
  // ============== End of While ========================

  return connected;
}