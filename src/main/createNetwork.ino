void createNetwork()
{
  // Create network
  randomSeed(analogRead(0));
  long randNumber = random(100000, 999999);
  network.id = String(randNumber);
  network.networkMember[0] = nodeID;
  network.routingID = timeNow();

  Serial.println("\n========================================");
  Serial.println("Create Network with ID " + network.id);
  Serial.println("========================================\n");
}
